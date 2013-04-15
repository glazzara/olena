// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pdf/load.hh>
#include "tests/data.hh"

int main()
{
  using namespace mln;

  image2d<value::rgb8> page0, page1, page2, page3;

  io::ppm::load(page0, MLN_TESTS_IMG_DIR "/example-0.ppm");
  io::ppm::load(page1, MLN_TESTS_IMG_DIR "/example-1.ppm");
  io::ppm::load(page2, MLN_TESTS_IMG_DIR "/example-2.ppm");
  io::ppm::load(page3, MLN_TESTS_IMG_DIR "/example-3.ppm");

  // Loading full PDF.
  {
    util::array<image2d<value::rgb8> > arr;
    io::pdf::load(arr, MLN_TESTS_IMG_DIR "/example.pdf", 75);

    mln_assertion(arr.size() == 4);
    mln_assertion(arr[0] == page0);
    mln_assertion(arr[1] == page1);
    mln_assertion(arr[2] == page2);
    mln_assertion(arr[3] == page3);
  }

  // Loading a page range
  {
    util::array<image2d<value::rgb8> > arr;
    io::pdf::load(arr, MLN_TESTS_IMG_DIR "/example.pdf", 1, 2, 75);

    mln_assertion(arr.size() == 2);
    mln_assertion(arr[0] == page1);
    mln_assertion(arr[1] == page2);
  }

  // Loading specific pages.
  {
    util::array<int> pages;
    pages.append(1);
    pages.append(3);
    util::array<image2d<value::rgb8> > arr;
    io::pdf::load(arr, MLN_TESTS_IMG_DIR "/example.pdf", pages, 75);

    mln_assertion(arr.size() == 2);
    mln_assertion(arr[0] == page1);
    mln_assertion(arr[1] == page3);
  }

  // Loading a specific page.
  {
    image2d<value::rgb8> ima;
    io::pdf::load(ima, MLN_TESTS_IMG_DIR "/example.pdf", 3, 75);

    mln_assertion(ima.is_valid());
    mln_assertion(ima == page3);
  }

}
