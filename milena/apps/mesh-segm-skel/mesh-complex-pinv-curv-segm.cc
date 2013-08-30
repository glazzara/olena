// Copyright (C) 2008, 2009, 2010, 2013 EPITA Research and Development
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

/// \file apps/mesh-segm-skel/mesh-complex-pinv-curv-segm.cc
/// \brief A program computing the pseudo-inverse curvature values
/// from the surface of the (triangle) mesh of a statue, then
/// performing a WST-based segmentation, using a complex-based image.

// Factor with mesh-complex-max-curv-segm.cc.

#include <cstdlib>
#include <cmath>

#include <utility>
#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/morpho/closing/area.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/math/max.hh>
#include <mln/math/sqr.hh>

#include <mln/literal/white.hh>

#include <mln/io/off/load.hh>
#include <mln/io/off/save.hh>

#include "misc.hh"


// Doesn't C++ have a better way to express Pi?
static const float pi = 4 * atanf(1);


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
      float h = (curv.first(v) + curv.second(v)) / 2;
      // Pseudo-inverse curvature.
      float h_inv = 1 / pi * (atan(-h) + pi / 2);
      float_ima(v) = h_inv;
    }

  // Values on edges.
  mln::p_n_faces_fwd_piter<D, G> e(float_ima.domain(), 1);
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
      s += float_ima(adj_v);
      ++n;
    }
    float_ima(e) = s / n;
    // An edge should be adjacent to exactly two vertices.
    mln_invariant(n <= 2);
  }

  // Convert the float image into an unsigned image because some
  // algorithms do not handle float images well.
  /* FIXME: This is bad: float images should be handled as-is.  At
     least, we should use a decent conversion, using an optimal affine
     transformation (stretching/shrinking) or any other kind of
     interpolation.  */
  typedef mln::unsigned_2complex_image3df ima_t;
  ima_t ima(float_ima.domain());
  mln_piter_(ima_t) p(float_ima.domain());
  for_all(p)
    ima(p) = 1000 * float_ima(p);

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  /// Adjacent edges are connected by shared polygons.
  typedef
    mln::complex_higher_dim_connected_n_face_neighborhood<D, G>
    adj_edges_nbh_t;
  adj_edges_nbh_t adj_edges_nbh;

  ima_t closed_ima = mln::morpho::closing::area(ima, adj_edges_nbh, lambda);

  /*------.
  | WST.  |
  `------*/

  /* FIXME: Note that the WST is doing too much work, since we have
     not constrained the domain of the image to 1-faces only.
     It would be great if we could use something like this:

       closed_ima | mln::p_faces<1, D, G>(closed_ima.domain())

     as input of the WST.  */

  // Compute the WST on edges.
  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  typedef mln::unsigned_2complex_image3df wst_ima_t;
  wst_ima_t wshed =
    mln::morpho::meyer_wst(closed_ima, adj_edges_nbh, nbasins);
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
