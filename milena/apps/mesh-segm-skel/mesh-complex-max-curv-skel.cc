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
/// skeleton of this surface (by/ thinning), using a complex-based
/// image.

#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/core/site_set/p_set.hh>

#include <mln/value/label_16.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/morpho/closing/area.hh>

#include <mln/topo/is_n_face.hh>
#include <mln/topo/is_simple_cell.hh>
#include <mln/topo/detach.hh>
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

  // Image type.
  typedef mln::float_2complex_image3df ima_t;
  // Dimension of the image (and therefore of the complex).
  static const unsigned D = ima_t::dim;
  // Geometry of the image.
  typedef mln_geom_(ima_t) G;

  mln::bin_2complex_image3df bin_input;
  mln::io::off::load(bin_input, input_filename);
  std::pair<ima_t, ima_t> curv = mln::geom::mesh_curvature(bin_input.domain());

  // Compute the pseudo_inverse curvature at each vertex.
  ima_t input(bin_input.domain());
  mln::p_n_faces_fwd_piter<D, G> v(input.domain(), 0);
  for_all(v)
    {
      // FIXME: The program should allow the user to choose the kind
      // of measure.
#if 0
      float h = (curv.first(v) + curv.second(v)) / 2;
      // Pseudo-inverse curvature.
      float h_inv = 1 / pi * (atan(-h) + pi / 2);
      input(v) = h_inv;
#endif
      // Max curvature.
      input(v) = mln::math::max(mln::math::sqr(curv.first(v)),
 				mln::math::sqr(curv.second(v)));
    }

  // Values on triangles.
  mln::p_n_faces_fwd_piter<D, G> t(input.domain(), 2); 
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
      s += input(adj_v);
      ++n;
    }
    input(t) = s / n;
    // A triangle should be adjacent to exactly two vertices.
    mln_invariant(n <= 3);
  }

  /* FIXME: Workaround: Set maximal values on vertices and edges to
     exclude them from the set of minimal values.  */
  for_all(v)
    input(v) = mln_max(float);
  mln::p_n_faces_fwd_piter<D, G> e(input.domain(), 1);
  for_all(e)
    input(e) = mln_max(float);

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  /// Adjacent triangles are connected by shared edges.
  typedef mln::complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;

  ima_t closed_input = mln::morpho::closing::area(input, nbh, lambda);

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
    mln::labeling::regional_minima(closed_input, nbh, nminima);

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
	  // If V is adjcent to a non-minimal triangle, then it must
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

  mln::topo::is_simple_cell<bin_ima_t> is_simple_p;
    /* FIXME: Cheat!  We'd like to iterate on cells of highest
       dimension (2-cells) only, but we cannot constrain the domain of
       the input using image_if (yet) like this

         breadth_first_thinning(surface | is_n_face<2>, nbh, is_simple_p);

       As a workaround, we use the constraint predicate of the
       skeleton routine to restrict the iteration to 2-cells.  */
  mln::topo::is_n_face<bin_ima_t::dim> constraint_p;
  bin_ima_t skel =
    mln::topo::skeleton::breadth_first_thinning(surface, nbh,
						is_simple_p,
						mln::topo::detach<D, G>,
						constraint_p);

  /*---------.
  | Output.  |
  `---------*/

  /* FIXME: This does not work (yet).
     Use workaround mln::io::off::save_bin_alt instead (bad!)  */
#if 0
  mln::io::off::save(skel | mln::pw::value(skel) == mln::pw::cst(true),
		     output_filename);
#endif
  mln::io::off::save_bin_alt(skel, output_filename);
}
