// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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

#include <mln/core/image/image2d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/data/transform.hh>

#include "tests/data.hh"


using namespace mln;

struct to16bits : mln::Function_v2v<to16bits>
{

  typedef value::int_u16 result;
  result operator()(value::int_u8 v) const
  {
    result ret(v * 256);
    return ret;
  }
};

struct to8bits : mln::Function_v2v<to8bits>
{

  typedef value::int_u8 result;
  result operator()(value::int_u16 v) const
  {
    result ret(v / 256);
    return ret;
  }
};


int main()
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;

  {
    win::rectangle2d rect(51, 51);
    border::thickness = 52;

    image2d<int_u8> lena = io::pgm::load<int_u8>(MLN_IMG_DIR "/lena.pgm");
    image2d<int_u16> out(lena.domain());

    out = data::transform(lena, to16bits());
    io::pgm::save(out, "pgm16-out16.pgm");

    image2d<int_u16> lena2 = io::pgm::load<int_u16>("pgm16-out16.pgm");

    image2d<int_u8> out2(lena.domain());
    out2 = data::transform(lena2, to8bits());
    io::pgm::save(out2, "pgm16-out8.pgm");
  }


  {
    /* FIXME: Is this dead code, or code not working?  Be more specific.  */
    // Abort
//     image2d< value::int_u<8> > a;
//     io::pgm::load(a, "pgm16-out16.pgm");

    image2d< value::int_u<16> > b;
    io::pgm::load(b, "pgm16-out16.pgm");
  }

}
