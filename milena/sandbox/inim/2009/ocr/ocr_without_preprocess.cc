// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/core/image/cast_image.hh>

#include <mln/value/int_u8.hh>

#include "resize.hh"
#include "enlarge.hh"
//#include "skeleton.hh"
#include <mln/linear/gaussian.hh>

#include <mln/trace/all.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/core/alias/w_window2d_float.hh>

#include <mln/debug/println.hh>
#include <mln/geom/chamfer.hh>
#include <mln/make/win_chamfer.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/morpho/dilation.hh>

#include "tesseract_wrap.hh"

// _COMPILATION_
// g++ -DNDEBUG -O3 -I../../.. ocr.cc -L/usr/lib -ltesseract_full -lpthread


// Call tesseract
// lang: expected language

int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  image2d<bool> input;

  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " in.pbm" << std::endl;
    return 1;
  }

  mln::border::thickness = 0;

  io::pbm::load(input, argv[1]);

  {
    image2d<int_u8> tmp = duplicate(cast_image<int_u8>(input));
    float score = 0;
    char* s = tesseract("fra", tmp, &score);
    std::cerr << "Tesseract result: (score " << score << ")" << std::endl;
    std::cout << s;
    delete[] s;
  }
}
