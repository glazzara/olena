// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file apps/mesh-segm-skel/mesh-complex-max-curv.cc
/// \brief A program computing the max curvature at each (2-)face of
/// the surface of the (triangle) mesh of a statue, using a
/// complex-based image.

#include <cstdlib>
#include <cmath>

#include <utility>
#include <iostream>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>

#include <mln/data/fill.hh>
#include <mln/literal/zero.hh>

#include <mln/math/max.hh>
#include <mln/math/sqr.hh>
#include <mln/accu/stat/min_max.hh>
#include <mln/fun/v2v/linear.hh>
#include <mln/data/transform.hh>

#include <mln/literal/white.hh>

#include <mln/io/off/load.hh>
#include <mln/io/off/save.hh>

#include "trimesh/misc.hh"


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

//   // ------------------------------------------------------------
//   // FIXME: TEST.
//   mln::complex_image< 2, mln::space_2complex_geometry, mln::algebra::vec<3, float> >
//     normal = mesh_normal(input.domain());
//   mln::p_n_faces_fwd_piter<D, G> v_(normal.domain(), 0);
//   for_all(v_)
//     std::cout << normal(v_) << std::endl;
//   std::exit(0);
//   // ------------------------------------------------------------

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

  // Normalize values between 0 and 1.
  /* Shrink the values of FACE_M into the range 0..1, as these are
     the only values accepted a an RGB floating-point component in the
     OFF file format.  */
  ima_t output(max_curv.domain());
  mln::data::fill(output, mln::literal::zero);
  std::pair<float, float> min_max(acc);
  // FIXME: Taken from mln/data/stretch.hh (this should be factored).
  float min = min_max.first;
  float max = min_max.second;
  std::cout << min << std::endl;
  std::cout << max << std::endl;
  // Don't normalize actually if the curvature is constant (i.e.,
  // if min == max).
  if (min != max)
    {
      float m = 0.0f;
      float M = 1.0f;
      float a = (M - m) / (max - min);
      float b = (m * max - M * min) / (max - min);
      mln::fun::v2v::linear<float, float, float> f(a, b);
      output = mln::data::transform(max_curv, f);
    }

  // Output.
  mln::io::off::save(output, output_filename);
}
