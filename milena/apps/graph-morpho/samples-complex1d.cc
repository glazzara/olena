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

/// \file
/// \brief Morphological operators in graph space implemented on
/// 1-complex images.
///
/// \todo There should be a second version of this program, where the
/// graph structure is implemented with an actual mln::util::graph.

#include <mln/io/pbm/load.hh>

#include "apps/graph-morpho/morpho.hh"

#include "apps/graph-morpho/io.hh"
#include "apps/graph-morpho/debug.hh"

#include "apps/data.hh"


int main()
{
  using namespace mln;

  /* Binary graph-based image with vertices aligned on a discrete 2D grid.

     Of course, it would have been better, simpler and faster to use a
     cubical 1-complex here, but they are not yet available (as of
     2009-09-10).  */
  typedef mln::bin_1complex_image2d ima_t;

  // ------------------------ //
  // Dilations and erosions.  //
  // ------------------------ //

  /* Create an image corresponding to the graph X of the ISMM 2009
     paper from Jean Cousty et al.  */
  image2d<bool> x_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/x.pbm");
  ima_t x = make_regular_complex1d_image(x_pbm);
  ::debug::println_graph("x:", x);
  
  ::debug::println_graph("dilation_e2v(x):", dilation_e2v(x));
  ::debug::println_graph("erosion_v2e(x):",  erosion_v2e(x));

  ::debug::println_graph("erosion_e2v(x):",  erosion_e2v(x));
  ::debug::println_graph("dilation_v2e(x):", dilation_v2e(x));

  ::debug::println_graph("dilation_graph(x):", dilation_graph(x));
  ::debug::println_graph("erosion_graph(x):",  erosion_graph(x));

  ::debug::println_graph("alpha3(x):", alpha3(x));
  ::debug::println_graph("beta3(x):",  beta3(x));

  // --------- //
  // Filters.  //
  // --------- //

  // Create an image corresponding to the graph Y.
  image2d<bool> y_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/y.pbm");
  ima_t y = make_regular_complex1d_image(y_pbm);
  ::debug::println_graph("y:", y);

  ::debug::println_graph("opening_graph(y):",      opening_graph(y));
  ::debug::println_graph("half_opening_graph(y):", half_opening_graph(y));
  ::debug::println_graph("beta3(alpha3(y)):",      beta3(alpha3(y)));

  // Create an image corresponding to the graph Z.
  image2d<bool> z_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/z.pbm");
  ima_t z = make_regular_complex1d_image(z_pbm);
  ::debug::println_graph("z:", z);

  ::debug::println_graph("closing_graph(z):",      closing_graph(z));
  ::debug::println_graph("half_closing_graph(z):", half_closing_graph(z));
  ::debug::println_graph("alpha3(beta3(z)):",      alpha3(beta3(z)));
}
