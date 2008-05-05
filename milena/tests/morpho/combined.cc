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

/// \file  tests/morpho/combined.cc
/// \brief Test on several mln::morpho entities.

#include <cmath>

#include <mln/core/image2d.hh>
#include <mln/level/fill.hh>
#include <mln/debug/println.hh>

#include <mln/core/window2d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/core/neighb2d.hh>

#include <mln/morpho/erosion.hh>
#include <mln/morpho/Rd.hh>

#include <mln/value/int_u8.hh>


// static value::int_u8 cos_sin(const mln::point2d& p)
// {
//   return (value::int_u8)(255 * std::cos(float(p.row())) * std::sin(float(p.col())));
// }


int main()
{
  using namespace mln;

  const unsigned size = 1000;
  image2d<value::int_u8> f(size, size);
  morpho::Rd(f, f, c8());
}



//   box2d b = make::box2d(/* row = */ 1, 3,
// 		     /* col = */ 4, 6);
//   std::cout << b << std::endl;

//   bool w[] =
//     { 0, 1, 0,
//       0, 1, 0,
//       1, 0, 0 };
//   window2d win = make::window2d(w);
//   std::cout << win << std::endl;
//   std::cout << c8() << std::endl;

//     {
//   image2d<int> ima(b);
//   level::fill(ima, 51);
//   debug::println(ima);

//   std::cout << win << std::endl;

//   morpho::erosion(ima, win);

//   rectangle2d rec(1, 2);
//   std::cout << rec << std::endl;
//   }


//   {
//     image2d<int> ima(b);
//     level::fill(ima, cos_sin);
//     debug::println(ima);

//     std::cout << std::endl;

//     image2d<int> ima2 = morpho::erosion(ima, win);
//     debug::println(ima2);
//   }
