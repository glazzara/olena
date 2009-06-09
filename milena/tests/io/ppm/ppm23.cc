// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/*! \file
 *
 * \brief Test on mln::io::ppm::load and mln::io::ppm::save.
 */

#include <mln/core/image/image2d.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/value/rgb8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/data/compare.hh>

#include "tests/data.hh"


using namespace mln;

typedef value::rgb<23> rgb23;

struct to23bits : mln::Function_v2v<to23bits>
{

  typedef rgb23 result;
  result operator()(value::rgb8 v) const
  {
    result ret(v.red().to_enc() * 256,
	       v.green().to_enc() * 256,
	       v.blue().to_enc() * 256);
    return ret;
  }
};

struct to8bits : mln::Function_v2v<to8bits>
{

  typedef value::rgb8 result;
  result operator()(rgb23 v) const
  {
    result ret(v.red().to_enc() / 256,
	       v.green().to_enc() / 256,
	       v.blue().to_enc() / 256);
    return ret;
  }
};

int main()
{
  using namespace mln;
  using value::rgb8;

  typedef image2d<rgb8> I;


  // load a 8-bit image A
  image2d<rgb8> a = io::ppm::load<rgb8>(MLN_IMG_DIR "/lena.ppm");
  image2d<rgb23> b(a.domain());

  image2d<rgb8>::fwd_piter  p(b.domain());

  // save it as a 23-bit ppm image B
  to23bits f;
  for_all(p)
    b(p) = f(a(p));
  io::ppm::save(b, "out23.ppm");

  // reload B into C
  image2d<rgb23>
    c = io::ppm::load<rgb23>("out23.ppm");
  image2d<rgb8> d(a.domain());


  // save C as a 8-bit ppm image D
  to8bits g;
  for_all(p)
    d(p) = g(c(p));
  io::ppm::save(d, "out8.ppm");

  // D should equals A
  mln_assertion(d == a);

}
