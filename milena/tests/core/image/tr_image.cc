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

/*! \file tests/core/image/tr_image.cc
 *
 * \brief Tests on mln::tr_image.
 */


#include <iostream>
#include <mln/fun/x2x/rotation.hh>
#include <mln/core/image/image3d.hh>
#include <mln/value/int_u8.hh>
#include <mln/core/image/tr_image.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

int main()
{
  using namespace mln;
  using value::int_u8;

  image3d<int_u8> in(3, 3, 3);
  image3d<int_u8> out(in.domain());

  debug::iota(in);
  debug::println(in);

  fun::x2x::rotation<3,float> rot1(1.67, 0);

  tr_image<fun::x2x::rotation<3,float>, image3d<int_u8> > inter(in, rot1);

  image3d<int_u8>::fwd_piter p(out.domain());
  
  for_all(p)
    {
      algebra::vec<3,int> vec = (image3d<int_u8>::point)p;
      if (inter.has(vec))
	out(p) = inter(vec);
      else
	out(p) = 255;
    }
  debug::println(out);
}
