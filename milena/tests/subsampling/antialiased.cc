// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>

#include <mln/data/compare.hh>

#include <mln/subsampling/antialiased.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using namespace mln::value;

  int_u8 refgl[3][3] = { { 124, 153, 116 },
			 { 105, 116, 158 },
			 {  93, 136, 106 } };

  rgb8 refrgb[3][3] = { { rgb8(197, 93, 92), rgb8(206, 131, 128), rgb8(171, 94, 101) },
			{ rgb8(164, 78, 96), rgb8(174, 91, 105), rgb8(204, 143, 128) },
			{ rgb8(135, 70, 96), rgb8(201, 112, 109), rgb8(159, 80, 90) } };

  image2d<int_u8> refgl_ima = make::image(refgl);
  image2d<rgb8> refrgb_ima = make::image(refrgb);

  box2d bref(3, 3);

  {
    image2d<value::int_u8> input;
    io::pgm::load(input, MLN_IMG_DIR "/tiny.pgm");

    image2d<value::int_u8> tmp = subsampling::antialiased(input, 6);

    mln_assertion(tmp == refgl_ima);
  }


  {
    image2d<value::rgb8> input;
    io::ppm::load(input, MLN_IMG_DIR "/tiny.ppm");

    image2d<value::rgb8> tmp = subsampling::antialiased(input, 6);

    mln_assertion(tmp == refrgb_ima);
  }
}
