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

#ifndef APPS_GRAPH_MORPHO_MAKE_HH
# define APPS_GRAPH_MORPHO_MAKE_HH

/// \file
/// \brief Creation routines for complex-based images.

# include <mln/core/alias/complex_image.hh>
# include <mln/core/alias/box2d.hh>

namespace make
{

  /// Create a 1-complex images in the 2D plane of which domain is
  /// based on an mln::box2d.
  template <typename V>
  inline
  mln::complex_image<1, mln::discrete_plane_1complex_geometry, V>
  complex1d_image(const mln::box2d& support)
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
	    // Edge between V1 and V2.
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
	      // Edge between V1 and V2.
	      c.add_face(v1 + v2);
	    }
      }

    // Site set (domain) of the image.
    p_complex<dim, geom_t> pc(c, geom);

    // Image based on this site set.
    typedef complex_image<dim, geom_t, V> ima_t;
    ima_t ima(pc);
    return ima;
  }

} // end of namespace make


#endif // ! APPS_GRAPH_MORPHO_MAKE_HH
