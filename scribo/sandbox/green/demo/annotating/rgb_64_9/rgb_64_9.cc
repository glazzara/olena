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
/// \brief Implement the Millet RGB-64 descriptor [millet.phd.2008.pdf]
///
/// This is an image descriptor. It works by making 2 bits
/// quantification on each channel. It result a processing in RGB-64
/// space. Then we build the color histogram.

#include <iostream>
#include <sstream>

#include <mln/accu/stat/histo3d_rgb.hh>

#include <mln/core/alias/box2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/routine/initialize.hh>

#include <mln/data/compute.hh>
#include <mln/data/paste.hh>
#include <mln/data/transform.hh>

#include <mln/debug/println.hh>

#include <mln/fun/v2v/rgb8_to_rgbn.hh>

#include <mln/img_path.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>

/// \brief Get the sub image name.
///
/// \return the name of the ith/jth sub image of the array.
const char *get_name(const char *base, const unsigned i, const unsigned j)
{
  std::ostringstream name;

  name << base;
  name << i;
  name << "_";
  name << j;
  name << ".ppm";

  return name.str().c_str();
}

/// \brief Same code as rgb-64 but divide the wall image in nine.
///
/// Adding a splitting phase for image rgb-64 code reference.
int main()
{
  typedef mln::fun::v2v::rgb8_to_rgbn<2>     t_rgb8_to_rgb2;
  typedef mln::value::rgb8                   t_rgb8;
  typedef mln::value::rgb<2>                 t_rgb2;
  typedef mln::image2d<t_rgb8>               t_image2d_rgb8;
  typedef mln::image2d<t_rgb2>               t_image2d_rgb2;
  typedef mln::image3d<unsigned>             t_histo3d;
  typedef mln::accu::meta::stat::histo3d_rgb t_histo3d_fun;

  t_image2d_rgb8                             input_rgb8;
  t_image2d_rgb2                             input_rgb2;
  t_histo3d                                  histo;

  // IMAGE LOADING PHASE
  std::cout << "Image loading phase ..." << std::endl;
  mln::io::ppm::load(input_rgb8, ICDAR_50P_PPM_IMG_PATH"/mp00082c_50p.ppm");

  input_rgb2 = mln::data::transform(input_rgb8, t_rgb8_to_rgb2());

  // IMAGE SPLITTING PHASE
  mln::box2d     domain = input_rgb2.domain();
  mln::point2d   pmin   = domain.pmin();
  mln::point2d   pmax   = domain.pmax();

  unsigned       sz_row = (pmax.row() - pmin.row())/ 3;
  unsigned       sz_col = (pmax.col() - pmin.col())/ 3;

  std::cout << domain << std::endl;

  // Divide the domain in nine sub-domains.
  for (unsigned i = 0; i < 3; ++i)
    for (unsigned j = 0; j < 3; ++j)
    {
      mln::point2d min(pmin.row()+sz_row*i,pmin.col()+sz_col*j);
      mln::point2d max(pmin.row()+sz_row*(i+1),pmin.col()+sz_col*(j+1));
      mln::box2d   dom(min,max);

      std::cout << dom << std::endl;

      // Save it
      t_image2d_rgb2 input_1o9_rgb2(dom);

      mln::data::paste(input_rgb2 | dom, input_1o9_rgb2);
      mln::io::ppm::save(input_1o9_rgb2, get_name("output",i,j));

      histo      = mln::data::compute(t_histo3d_fun(), input_1o9_rgb2);

      // PRINTING PHASE
      mln::debug::println(histo);
    }

}
