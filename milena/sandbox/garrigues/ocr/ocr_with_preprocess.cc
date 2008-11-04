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
#include "skeleton.hh"

#include <mln/linear/gaussian.hh>

#include <mln/trace/all.hh>

#include <mln/fun/p2v/ternary.hh>
#include <mln/pw/image.hh>

#include <mln/debug/println.hh>
#include <mln/labeling/regional_maxima.hh>
#include <mln/morpho/dilation.hh>
#include <mln/win/octagon2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/logical/not.hh>

#include "tesseract_wrap.hh"
#include <mln/subsampling/subsampling.hh>

// _COMPILATION_
// g++ -DNDEBUG -O3 -I../../.. ocr.cc -L/usr/lib -ltesseract_full -lpthread

#if 1
# define OCR_TEST(Var)						\
  {								\
    image2d<int_u8> tmp = clone(cast_image<int_u8>(Var));	\
    float score = 0.f;						\
    char* s = tesseract("fra", tmp, &score);			\
    std::cerr << #Var << ": " << score << std::endl << s;	\
    delete[] s;							\
  }
#else
# define OCR_TEST(Var)
#endif


int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  image2d<bool> input;

  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " in.pbm out.pbm" << std::endl;
    return 1;
  }

  mln::border::thickness = 0;

  io::pbm::load(input, argv[1]);
  OCR_TEST(input);

  // Resize
  image2d<int_u8> enlarged = enlarge(logical::not_(input), 2);
  io::pgm::save(enlarged, std::string(argv[2]) + "_1_enlarge.pgm");
  OCR_TEST(enlarged);

  // Blur.
  image2d<int_u8> blur = linear::gaussian(enlarged, 2);

  io::pgm::save(blur, std::string(argv[2]) + "_2_gaussian.pgm");
  OCR_TEST(blur);

  // Threshold
  image2d<bool> binary;
  {
    initialize(binary, blur);
    mln_piter_(image2d<int_u8>) p(blur.domain());
    for_all(p)
      binary(p) = blur(p) > 100;

    io::pbm::save(binary, std::string(argv[2]) + "_3_threshold.pbm");
    OCR_TEST(binary);
  }

  // Skeleton
  image2d<bool> skel = skeleton(binary, 4);
  io::pbm::save(skel, std::string(argv[2]) + "_4_skeleton.pbm");
  OCR_TEST(skel);

  // Dilation
  image2d<bool> dilate;
  win::octagon2d oct(5);
  { // FIXME?
#if 1
    image2d<int_u8> tmp;
    initialize(tmp, skel);
    initialize(dilate, skel);
    mln_piter_(image2d<int_u8>) p(tmp.domain());
    for_all(p)
      tmp(p) = skel(p);
    tmp = morpho::dilation(tmp, oct);
    for_all(p)
      dilate(p) = tmp(p);
#else
    // Should be using this but it doesn't work :(
    dilate = morpho::dilation(skel, oct);
#endif
  }

  io::pbm::save(dilate, std::string(argv[2]) + "_5_dilation.pbm");
  OCR_TEST(dilate);

  // Subsampling
  image2d<bool> subsampled = subsampling::subsampling(dilate, dpoint2d(1,1), 2);
  io::pbm::save(subsampled, std::string(argv[2]) + "_6_subsampling.pbm");
  OCR_TEST(subsampled);

  io::pbm::save(subsampled, argv[2]);

  {
    float score = 0;
    char* s = tesseract("fra", subsampled, &score);
    std::cerr << "Tesseract result: (score " << score << ")" << std::endl;
    std::cout << s;
    delete[] s;
  }

}
