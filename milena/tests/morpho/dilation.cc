// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/win/rectangle2d.hh>
#include <mln/win/octagon2d.hh>
#include <mln/win/diag2d.hh>
#include <mln/win/backdiag2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/data/fill.hh>
#include <mln/morpho/dilation.hh>

#include <mln/pw/value.hh>
#include <mln/pw/cst.hh>
#include <mln/fun/ops.hh>

#include <mln/core/alias/neighb2d.hh>

  /* FIXME: Re-enable these tests for Olena 1.1, when associated
     neighborhoods (i.e., the neighb::image morpher) are supported and
     shipped.  */
#if 0
#  include <mln/neighb/image.hh>
#endif

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  // FIXME: Do we really need such a high value?
  border::thickness = 66;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
  win::rectangle2d rec(5, 3);

  { 
    image2d<int_u8> out = morpho::dilation(lena, rec);
    io::pgm::save(out, "out1.pgm");
  }

  {
    win::octagon2d oct(7);
    image2d<int_u8> out = morpho::dilation(lena, oct);
    io::pgm::save(out, "out2.pgm");
  }

  {
    image2d<bool> bin(lena.domain());
    data::fill(bin, pw::value(lena) > pw::cst(127u));
    image2d<bool> out = morpho::dilation(bin, rec);

    image2d<int_u8> test(lena.domain());
    image2d<int_u8>::fwd_piter p(lena.domain());
    for_all(p)
      test(p) = out(p) ? 255 : 0;
    io::pgm::save(test, "out3.pgm");
  }

  /* FIXME: Re-enable these tests for Olena 1.1, when associated
     neighborhoods (i.e., the neighb::image morpher) are supported and
     shipped.  */
#if 0
  {
    image2d<int_u8> out = morpho::dilation(lena + c4());
    io::pgm::save(out, "out4.pgm");
  }

  {
    image2d<int_u8> out = morpho::dilation(lena + c8());
    io::pgm::save(out, "out5.pgm");
  }
#endif
}
