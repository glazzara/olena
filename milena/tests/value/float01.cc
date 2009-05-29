// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file tests/value/float01.cc
///
/// Tests on mln::value::float01.

#include <iostream>

#include <mln/core/concept/function.hh>
#include <mln/value/float01_8.hh>
#include <mln/value/float01_16.hh>
#include <mln/value/int_u8.hh>


namespace mln
{

  using namespace value;

  float fi(int) { return 0.5f; }
  int ii(int) { return 1; }

  float fd(double) { return 0.5f; }
  int id(double) { return 1; }


  struct tofloat01 : Function_v2v<tofloat01>
  {

    typedef float01_<12> result;
    result operator()(int_u8 v) const
    {
      result ret = static_cast<float>(v) / static_cast<float>(mln_max(int_u8));
      //    std::cout << v << "-> "  << ret << std::endl;
      return ret;
    }
  };

  struct to8bits : Function_v2v<to8bits>
  {

    typedef int_u8 result;
    result operator()(float01_<12> v) const
    {
      result ret = static_cast<int>(v.value() * 255);
      // FIXME: Dead code.
      //std::cout << v << "-> " << ret << std::endl;
      return ret;
    }
  };

} // mln


int main()
{
  using namespace mln;
  
  value::float01_8  a(0.5);
  value::float01_16 b(0.5);

  mln_assertion(approx_equal(b,a));

  std::cout << b << std::endl;
  b = b + 0.2f;
  std::cout << b << std::endl;
  b = b - 0.2f;
  std::cout << b << std::endl;
  b = b * 1.5f;
  std::cout << b << std::endl;
  b = b / 4.6f;
  std::cout << b << std::endl;

  b = b / 3;
  std::cout << b << std::endl;
  b = b * 1;
  std::cout << b << std::endl;

  a = fi( static_cast<int>(a) );
  a = static_cast<float>( ii( static_cast<int>(a) ) );
  a = fd(a);
  a = static_cast<float>( id(a) );

  b = a;
  a = b;
  b = 0.34f;
  std::cout << b << std::endl;
  b = 0;
  std::cout << b << std::endl;
  b = 1;
  std::cout << b << std::endl;
}
