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

/// \file tests/fun/x2x/rotation.cc
///
/// Tests on mln::fun::x2x::rotation.
///

#include <iostream>
#include <mln/fun/x2x/rotation.hh>
#include <mln/core/alias/vec2d.hh>
#include <mln/core/alias/vec3d.hh>
#include <mln/make/vec.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;

  {
    algebra::vec<2,float> axis;
    axis[0] = 0;
    axis[1] = 1;
    fun::x2x::rotation<2,float> rot(0.1f, axis);

    vec2d_f v = make::vec(4, 4);
    vec2d_f res = rot(v);
    mln_assertion(rot.inv()(res) == v);
  }

  {
    algebra::vec<3,float> axis;
    axis[0] = 1;
    axis[1] = 0;
    axis[2] = 0;
    fun::x2x::rotation<3,float> rot(0.1f, axis);

    vec3d_f v = make::vec(4, 1, 2);
    vec3d_f res = rot(v);
    mln_assertion(rot.inv()(res) == v);
  }

}
