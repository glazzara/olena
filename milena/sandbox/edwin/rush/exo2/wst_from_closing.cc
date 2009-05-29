// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>
#include <mln/data/convert.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>

#include "color_internal_gradient.hh"
#include "color_labeling_mean.hh"
#include <mln/morpho/closing/height.hh>
#include <mln/morpho/watershed/all.hh>
#include <mln/morpho/elementary/dilation.hh>


#include <iostream>
#include <string>


void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " input.ppm height [-o1 | -o2 | -o3]"
	    << std::endl
	    << "-o1\tSuperpose WS on input image (out: superpose.ppm)" << std::endl
	    << "-o2\tCount number of basins" << std::endl
  	    << "-o3\tReplace basin by its mean color on the input (out: mean.ppm)"
	    << std::endl;
  abort();
}



int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;
  using value::rgb8;

  if (argc < 3)
    usage(argv);

  int output_ = 0;
  int lambda_h = std::atoi(argv[2]);

  if (argc == 4)
    if (std::string(argv[3]) == "-o1")
      output_ = 1;
    else if (std::string(argv[3]) == "-o2")
      output_ = 2;
    else if (std::string(argv[3]) == "-o3")
      output_ = 3;
    else
      usage(argv);



  // Load color image.
  typedef image2d<rgb8> I_;
  I_ input_;
  io::ppm::load(input_, argv[1]);

  // Gradient.
  typedef image2d<int_u8> I;
  I input = color_internal_gradient(input_);

  // Closing
  std::cout << lambda_h << std::endl;
  input = morpho::closing::height(input, c4(), lambda_h);

  // Watershed
  typedef image2d<int_u16> WST;
  int_u16 n_basins = 0;
  WST wst = morpho::watershed::flooding(input, c4(), n_basins);

  if (!output_ || output_ == 1)
    {
      I_ out = morpho::watershed::superpose(input_, wst);
      io::ppm::save(out, "superpose.pgm");
    }

  if (!output_ || output_ == 2)
    std::cout << "Number of basins: " << n_basins << std::endl;

  if (!output_ || output_ == 3)
    {
      typedef image2d<value::label_16> L;
      L lab = data::convert(value::label_16(),
			     morpho::elementary::dilation(wst, c8()));
      I_ out = color_labeling_mean(input_, lab, n_basins);
      io::ppm::save(out, "mean.pgm");
    }

}
