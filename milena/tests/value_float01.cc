// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#include <mln/core/image2d_b.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

#include <mln/value/float01_8.hh>
#include <mln/value/float01_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/level/transform.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/core/win/rectangle2d.hh>
#include <mln/level/median.hh>
#include <mln/level/compare.hh>


using namespace mln;
using namespace mln::value;
using  mln::value::int_u8;

float fi(int) { return 0.5; }
int ii(int) { return 1; }

float fd(double) { return 0.5; }
int id(double) { return 1; }


struct tofloat01 : mln::Function_v2v<tofloat01>
{

  typedef float01_<16> result;
  result operator()(int_u8 v) const
  {
    result ret(double(v) / (mln_max(int_u8)));
    //    std::cout << v << "-> "  << ret << std::endl;
    return ret;
  }
};

struct to8bits : mln::Function_v2v<to8bits>
{

  typedef int_u8 result;
  result operator()(float01_<16> v) const
  {
    result ret = int(v.value() * 255);
    //std::cout << v << "-> " << ret << std::endl;
    return ret;
  }
};

int main()
{
  win::rectangle2d rect(51, 51);
  border::thickness = 52;

  float01_8  a(0.5);
  float01_16 b(0.5);

  assert(approx_equal(b,a));

  std::cout << b << std::endl;
  b = b + 0.2;
  std::cout << b << std::endl;
  b = b - 0.2;
  std::cout << b << std::endl;
  b = b * 1.5;
  std::cout << b << std::endl;
  b = b / 4.6;
  std::cout << b << std::endl;

  b = b / 3;
  std::cout << b << std::endl;
  b = b * 1;
  std::cout << b << std::endl;

  a = fi(a);
  a = ii(a);
  a = fd(a);
  a = id(a);

  b = a;
  a = b;
  b = 0.34;
  std::cout << b << std::endl;
  b = 0;
  std::cout << b << std::endl;
  b = 1;
  std::cout << b << std::endl;

  {
    std::cout << "convert" << std::endl;
    image2d_b<int_u8>
      lena = io::pgm::load<int_u8>("../img/lena.pgm"),
      ref(lena.domain());

    image2d_b<float01_16> out(lena.domain());
    image2d_b<float01_16> tmp(lena.domain());

    level::transform(lena, tofloat01(), tmp);

    level::median(tmp, rect, out);
    level::median(lena, rect, ref);


    level::transform(out, to8bits(), lena);

    io::pgm::save(lena, "out.pgm");
    io::pgm::save(ref, "ref.pgm");
    mln_assertion(lena == ref);
    //debug::println(out);
  }


}
