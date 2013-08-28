// Copyright (C) 2008, 2009, 2011, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file apps/mesh-segm-skel/mesh-complex-max-curv-segm.cc
/// \brief A program computing the maximal curvature values from the
/// surface of the (triangle) mesh of a statue, then performing a
/// WST-based segmentation, using a complex-based image.

#include <cstdlib>
#include <cmath>

#include <utility>
#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/morpho/closing/area.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/math/pi.hh>
#include <mln/math/max.hh>
#include <mln/math/sqr.hh>

#include <mln/literal/white.hh>

#include <mln/io/off/load.hh>
#include <mln/io/off/save.hh>

#include "misc.hh"


int main(int argc, char* argv[])
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
      float h = (curv.first(v) + curv.second(v)) / 2;
      // Pseudo-inverse curvature.
      float h_inv =
	1 / float(mln::math::pi) * (atanf(-h) + float(mln::math::pi) / 2);
      input(v) = h_inv;
      // FIXME: The program should allow the user to choose the kind
      // of measure.
//       input(v) = mln::math::max(mln::math::sqr(curv.first(v)),
// 				mln::math::sqr(curv.second(v)));
    }

  // Values on edges.
  mln::p_n_faces_fwd_piter<D, G> e(input.domain(), 1);
  typedef mln::complex_lower_neighborhood<D, G> adj_vertices_nbh_t;
  adj_vertices_nbh_t adj_vertices_nbh;
  mln_niter_(adj_vertices_nbh_t) adj_v(adj_vertices_nbh, e);
  // Iterate on edges (1-faces).
  for_all(e)
  {
    float s = 0.0f;
    unsigned n = 0;
    // Iterate on vertices (0-faces).
    for_all(adj_v)
    {
      s += input(adj_v);
      ++n;
    }
    input(e) = s / n;
    // An edge should be adjacent to exactly two vertices.
    mln_invariant(n <= 2);
  }

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  /// Adjacent edges are connected by shared polygons.
  typedef
    mln::complex_higher_dim_connected_n_face_neighborhood<D, G>
    adj_edges_nbh_t;
  adj_edges_nbh_t adj_edges_nbh;

  ima_t closed_input = mln::morpho::closing::area(input, adj_edges_nbh, lambda);

  /*------.
  | WST.  |
  `------*/

  /* FIXME: Note that the WST is doing too much work, since we have
     not constrained the domain of the image to 1-faces only.
     It would be great if we could use something like this:

       closed_input | mln::p_faces<1, D, G>(closed_input.domain())

     as input of the WST.  */

  // Compute the WST on edges.
  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  typedef mln::unsigned_2complex_image3df wst_ima_t;
  wst_ima_t wshed =
    mln::morpho::watershed::flooding(closed_input, adj_edges_nbh, nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  // Label polygons (i.e., propagate labels from edges to polygons).
  typedef mln::complex_higher_neighborhood<D, G> adj_polygons_nbh_t;
  adj_polygons_nbh_t adj_polygons_nbh;
  mln_niter_(adj_polygons_nbh_t) adj_p(adj_polygons_nbh, e);
  for_all(e)
    if (wshed(e) != 0)
      for_all(adj_p)
	wshed(adj_p) = wshed(e);

  /*---------.
  | Output.  |
  `---------*/

  mln::rgb8_2complex_image3df output(wshed.domain());
  mln::data::fill(output, mln::literal::white);

  // FIXME: Use a colorize functor instead.
  // Choose random colors for each basin number.
  std::vector<mln::value::rgb8> basin_color (nbasins + 1);
  for (unsigned i = 0; i <= nbasins; ++i)
    basin_color[i] = mln::value::rgb8(random() % 256,
				      random() % 256,
				      random() % 256);
  mln_piter_(ima_t) f(wshed.domain());
  for_all(f)
    output(f) = basin_color[wshed(f)];

  mln::io::off::save(output, output_filename);
}
