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

#include <cstdlib>

#include <string>

#include <mln/core/image/complex_image.hh>
#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/complex_windows.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>

#include <mln/data/fill.hh>
#include <mln/math/diff_abs.hh>
#include <mln/literal/zero.hh>

#include <mln/io/off/load.hh>
#include <mln/io/off/save.hh>
#include <mln/labeling/colorize.hh>

#include "chain.hh"


// FIXME: Should be rewritten using a diff_abs-based accumulator
// taking values from triangles (and checking that exactly two values
// where taken, of course).
template <unsigned D, typename G, typename V>
inline
mln::complex_image<D, G, V>
gradient_on_edges(const mln::complex_image<D, G, V>& input)
{
  typedef mln::complex_image<D, G, V> ima_t;
  ima_t output (input.domain());
  mln::data::fill(output, mln::literal::zero);

  // Values on edges.
  mln::p_n_faces_fwd_piter<D, G> e(input.domain(), 1);
  // Edge-to-triangle adjacency.
  typedef mln::complex_higher_neighborhood<D, G> e2t_t;
  e2t_t e2t;
  mln_niter(e2t_t) t(e2t, e);
  // Iterate on edges (1-faces).
  for_all(e)
  {
    t.start();
    // An edge should be adjacent to at least one triangle.
    if (!t.is_valid())
      abort();
    V v1 = input(t);
    t.next();
    // If E is adjacent to two triangles, compute the absolute
    // difference between their values.
    if (t.is_valid())
      {
	V v2 = input(t);
	output(e) = mln::math::diff_abs(v1, v2);
	// There should be no more adjacent triangles.
	t.next();
	mln_assertion(!t.is_valid());
      }
  }
  return output;
}


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

  using namespace mln;

  typedef float val;
  typedef value::label_16 label;
  // Input and output types.
  typedef mln::float_2complex_image3df input;
  typedef mln_ch_value_(input, label) output;
  static const unsigned dim = input::dim;
  typedef mln_geom_(input) geom;
  complex_lower_dim_connected_n_face_neighborhood<dim, geom> nbh;
  complex_lower_dim_connected_n_face_window_p<dim, geom> win;
  label nbasins;

  // Load, process, save.
  /* FIXME: The domain of IMA should be limited to 2-faces.  Alas, we
     do not have (yet) an mln::pcomplex_subset site set type.  Anyway,
     this should not alter the results, only slow the computation
     down.  */
  input ima;
  io::off::load(ima, input_filename);

  // Gradient on edges.
  input g = gradient_on_edges(ima);
  output s = chain(ima, nbh, lambda, nbasins);
  io::off::save(labeling::colorize(value::rgb8(), s, nbasins),
		output_filename);
}
