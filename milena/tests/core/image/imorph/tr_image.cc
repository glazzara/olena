// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/core/image/imorph/tr_image.cc
///
/// \brief Tests on mln::tr_image.
///
/// \todo Write an effective test!

#include <iostream>

#include <mln/fun/x2x/rotation.hh>
#include <mln/core/image/image3d.hh>
#include <mln/value/int_u8.hh>
#include <mln/core/image/imorph/tr_image.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  {
    image2d<int_u8> in(3, 3);

    debug::iota(in);
    debug::println(in);

    algebra::vec<2, float> v;
    v[0] = 0;
    v[1] = 1;
    fun::x2x::rotation<2, float> rot1(1.67, v);

    tr_image<mln_domain_(image2d<int_u8>),
      image2d<int_u8>,
      fun::x2x::rotation<2, float> >
	inter(in.domain(), in, rot1);

    debug::println(inter);
  }

  {
    image3d<int_u8> in(3, 3, 3);

    debug::iota(in);
    debug::println(in);

    algebra::vec<3, float> v;
    v[0] = 0;
    v[1] = 0;
    v[2] = 1;
    fun::x2x::rotation<3, float> rot1(1.67, v);

    tr_image<mln_domain_(image3d<int_u8>),
      image3d<int_u8>,
      fun::x2x::rotation<3, float> >
	inter(in.domain(), in, rot1);

    debug::println(inter);
  }
}
