// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

///
/// \brief Test fld IO on 2D images.
///

#include <mln/core/image/image2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/fld/load.hh>
#include <mln/io/fld/save.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/data/compare.hh>
#include "tests/data.hh"

#include <stdio.h>
#include <float.h>
#include <time.h>

int main()
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;

  // Test on int_u8.
  // Veclen = 1, data = byte
  {
    image2d<int_u8> ori, test;
    io::pgm::load(ori, MLN_IMG_DIR "/lena.pgm");

    io::fld::save(ori, "out2d.fld");
    io::fld::load(test, "out2d.fld");

    // Clean output.
    std::remove("out2d.fld");

    mln_assertion(ori == test);
  }

  // Test on RGB 16
  // Veclen = 3, data = short
  {
    image2d<int_u16> ori, test;
    io::ppm::load(ori, MLN_IMG_DIR "/lena_16.ppm");

    io::fld::save(ori, "out2d.fld");
    io::fld::load(test, "out2d.fld");

    // Clean output.
    std::remove("out2d.fld");

    mln_assertion(ori == test);
  }

  // Test on 32-bits data type
  // Veclen = 1, data = float
  {
    image2d<float> ori, test;
    box<point2d> domain(8, 9);

    srand(time(NULL));
    ori.init_(domain);
    {
      mln_piter_(image2d<float>) p(domain);
      for_all(p)
	ori(p) = random() / RAND_MAX;
    }

    io::fld::save(ori, "out2d.fld");
    io::fld::load(test, "out2d.fld");

    // Clean output.
    std::remove("out2d.fld");

    {
      mln_piter_(image2d<float>) p(domain);
      for_all(p)
	mln_assertion(fabs(ori(p) - test(p)) < FLT_EPSILON);
    }
  }

}
