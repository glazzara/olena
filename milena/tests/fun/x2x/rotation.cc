// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/fun/x2v/rotation.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/core/image/interpolated.hh>
#include <mln/make/vec.hh>
#include <mln/fun/x2v/bilinear.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  algebra::vec<2,float> axis;
  axis[0] = 0;
  axis[1] = 1;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
  image2d<int_u8> out(lena.domain());

  interpolated<image2d<int_u8>, fun::x2v::bilinear> inter(lena);

  fun::x2x::rotation<2,float> rot1(0.1, axis);

  mln_piter_(image2d<int_u8>) p(out.domain());

  for_all(p)
    {
      algebra::vec<2,float> v = rot1.inv()(p.to_site().to_vec());
      if (inter.has(v))
	out(p) = inter(v);
      else
	out(p) = 255;
    }
  io::pgm::save(out, "out.pgm");

  fun::x2x::rotation<2,float> rot2(3.14116, axis);
  mln_assertion(fabs(rot2(make::vec(0.0, 1.0))[0] -
		     make::vec(0.0, -1.0)[0]) <= 0.125);
  mln_assertion(fabs(rot2(make::vec(0.0, 1.0))[1] -
		     make::vec(0.0, -1.0)[1]) <= 0.125);
}
