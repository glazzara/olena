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
#include "apps/data.hh"


int main()
{
  using namespace mln;

  // A graph is considered as a 1-complex here.
  const unsigned dim = 1;
  typedef topo::complex<dim> complex_t;
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
  box2d x_box(x_pbm.nrows() / 2 + 1, x_pbm.ncols() / 2 + 1);
  println("x:", x, x_box);
  
  println("dilation_e2v(x):", dilation_e2v(x), x_box);
  println("erosion_v2e(x):",  erosion_v2e(x),  x_box);

  println("erosion_e2v(x):",  erosion_e2v(x),  x_box);
  println("dilation_v2e(x):", dilation_v2e(x), x_box);

  println("dilation_graph(x):", dilation_graph(x), x_box);
  println("erosion_graph(x):",  erosion_graph(x),  x_box);

  println("alpha3(x):", alpha3(x), x_box);
  println("beta3(x):",  beta3(x),  x_box);

  // --------- //
  // Filters.  //
  // --------- //

  // Create an image corresponding to the graph Y.
  image2d<bool> y_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/y.pbm");
  ima_t y = make_regular_complex1d_image(y_pbm);
  box2d y_box(y_pbm.nrows() / 2 + 1, y_pbm.ncols() / 2 + 1);
  println("y:", y, y_box);

  println("opening_graph(y):",      opening_graph(y),      y_box);
  println("half_opening_graph(y):", half_opening_graph(y), y_box);
  println("beta3(alpha3(y)):",      beta3(alpha3(y)),      y_box);

  // Create an image corresponding to the graph Z.
  image2d<bool> z_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/z.pbm");
  ima_t z = make_regular_complex1d_image(z_pbm);
  box2d z_box(z_pbm.nrows() / 2 + 1, z_pbm.ncols() / 2 + 1);
  println("z:", z, z_box);

  println("closing_graph(z):",      closing_graph(z),      z_box);
  println("half_closing_graph(z):", half_closing_graph(z), z_box);
  println("alpha3(beta3(z)):",      alpha3(beta3(z)),      z_box);
}
