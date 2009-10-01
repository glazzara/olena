// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef APPS_GRAPH_MORPHO_IO_HH
# define APPS_GRAPH_MORPHO_IO_HH

/// \file apps/graph-morpho/io.hh
/// \brief I/O routines for graphs (1-complexes).

# include <mln/core/alias/complex_image.hh>
# include <mln/core/image/image2d.hh>

# include <mln/math/abs.hh>

// FIXME: We should turn these routines into something much more
// generic, and move it to the library (into make/).

inline
mln::bin_1complex_image2d
build_regular_complex1d_image(const mln::box2d& support)
{
  using namespace mln;

  unsigned nrows = support.pmax().row() - support.pmin().row() + 1;
  unsigned ncols = support.pmax().col() - support.pmin().col() + 1;

  const unsigned dim = 1;
  typedef topo::n_face<0, dim> vertex_t;

  typedef topo::complex<dim> complex_t;
  complex_t c;
  typedef geom::complex_geometry<dim, point2d> geom_t;
  geom_t geom;

  // Vertices.
  for (unsigned row = 0; row < nrows; ++row)
    for (unsigned col = 0; col < ncols; ++col)
      {
	c.add_face();
	geom.add_location(point2d(row,col));
      }

  // Edges.
  for (unsigned row = 0; row < nrows; ++row)
  {
    // Horizontal edges.
    for (unsigned col = 1; col < ncols; ++col)
      {
	// First vertex.
	vertex_t v1(c, row * ncols + col - 1);
	// Second vertex.
	vertex_t v2(c, row * ncols + col);
	// Edge bewteen V1 and V2.
	c.add_face(v1 + v2);
      }

    // Vertical edges.
    if (row != 0)
      for (unsigned col = 0; col < ncols; ++col)
	{
	  // First vertex.
	  vertex_t v1(c, (row - 1) * ncols + col);
	  // Second vertex.
	  vertex_t v2(c, row * ncols + col);
	  // Edge bewteen V1 and V2.
	  c.add_face(v1 + v2);
	}
  }

  // Site set (domain) of the image.
  p_complex<dim, geom_t> pc(c, geom);

  // Image based on this site set.
  bin_1complex_image2d ima(pc);
  return ima;
}


template <typename I>
inline
mln::bin_1complex_image2d
make_regular_complex1d_image(const mln::Image<I>& input_)
{
  using namespace mln;

  const I& input = exact(input_);
  const box2d& input_box = input.domain();
  // The input image must have an odd number of rows and columns.
  mln_precondition(input_box.nrows() % 2 == 1);
  mln_precondition(input_box.ncols() % 2 == 1);

  // The domain of the graph image is twice as small, since we
  // consider only vertices (edges are set ``between'' vertices).
  box2d output_box(input_box.nrows() / 2 + 1,
		   input_box.ncols() / 2 + 1);
  bin_1complex_image2d output = build_regular_complex1d_image(output_box);

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


#endif // ! APPS_GRAPH_MORPHO_IO_HH
