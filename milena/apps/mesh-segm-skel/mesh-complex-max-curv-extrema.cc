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

/// \file apps/mesh-segm-skel/mesh-complex-max-curv-extrema.cc
/// \brief A program computing the max curvature at each (2-)face of
/// the surface of the (triangle) mesh of a statue, using
/// complex-based image, and displaying its local minima (in blue) and
/// maxima (in red).

#include <cstdlib>
#include <cmath>

#include <utility>
#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/data/fill.hh>
#include <mln/literal/zero.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/regional_maxima.hh>

#include <mln/math/max.hh>
#include <mln/math/sqr.hh>
#include <mln/accu/stat/min_max.hh>
#include <mln/fun/v2v/linear.hh>
#include <mln/data/transform.hh>

#include <mln/value/label_16.hh>

#include <mln/literal/white.hh>
#include <mln/literal/grays.hh>
#include <mln/literal/colors.hh>

#include <mln/io/off/load.hh>
#include <mln/io/off/save.hh>

#include "misc.hh"


// Doesn't C++ have a better way to express Pi?
static const float pi = 4 * atanf(1);


int main(int argc, char* argv[])
{
  if (argc != 3)
    {
      std::cerr << "usage: " << argv[0] << " input.off output.off"
		<< std::endl;
      std::exit(1);
    }

  std::string input_filename = argv[1];
  std::string output_filename = argv[2];

  /*----------------.
  | Complex image.  |
  `----------------*/

  // Image type.
  typedef mln::float_2complex_image3df ima_t;
  // Dimension of the image (and therefore of the complex).
  static const unsigned D = ima_t::dim;
  // Geometry of the image.
  typedef mln_geom_(ima_t) G;

  mln::bin_2complex_image3df input;
  mln::io::off::load(input, input_filename);

  std::pair<ima_t, ima_t> curv = mln::geom::mesh_curvature(input.domain());

  // Compute the max curvature at each vertex.
  ima_t max_curv(input.domain());
  mln::data::fill(max_curv, mln::literal::zero);
  mln::p_n_faces_fwd_piter<D, G> v(max_curv.domain(), 0);
  for_all(v)
    max_curv(v) = mln::math::max(mln::math::sqr(curv.first(v)),
				 mln::math::sqr(curv.second(v)));

  // Propagate these values on triangles.
  mln::p_n_faces_fwd_piter<D, G> t(max_curv.domain(), 2);
  typedef mln::complex_m_face_neighborhood<D, G> adj_vertices_nbh_t;
  adj_vertices_nbh_t adj_vertices_nbh;
  mln_niter_(adj_vertices_nbh_t) adj_v(adj_vertices_nbh, t);
  /* FIXME: Not really user friendly!  The `m' value should pass at
     the construction of ADJ_V.  */
  adj_v.iter().set_m(0);
  mln::accu::stat::min_max<float> acc;
  // Iterate on triangles (2-faces).
  for_all(t)
  {
    float s = 0.0f;
    unsigned n = 0;
    // Iterate on vertices (0-faces).
    for_all(adj_v)
    {
      s += max_curv(adj_v);
      ++n;
    }
    float m = s / n;
    max_curv(t) = m;
    acc.take(m);
    // A triangle should be adjacent to exactly three vertices.
    mln_invariant(n <= 3);
  }

  /*----------------.
  | Local extrema.  |
  `----------------*/

  /// Adjacent triangles are connected by shared edges.
  typedef mln::complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;

  typedef mln::value::label_16 label_t;
  typedef mln_ch_value_(ima_t, label_t) label_ima_t;

  /* FIXME: We should use something like `ima_t | p_n_faces(2)' instead
     of `ima_t' here.  Or better: `input' should only associate data
     to 2-faces.  */
  label_t nminima;
  label_ima_t minima =
    mln::labeling::regional_minima(max_curv, nbh, nminima);
  std::cout << "nminima = " << nminima << std::endl;

  label_t nmaxima;
  label_ima_t maxima =
    mln::labeling::regional_maxima(max_curv, nbh, nmaxima);
  std::cout << "nmaxima = " << nmaxima << std::endl;

  /*-----------------.
  | Colored output.  |
  `-----------------*/

  typedef mln::rgb8_2complex_image3df output_t;
  output_t output(max_curv.domain());

  for_all(t)
  {
    if (minima(t) != mln::literal::zero)
      {
	// Special (unexpected) case: T is both a minimum and a
	// maximum.  Paint it in magenta (blue + red).
	if (maxima(t) != mln::literal::zero)
	  {
	    std::cerr
	      << "warning: " << t << " is both a minimum and a maximum."
	      << std::endl;
	    output(t) = mln::literal::magenta;
	  }
	else
	  // Paint minimum T in blue.
	  output(t) = mln::literal::blue;
      }
    else if (maxima(t) != mln::literal::zero)
      // Paint maximum T in red.
      output(t) = mln::literal::red;
    else
      // Default color.
      output(t) = mln::literal::medium_gray;
  }

  // Output.
  mln::io::off::save(output, output_filename);
}
