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

// _COMPILATION_
// g++ -DNDEBUG -O3 -I../../.. ocr.cc -L/usr/lib -ltesseract_full -lpthread

#if 0
# define TEST(Var)						\
  {								\
    image2d<int_u8> tmp = clone(cast_image<int_u8>(Var));	\
    float score = 0.f;						\
    char* s = tesseract("fra", tmp, &score);			\
    std::cerr << #Var << ": " << score << std::endl << s;	\
    delete[] s;							\
  }
#else
# define TEST(Var)
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
  TEST(input);

  // Resize
  //std::cerr << "Enlarge the image" << std::endl;
  image2d<int_u8> enlarged = enlarge(logical::not_(input), 2);
  //image2d<bool> enlarged = geom::resize(logical::not_(input), 4);
  io::pgm::save(enlarged, "1_enlarge.pgm");
  TEST(enlarged);

  // Blur.
  //std::cerr << "Blur the enlarged image" << std::endl;
//   image2d<int_u8> blur = linear::gaussian(fun::p2v::ternary(pw::value(enlarged), pw::cst(int_u8(255)), pw::cst(int_u8(0))) | enlarged.domain(),
//					  4);
  image2d<int_u8> blur = linear::gaussian(clone(enlarged), 1);

  io::pgm::save(blur, "2_gaussian.pgm");
  TEST(blur);

  // Crest.
//   image2d<bool> c = crest(enlarged, blur, c4());
//   io::pbm::save(c, "3_crest.pbm");



  // Threshold
  image2d<bool> binary;
  {
    //std::cerr << "Threshold the blur image" << std::endl;

//     // Compute the histogram.
//     histo::data<int_u8> h = histo::compute(blur);
//     image1d<std::size_t> h_ima = convert::to_image(h);

//     // Blur the histogram.
//     h_ima = linear::gaussian(h_ima, 4);

//     // Get the maxima.
//     unsigned n;
//     image1d<std::size_t> maxs = regional_maxima(h_ima, c2(), n);
//     mln_piter()


    initialize(binary, blur);
    mln_piter_(image2d<int_u8>) p(blur.domain());
    for_all(p)
      binary(p) = blur(p) > 100;

    io::pbm::save(binary, "3_threshold.pbm");
    TEST(binary);
  }

  // Skeleton
  //std::cerr << "Compute the skeleton" << std::endl;
  image2d<bool> skel = skeleton(binary, 4);
  io::pbm::save(skel, "4_skeleton.pbm");
  TEST(skel);

  // Dilation
  //std::cerr << "Dilate the skeleton" << std::endl;
  win::octagon2d oct(7);
  for (unsigned i = 0; i < 1; i++)
    skel = morpho::dilation(skel, oct);

  io::pbm::save(skel, "5_dilation.pbm");
  TEST(skel);

  io::pbm::save(skel, argv[2]);

  //std::cerr << "Text recognition" << std::endl;
  //char* s = tesseract("fra", clone(logical::not_(skel)));
  {
    image2d<int_u8> tmp = clone(cast_image<int_u8>(skel));
    float score = 0;
    char* s = tesseract("fra", tmp, &score);
    std::cerr << "Tesseract result: (score " << score << ")" << std::endl;
    std::cout << s;
    delete[] s;
  }

}
