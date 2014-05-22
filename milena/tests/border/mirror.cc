// Copyright (C) 2007, 2008, 2009, 2011, 2014 EPITA Research and Development
// Laboratory (LRDE).
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

// Exercise mln::border::mirror.

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/extended.hh>
#include <mln/debug/iota.hh>
#include <mln/border/mirror.hh>
#include <mln/opt/element.hh>
#include <mln/data/paste_without_localization.hh>
#include <mln/data/compare.hh>

int
main()
{
  using namespace mln;

  // image1d.
  {
    // A 2-pixel 1D image with a 3-pixel border.
    image1d<unsigned> input(2, 3);
    debug::iota(input);
    border::mirror(input);

    // Data are framed; other values form the border.
    unsigned ref_data[8] =
    //              ,-------------.
      { 2, 2, 1, /* | */ 1, 2, /* | */ 2, 1, 1 };
    //              `-------------'
    image1d<unsigned> ref = make::image(ref_data);

    image1d<unsigned> output(8, 0);
    data::paste_without_localization(extended_to(input, input.vbbox()),
                                     output);
    mln_assertion(output == ref);
  }

  // image2d.
  {
    // A 2x3-pixel 2D image with a 2-pixel border.
    image2d<unsigned> input(2, 3, 2);
    debug::iota(input, 0);
    border::mirror(input);

    // Data are framed; other values form the border.
    unsigned ref_data[6][7] = {
      { 1, 1,         4, 5, 6,         3, 3 },
      { 1, 1,         1, 2, 3,         3, 3 },
      //         ,----------------.
      { 2, 1, /* | */ 1, 2, 3, /* | */ 3, 2 },
      { 5, 4, /* | */ 4, 5, 6, /* | */ 6, 5 },
      //         `----------------'
      { 4, 4,         4, 5, 6,         6, 6 },
      { 4, 4,         1, 2, 3,         6, 6 }
    };
    image2d<unsigned> ref = make::image(ref_data);

    image2d<unsigned> output(6, 7, 0);
    data::paste_without_localization(extended_to(input, input.vbbox()),
                                     output);
    mln_assertion(output == ref);
  }
}
