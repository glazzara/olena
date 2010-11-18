// Copyright (C) 2007, 2008, 2009, 2010 EPITA LRDE
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
///
/// \brief This tool aimed at filtering the previous 3d RGB histogram.
///
/// The current filtering is managed with morphological volume attribute.
///
/// \fixme I don't know if the volume attribute do well it's
/// job. Sometimes, there is something strange happened. I know it's
/// related to that part of processing but i can't tell where it is wrong.

#include <iostream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/macros.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/data/compute.hh>

#include <mln/display/display_histo.hh>

#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/literal/colors.hh>

#include <mln/morpho/opening/volume.hh>

#include <mln/value/rgb.hh>
#include <mln/value/int_u8.hh>

/// \brief The histogram image processing chain.
///
/// \param[in] histo the name of the input histogram to filter.
/// \param[in] min_vol the minimum volume threshold to leave in the image.
/// \param[out] opened the name of the filtered histogram.
/// \param[out] proj1 the red/green projection of the histogram.
/// \param[out] proj2 the red/green projection with maxima plot on.
///
/// This chain aimed at filtering 3d RGB histogram by volume
/// morphological attribute. The threshold indicates the minimum
/// volume information to leave, otherwhise information with less
/// volume attribute is discarded. Then, dumping and
/// projections are done.
template <unsigned n>
void mk_opening(const std::string& histo,   // in
		const unsigned     min_vol, // in
		const std::string& opened,  // out
		const std::string& proj1,   // out
		const std::string& proj2)   // out
{
  typedef mln::value::int_u8                          t_int_u8;
  typedef mln::value::rgb<n>                          t_rgbn;
  typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
  typedef mln::image2d<t_rgbn>                        t_image2d_rgbn;
  typedef mln::image3d<unsigned>                      t_histo3d;
  typedef mln::accu::meta::stat::histo3d_rgb          t_histo3d_fun;

  // START OF IMAGE PROCESSING CHAIN
  t_histo3d                                           h1_input; // histo input
  t_histo3d                                           h2_input; // histo input
  t_image2d_int_u8                                    p1_histo1;// histo proj1
  t_image2d_rgbn                                      p1_histo2;// histo proj2
  t_rgbn                                              red(mln::literal::red);

  mln::io::dump::load(h1_input, histo.c_str());
  h2_input = mln::morpho::opening::volume(h1_input, mln::c6(), min_vol);
  // END OF IMAGE PROCESSING CHAIN

  // BEGIN DUMPING
  p1_histo1 = mln::display::display_histo3d_unsigned(h2_input);
  p1_histo2 = mln::display::display3_histo3d_unsigned(h2_input, red);
  mln::io::dump::save(h2_input, opened.c_str());
  mln::io::pgm::save(p1_histo1, proj1.c_str());
  mln::io::ppm::save(p1_histo2, proj2.c_str());
  // END DUMPING
}


/// \brief The online documentation of the tool.
///
/// Print the documentation of the tool with the meaning of the
/// different parameters.
void usage()
{
  std::cout                                                       << std::endl;
  std::cout << "opening q histo.dump v opened.dump proj.pgm"
	    << " proj.ppm"                                        << std::endl;
  std::cout << std::endl;
  std::cout << "where :"                                          << std::endl;
  std::cout << "* [ in] q is the degree of quantification"
	    << " {2,3,4,5,6,7,8}"                                 << std::endl;
  std::cout << "* [ in] histo.dump is the quantified color"
	    << " histogram"                                       << std::endl;
  std::cout << "* [ in] v is the minimum size (in pixels) of"
	    << " each composant"                                  << std::endl;
  std::cout << "* [out] opened.dump is the filtered histogram"    << std::endl;
  std::cout << "* [out] proj.pgm is the r/g projection of the"
	    << " histogram (summing along the blue axe)"          << std::endl;
  std::cout << "* [out] proj.ppm is the r/g projection of the"
	    << " histogram with maxima plot on"                   << std::endl;
  std::cout << std::endl;
}


/// \brief The main entry of the software.
///
/// \param[in] q the quantification previously used.
/// \param[in] histo the name of the 3d RGB histogram to filter.
/// \param[in] min_vol the threshold of the morphological filtering.
/// \param[out] opened.dump the name of the resulting 3d histogram.
/// \param[out] proj.pgm the red/green projection of the histogram.
/// \param[out] proj2.ppm the red/green projection with maxima plot on.
///
/// This is the front hand for the image processing routine. The goal of this
/// chain is to filter 3d histogram for further analysis and to give a way to
/// see it.
int main(int argc, char* args[])
{
  if (7 == argc)
  {
    const char        q       = args[1][0];    // in
    const std::string histo(args[2]);          // in
    const unsigned    min_vol = atoi(args[3]); // in
    const std::string opened(args[4]);         // out
    const std::string proj1(args[5]);          // out
    const std::string proj2(args[6]);          // out

    switch(q)
    {
      case '2': mk_opening<2>(histo, min_vol, opened, proj1, proj2); break;
      case '3': mk_opening<3>(histo, min_vol, opened, proj1, proj2); break;
      case '4': mk_opening<4>(histo, min_vol, opened, proj1, proj2); break;
      case '5': mk_opening<5>(histo, min_vol, opened, proj1, proj2); break;
      case '6': mk_opening<6>(histo, min_vol, opened, proj1, proj2); break;
      case '7': mk_opening<7>(histo, min_vol, opened, proj1, proj2); break;
      case '8': mk_opening<8>(histo, min_vol, opened, proj1, proj2); break;
      default:  usage(); break;
    }
  }
  else
    usage();

  return 0;
}
