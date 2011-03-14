// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Exercise mln::topo::is_simple_pair.

#include <mln/core/alias/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/topo/is_simple_pair.hh>

#include <mln/io/vtk/load.hh>

#include "tests/data.hh"


int
main()
{
  // Image type.
  typedef mln::bin_2complex_image3df ima_t;
  // Dimension of the image (and thus of the complex).
  static const unsigned D = ima_t::dim;
  // Geometry of the image.
  typedef mln_geom_(ima_t) G;

  ima_t ima;
  mln::io::vtk::load(ima, MLN_MESH_DIR "/pseudo-manifold.vtk");

  // Neighborhood type returning the set of (n-1)-faces adjacent to a
  // an n-face.
  typedef mln::complex_lower_neighborhood<D, G> lower_adj_nbh_t;
  lower_adj_nbh_t lower_adj_nbh;
  // Neighborhood type returning the set of (n+1)-faces adjacent to a
  // an n-face.
  typedef mln::complex_higher_neighborhood<D, G> higher_adj_nbh_t;
  higher_adj_nbh_t higher_adj_nbh;
  // Predicate type: does a triangle (2-face) belongs to a simple pair?
  typedef mln::topo::is_simple_pair< ima_t,
                                     lower_adj_nbh_t,
                                     higher_adj_nbh_t > is_simple_t;

  // Check the simplicity of IMA's faces.
  is_simple_t is_simple(ima, lower_adj_nbh, higher_adj_nbh);
  mln_piter_(ima_t) p(ima.domain());
  for_all(p)
  {
    if (is_simple(p))
      std::cout << p << std::endl;
  }
}
