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

  /* Set the values so that X corresponds to the graph X of the ISMM
     2009 paper from Jean Cousty et al.  */
  image2d<bool> x_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/x.pbm");
  ima_t x = make_regular_complex1d_image(x_pbm);
  box2d x_box(x_pbm.nrows() / 2 + 1, x_pbm.ncols() / 2 + 1);
  std::cout << "x:" << std::endl;
  println(x, x_box);
  
  ima_t dil_e2v_ima = dilation_e2v(x);
  std::cout << "dil_e2v_ima:" << std::endl;
  println(dil_e2v_ima, x_box);

  ima_t ero_v2e_ima = erosion_v2e(x);
  std::cout << "ero_v2e_ima:" << std::endl;
  println(ero_v2e_ima, x_box);


  ima_t ero_e2v_ima = erosion_e2v(x);
  std::cout << "ero_e2v_ima:" << std::endl;
  println(ero_e2v_ima, x_box);

  ima_t dil_v2e_ima = dilation_v2e(x);
  std::cout << "dil_v2e_ima:" << std::endl;
  println(dil_v2e_ima, x_box);


  ima_t dil_ima = dilation_graph(x);
  std::cout << "dil_ima:" << std::endl;
  println(dil_ima, x_box);

  ima_t ero_ima = erosion_graph(x);
  std::cout << "ero_ima:" << std::endl;
  println(ero_ima, x_box);


  ima_t alpha3_ima = alpha3(x);
  std::cout << "alpha3_ima:" << std::endl;
  println(alpha3_ima, x_box);

  ima_t beta3_ima = beta3(x);
  std::cout << "beta3_ima:" << std::endl;
  println(beta3_ima, x_box);

  // --------- //
  // Filters.  //
  // --------- //

  image2d<bool> y_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/y.pbm");
  ima_t y = make_regular_complex1d_image(y_pbm);
  box2d y_box(y_pbm.nrows() / 2 + 1, y_pbm.ncols() / 2 + 1);
  std::cout << "y:" << std::endl;
  println(y, y_box);

  ima_t ope_ima = opening_graph(y);
  std::cout << "ope_ima:" << std::endl;
  println(ope_ima, y_box);

  ima_t half_ope_ima = half_opening_graph(y);
  std::cout << "half_ope_ima:" << std::endl;
  println(half_ope_ima, y_box);

  ima_t beta3_o_alpha3_ima = beta3(alpha3(y));
  std::cout << "beta3_o_alpha3_ima:" << std::endl;
  println(beta3_o_alpha3_ima, y_box);


  image2d<bool> z_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/z.pbm");
  ima_t z = make_regular_complex1d_image(z_pbm);
  box2d z_box(z_pbm.nrows() / 2 + 1, z_pbm.ncols() / 2 + 1);
  std::cout << "z:" << std::endl;
  println(z, z_box);

  ima_t clo_ima = closing_graph(z);
  std::cout << "clo_ima:" << std::endl;
  println(clo_ima, z_box);

  ima_t half_clo_ima = half_closing_graph(z);
  std::cout << "half_clo_ima:" << std::endl;
  println(half_clo_ima, z_box);

  ima_t alpha3_o_beta3_ima = alpha3(beta3(z));
  std::cout << "alpha3_o_beta3_ima:" << std::endl;
  println(alpha3_o_beta3_ima, z_box);
}
