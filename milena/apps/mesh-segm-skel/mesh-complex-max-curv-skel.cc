// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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

/// \file apps/mesh-segm-skel/mesh-complex-max-curv-skel.cc
/// \brief A program computing the maximal curvature values from the
/// surface of the (triangle) mesh of a statue, then computing a
/// skeleton of this surface (by thinning), using a complex-based
/// image.

// FIXME: Factor with mesh-complex-pinv-curv-skel.cc.

#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/mutable_extension_ima.hh>
#include <mln/core/routine/mutable_extend.hh>

#include <mln/value/label_16.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/morpho/closing/area.hh>

#include <mln/topo/is_n_face.hh>
#include <mln/topo/is_simple_cell.hh>
#include <mln/topo/detach_cell.hh>
#include <mln/topo/skeleton/breadth_first_thinning.hh>

#include <mln/io/off/load.hh>
/* FIXME: Remove as soon as mln::io::off::save is able to save a
   morphed mln::complex_image (i.e., seen through image_if).  */
#include "save_bin_alt.hh"

#include "trimesh/misc.hh"


int
main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "usage: " << argv[0] << " input.off lambda output.off"
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
  mln::io::off::load(bin_input, input_filename);
  std::pair<float_ima_t, float_ima_t> curv =
    mln::geom::mesh_curvature(bin_input.domain());

  // Compute the pseudo_inverse curvature at each vertex.
  float_ima_t float_ima(bin_input.domain());
  mln::p_n_faces_fwd_piter<D, G> v(float_ima.domain(), 0);
  for_all(v)
    {
      // Max curvature.
      float_ima(v) = mln::math::max(mln::math::sqr(curv.first(v)),
				    mln::math::sqr(curv.second(v)));
    }

  // Values on triangles.
  mln::p_n_faces_fwd_piter<D, G> t(float_ima.domain(), 2); 
  // For each triangle (2-face) T, iterate on the the set of vertices
  // (0-faces) transitively adjacent to T.
  typedef mln::complex_m_face_neighborhood<D, G> adj_vertices_nbh_t;
  adj_vertices_nbh_t adj_vertices_nbh;
  mln_niter_(adj_vertices_nbh_t) adj_v(adj_vertices_nbh, t);
  /* FIXME: We should be able to pass this value (m) either at the
     construction of the neighborhood or at the construction of the
     iterator.  */
  adj_v.iter().set_m(0);
  // Iterate on triangles (2-faces).
  for_all(t)
  {
    float s = 0.0f;
    unsigned n = 0;
    // Iterate on vertices (0-faces).
    for_all(adj_v)
    {
      s += float_ima(adj_v);
      ++n;
    }
    float_ima(t) = s / n;
    // A triangle should be adjacent to exactly two vertices.
    mln_invariant(n <= 3);
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

  /* FIXME: Workaround: Set maximal values on vertices and edges to
     exclude them from the set of minimal values.  */
  for_all(v)
    ima(v) = mln_max(mln_value_(ima_t));
  mln::p_n_faces_fwd_piter<D, G> e(float_ima.domain(), 1);
  for_all(e)
    ima(e) = mln_max(mln_value_(ima_t));

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  /// Adjacent triangles are connected by shared edges.
  typedef mln::complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;

  ima_t closed_ima = mln::morpho::closing::area(ima, nbh, lambda);

  /*---------------.
  | Local minima.  |
  `---------------*/

  typedef mln::value::label_16 label_t;
  label_t nminima;

  /* FIXME: We should use something like `ima_t | p_n_faces(2)' instead
     of `ima_t' here.  Or better: `input' should only associate data
     to 2-faces.  */
  typedef mln_ch_value_(ima_t, label_t) label_ima_t;
  label_ima_t minima =
    mln::labeling::regional_minima(closed_ima, nbh, nminima);

  typedef mln::complex_higher_neighborhood<D, G> higher_nbh_t;
  higher_nbh_t higher_nbh;

  // Propagate minima values from triangles to edges.
  // FIXME: Factor this inside a function.
  mln_niter_(higher_nbh_t) adj_t(higher_nbh, e);
  for_all(e)
  {
    label_t ref_adj_minimum = mln::literal::zero;
    for_all(adj_t)
      if (minima(adj_t) == mln::literal::zero)
	{
	  // If E is adjacent to a non-minimal triangle, then it must
	  // not belong to a minima.
	  ref_adj_minimum = mln::literal::zero;
	  break;
	}
      else
	{
	  if (ref_adj_minimum == mln::literal::zero)
	    // If this is the first minimum seen, use it as a reference.
	    ref_adj_minimum = minima(adj_t);
	  else
	    // If this is not the first time a minimum is encountered,
	    // ensure it is REF_ADJ_MINIMUM.
	    mln_assertion(minima(adj_t) == ref_adj_minimum);
	}
    minima(e) = ref_adj_minimum;
  }

  // Likewise from edges to edges to vertices.
  mln_niter_(higher_nbh_t) adj_e(higher_nbh, v);
  for_all(v)
  {
    label_t ref_adj_minimum = mln::literal::zero;
    for_all(adj_e)
      if (minima(adj_e) == mln::literal::zero)
	{
	  // If V is adjacent to a non-minimal triangle, then it must
	  // not belong to a minima.
	  ref_adj_minimum = mln::literal::zero;
	  break;
	}
      else
	{
	  if (ref_adj_minimum == mln::literal::zero)
	    // If this is the first minimum seen, use it as a reference.
	    ref_adj_minimum = minima(adj_e);
	  else
	    // If this is not the first time a minimum is encountered,
	    // ensure it is REF_ADJ_MINIMUM.
	    mln_assertion(minima(adj_e) == ref_adj_minimum);
	}
    minima(v) = ref_adj_minimum;
  }

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
  mln::data::fill(surface, true);
  // Dig ``holes'' in the surface surface by setting minima faces to false.
  // FIXME: Use fill with an image_if instead, when available/working.
  mln_piter_(bin_ima_t) f(minima.domain());
  for_all(f)
    if (minima(f) != mln::literal::zero)
      surface(f) = false;  

  /*-----------.
  | Skeleton.  |
  `-----------*/

  // ---------------- //
  // Skeleton image.  //
  // ---------------- //

  // Predicate type: is a face a triangle (2-face)?
  typedef mln::topo::is_n_face<mln_psite_(bin_ima_t), D> is_a_triangle_t;
  is_a_triangle_t is_a_triangle;
  // Surface image type, of which domain is restricted to triangles.
  typedef mln::image_if<bin_ima_t, is_a_triangle_t> bin_triangle_only_ima_t;
  // Surface image type, of which iteration (not domain) is restricted
  // to triangles.
  typedef mln::mutable_extension_ima<bin_triangle_only_ima_t, bin_ima_t>
    bin_triangle_ima_t;
  // FIXME: Find a shorter name (skel_ima ?  Careful, there is already a `skel' image below).
  bin_triangle_ima_t bin_triangle_ima =
    mln::mutable_extend((surface | is_a_triangle).rw(), surface);

  // ------------------------ //
  // Simple point predicate.  //
  // ------------------------ //

  // Neighborhood type returning the set of (n-1)- and (n+1)-faces
  // adjacent to a an n-face.
  typedef mln::complex_lower_higher_neighborhood<D, G> adj_nbh_t;
  // Neighborhood type returning the set of (n-1)-faces adjacent to a
  // an n-face.
  typedef mln::complex_lower_neighborhood<D, G> lower_adj_nbh_t;
  // Neighborhood type returning the set of (n+1)-faces adjacent to a
  // an n-face.
  typedef mln::complex_higher_neighborhood<D, G> higher_adj_nbh_t;
  // Predicate type: is a triangle (2-face) simple?
  typedef mln::topo::is_simple_cell< bin_triangle_ima_t,
                                     adj_nbh_t,
                                     lower_adj_nbh_t,
                                     higher_adj_nbh_t >
    is_simple_triangle_t;
  is_simple_triangle_t is_simple_triangle;

  // ------------------------------- //
  // Simple point detach procedure.  //
  // ------------------------------- //

  // Type of adjacency relationships between faces of immediately
  // lower and higher dimensions.
  adj_nbh_t adj_nbh;
  // Functor detaching a cell.
  mln::topo::detach_cell<bin_triangle_ima_t, adj_nbh_t> detach(adj_nbh);

  mln_concrete_(bin_triangle_ima_t) skel =
    mln::topo::skeleton::breadth_first_thinning(bin_triangle_ima,
						nbh,
						is_simple_triangle,
						detach);


  /*---------.
  | Output.  |
  `---------*/

  /* FIXME: This does not work (yet).
     Use workaround mln::io::off::save_bin_alt instead (bad!)

     Moreover, even if it worked, it would not have the same meaning
     as mln::io::off::save_bin_alt.  Maybe the latter is useful, after
     all.  But we need to factor it with the code of
     mln::io::off::save, anyway.  */
#if 0
  mln::io::off::save(skel | mln::pw::value(skel) == mln::pw::cst(true),
		     output_filename);
#endif
  // FIXME: We have to ``unmorph'' (twice!) SKEL first, since save_bin_alt only
  // handles complex_image's.
  mln::io::off::save_bin_alt(skel.unmorph_().unmorph_(), output_filename);
}
