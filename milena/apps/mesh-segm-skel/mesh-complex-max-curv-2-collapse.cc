// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file
/// \brief A program computing the maximal curvature values from the
/// surface of the (triangle) mesh of a statue, then performing a
/// 2-collapse of this surface, using a complex-based image.

// FIXME: Factor with mesh-complex-max-curv-skel.cc and
// mesh-complex-pinv-curv-skel.cc, mesh-complex-max-curv-1-collapse.cc

#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/routine/mutable_extend.hh>
#include <mln/data/paste.hh>

#include <mln/value/label_16.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/dilation.hh>

#include <mln/topo/is_n_face.hh>
#include <mln/topo/is_simple_pair.hh>
#include <mln/topo/detach_pair.hh>
#include <mln/topo/skeleton/breadth_first_thinning.hh>

#include <mln/io/vtk/load.hh>
#include <mln/io/vtk/save.hh>

#include "trimesh/misc.hh"


int
main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "usage: " << argv[0] << " input.vtk lambda output.vtk"
		<< std::endl;
      std::exit(1);
    }

  std::string input_filename = argv[1];
  unsigned lambda = atoi(argv[2]);
  std::string output_filename = argv[3];

  /*----------------.
  | Complex image.  |
  `----------------*/

  // Curvature image type.
  typedef mln::float_2complex_image3df float_ima_t;
  // Dimension of the image (and therefore of the complex).
  static const unsigned D = float_ima_t::dim;
  // Geometry of the image.
  typedef mln_geom_(float_ima_t) G;

  mln::bin_2complex_image3df bin_input;
  mln::io::vtk::load(bin_input, input_filename);
  std::pair<float_ima_t, float_ima_t> curv =
    mln::geom::mesh_curvature(bin_input.domain());

  // Compute the max curvature at each vertex.
  float_ima_t float_ima(bin_input.domain());
  mln::p_n_faces_fwd_piter<D, G> v(float_ima.domain(), 0);
  for_all(v)
    {
      // Max curvature.
      float_ima(v) = mln::math::max(mln::math::sqr(curv.first(v)),
				    mln::math::sqr(curv.second(v)));
    }

  // Neighborhood type returning the set of (n-1)-faces adjacent to a
  // an n-face.
  typedef mln::complex_lower_neighborhood<D, G> lower_adj_nbh_t;
  lower_adj_nbh_t lower_adj_nbh;

  // Values on edges.
  /* FIXME: We could probably simplify this by using a
     convolution-like operator and morphers (see
     apps/graph-morpho).  */
  mln::p_n_faces_fwd_piter<D, G> e(float_ima.domain(), 1);
  // For each edge (1-face) E, iterate on the the set of vertices
  // (0-faces) adjacent to E.
  mln_niter_(lower_adj_nbh_t) adj_v(lower_adj_nbh, e);
  // Iterate on edges (1-faces).
  for_all(e)
  {
    float s = 0.0f;
    unsigned n = 0;
    // Iterate on vertices (0-faces).
    for_all(adj_v)
    {
      s += float_ima(adj_v);
      ++n;
    }
    float_ima(e) = s / n;
    // An edge should be adjacent to exactly two vertices.
    mln_invariant(n == 2);
  }

  // Values on triangles.
  /* FIXME: We could probably simplify this by using a
     convolution-like operator and morphers (see
     apps/graph-morpho).  */
  mln::p_n_faces_fwd_piter<D, G> t(float_ima.domain(), 2);
  // For each triangle (2-face) T, iterate on the the set of edges
  // (1-faces) adjacent to T.
  mln_niter_(lower_adj_nbh_t) adj_e(lower_adj_nbh, t);
  // Iterate on triangles (2-faces).
  for_all(t)
  {
    float s = 0.0f;
    unsigned n = 0;
    // Iterate on edges (1-faces).
    for_all(adj_e)
    {
      s += float_ima(adj_e);
      ++n;
    }
    float_ima(t) = s / n;
    // A triangle should be adjacent to exactly three edges.
    mln_invariant(n == 3);
  }

  // Convert the float image into an unsigned image because some
  // algorithms do not handle float images well.
  /* FIXME: This is bad: float images should be handled as-is.  At
     least, we should use a decent conversion, using an optimal affine
     transformation (stretching/shrinking) or any other kind of
     interpolation.  */
  typedef mln::unsigned_2complex_image3df ima_t;
  ima_t ima(float_ima.domain());
  // Process only triangles, as edges and vertices are set afterwards
  // (see FIXME below).
  for_all(t)
    ima(t) = 1000 * float_ima(t);

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  /// Adjacent triangles are connected by shared edges.
  typedef mln::complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;

  // Predicate type: is a face a triangle (2-face)?
  typedef mln::topo::is_n_face<mln_psite_(ima_t), 2> is_a_triangle_t;
  is_a_triangle_t is_a_triangle;

  // Consider only triangles.
  ima_t closed_ima = mln::duplicate(ima);
  mln::data::paste(mln::morpho::closing::area(ima | is_a_triangle,
					      nbh, lambda),
		   closed_ima);

  /*---------------.
  | Local minima.  |
  `---------------*/

  typedef mln::value::label_16 label_t;
  label_t nminima;

  // Consider only triangles.
  typedef mln_ch_value_(ima_t, label_t) label_ima_t;
  label_ima_t minima;
  mln::initialize(minima, closed_ima);
  mln::data::paste(mln::labeling::regional_minima(closed_ima | is_a_triangle,
						  nbh, nminima),
	     minima);

  /*-----------------------.
  | Initial binary image.  |
  `-----------------------*/

  /* Careful: creating ``holes'' in the surface obviously changes its
     topology, but it may also split a single connected component in
     two or more components, resulting in a disconnected skeleton.  We
     may want to improve this step either by forbidding any splitting,
     or by incrementally ``digging'' a regional minima as long as no
     splitting occurs.  */

  typedef mln_ch_value_(ima_t, bool) bin_ima_t;
  bin_ima_t surface(minima.domain());

  // Predicate type: is a face an edge (1-face)?
  typedef mln::topo::is_n_face<mln_psite_(ima_t), 1> is_an_edge_t;
  is_an_edge_t is_an_edge;
  // Predicate type: is a face a vertex (0-face)?
  typedef mln::topo::is_n_face<mln_psite_(ima_t), 0> is_a_vertex_t;
  is_a_vertex_t is_a_vertex;

  // Neighborhood type returning the set of (n+1)-faces adjacent to a
  // an n-face.
  typedef mln::complex_higher_neighborhood<D, G> higher_adj_nbh_t;
  higher_adj_nbh_t higher_adj_nbh;

  mln::data::fill(surface, false);
  // Set non minima triangles to true;
  mln::data::fill
    ((surface |
      mln::pw::value(minima) == mln::pw::cst(mln::literal::zero)).rw(),
     true);
  // Extend non minima values from triangles to edges.
  mln::data::paste (mln::morpho::dilation(mln::extend(surface | is_an_edge,
						      surface),
					  /* Dilations require windows,
					     not neighborhoods.  */
					  higher_adj_nbh.win()),
		    surface);
  // Extend non minima values from edges to vertices.
  mln::data::paste(mln::morpho::dilation(mln::extend(surface | is_a_vertex,
						     surface),
					  /* Dilations require windows,
					     not neighborhoods.  */
					 higher_adj_nbh.win()),
		   surface);

  /*-------------.
  | 2-collapse.  |
  `-------------*/

  // ------------------------------- //
  // Image restricted to triangles.  //
  // ------------------------------- //

  // Surface image type, of which domain is restricted to triangles.
  typedef mln::image_if<bin_ima_t, is_a_triangle_t> bin_triangle_only_ima_t;
  // Surface image type, of which iteration (not domain) is restricted
  // to triangles.
  typedef mln::mutable_extension_ima<bin_triangle_only_ima_t, bin_ima_t>
    bin_triangle_ima_t;

  // ------------------------ //
  // Simple point predicate.  //
  // ------------------------ //

  // Predicate type: is a triangle (2-face) simple?
  typedef mln::topo::is_simple_pair< bin_triangle_ima_t,
                                     lower_adj_nbh_t,
                                     higher_adj_nbh_t >
    is_simple_triangle_t;
  is_simple_triangle_t is_simple_triangle(lower_adj_nbh, higher_adj_nbh);

  // ------------------------------- //
  // Simple point detach procedure.  //
  // ------------------------------- //

  // Functor detaching a cell.
  typedef mln::topo::detach_pair< bin_triangle_ima_t,
                                  lower_adj_nbh_t,
                                  higher_adj_nbh_t > detach_triangle_t;
  detach_triangle_t detach_triangle(lower_adj_nbh, higher_adj_nbh);

  // ------------------------ //
  // Thinning by 2-collapse.  //
  // ------------------------ //

  mln_concrete_(bin_ima_t) surface_2_collapse;
  mln::initialize(surface_2_collapse, surface);
  mln::data::paste
    (mln::topo::skeleton::breadth_first_thinning
     (mln::mutable_extend((surface | is_a_triangle).rw(), surface),
      nbh,
      is_simple_triangle,
      detach_triangle)
     /* Before pasting the result of the computation into
        SURFACE_2_COLLAPSE, re-expand its domain to the initial site
        set, to ensure data from all faces (i.e., both the 2-faces,
        directly processed; and the 1-faces from the extension,
        undirectly processed).  */
     | surface.domain(),
     surface_2_collapse);

  /*---------.
  | Output.  |
  `---------*/

  mln::io::vtk::save(surface_2_collapse, output_filename);
}
