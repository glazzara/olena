// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef APPS_GRAPH_MORPHO_CONVERT_HH
# define APPS_GRAPH_MORPHO_CONVERT_HH

/// \file
/// \brief Conversion between various graph/complex image types.

# include <mln/core/alias/complex_image.hh>
# include <mln/core/image/image2d.hh>

# include <mln/math/abs.hh>

# include "apps/graph-morpho/make.hh"


namespace convert
{

  /// Convert an mln::bin_1complex_image2d to an mln::image2d<bool>.
  inline
  mln::image2d<bool>
  to_image2d(const mln::bin_1complex_image2d& input)
  {
    using namespace mln;

    const unsigned dim = 1;
    typedef geom::complex_geometry<dim, point2d> geom_t;

    // Compute the bounding box of the domain of FROM.
    accu::shape::bbox<point2d> bbox;
    p_n_faces_fwd_piter<dim, geom_t> v(input.domain(), 0);
    for_all(v)
    {
      mln_site_(geom_t) s(v);
      // Site S is point2d multi-site and should be a singleton (since V
      // iterates on vertices).
      mln_invariant(s.size() == 1);
      point2d p = s.front();
      bbox.take(p);
    }
    mln::box2d support = bbox;

    point2d box_p_min(support.pmin().row() * 2, support.pmin().col() * 2);
    point2d box_p_max(support.pmax().row() * 2, support.pmax().col() * 2);
    image2d<bool> output(box2d(box_p_min, box_p_max));
    data::fill(output, false);

    // Iterate on vertices.
    for_all(v)
    {
      mln_site_(geom_t) s(v);
      // Site S is point2d multi-site and should be a singleton (since V
      // iterates on vertices).
      mln_invariant(s.size() == 1);
      point2d p_in = s.front();
      point2d p_out(p_in.row() * 2, p_in.col() * 2);
      output(p_out) = input(v);
    }

    // Iterate on edges.
    p_n_faces_fwd_piter<dim, geom_t> e(input.domain(), 1);
    for_all(e)
    {
      mln_site_(geom_t) s(e);
      // Site S is point2d multi-site and should be a pair (since E
      // iterates on vertices).
      mln_invariant(s.size() == 2);
      point2d p1 = s[0];
      point2d p2 = s[1];
      if (p1.row() == p2.row())
	{
	  // Horizontal edge.
	  point2d p_out(p1.row() * 2,
			p1.col() + p2.col());
	  output(p_out) = input(e);
	}
      else if (p1.col() == p2.col())
	{
	  // Vertical edge.
	  point2d p_out(p1.row() + p2.row(),
			p1.col() * 2);
	  output(p_out) = input(e);
	}
      else
	{
	  // Edge not fitting in the 2D regular grid canvas, aborting.
	  abort();
	}
    }
    return output;
  }


  /// Convert an mln::image2d<bool> to an mln::bin_1complex_image2d.
  inline
  mln::bin_1complex_image2d
  to_complex_image(const mln::image2d<bool>& input)
  {
    using namespace mln;
    
    const box2d& input_box = input.domain();
    // The input image must have an odd number of rows and columns.
    mln_precondition(input_box.nrows() % 2 == 1);
    mln_precondition(input_box.ncols() % 2 == 1);

    // The domain of the graph image is twice as small, since we
    // consider only vertices (edges are set ``between'' vertices).
    box2d output_box(input_box.nrows() / 2 + 1,
		     input_box.ncols() / 2 + 1);
    bin_1complex_image2d output = ::make::complex1d_image<bool>(output_box);

    const unsigned dim = 1;
    typedef geom::complex_geometry<dim, point2d> geom_t;

    // Add values on vertices.
    p_n_faces_fwd_piter<dim, geom_t> v(output.domain(), 0);
    for_all(v)
    {
      mln_site_(geom_t) s(v);
      // Site S is point2d multi-site and should be a singleton (since V
      // iterates on vertices).
      mln_invariant(s.size() == 1);
      point2d p = s.front();
      point2d q(p.row() * 2, p.col() * 2);
      output(v) = input(q);
    }

    // Add values on edges.
    p_n_faces_fwd_piter<dim, geom_t> e(output.domain(), 1);
    for_all(e)
    {
      mln_site_(geom_t) s(e);
      // Site S is point2d multi-site and should be a pair (since E
      // iterates on vertices).
      mln_invariant(s.size() == 2);
      point2d p1 = s[0];
      point2d p2 = s[1];
      mln_invariant(math::abs(p1.row() - p2.row()) == 1
		    || math::abs(p1.col() - p2.col()) == 1);
      point2d q (p1.row() + p2.row(), p1.col() + p2.col());
      output(e) = input(q);
    }

    return output;
  }

} // end of namespace convert


#endif // ! APPS_GRAPH_MORPHO_CONVERT_HH
