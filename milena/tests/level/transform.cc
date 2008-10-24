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

/*! \file tests/level/transform.cc
 *
 * \brief Tests on mln::level::transform
 */

#include <cmath>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/pw/image.hh>
#include <mln/core/image/flat_image.hh>
#include <mln/core/image/image_if.hh>

#include <mln/level/fill.hh>
#include <mln/level/transform.hh>
#include <mln/level/paste.hh>

#include <mln/core/var.hh>

#include <mln/fun/p2b/chess.hh>
#include <mln/fun/p2v/iota.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

struct mysqrt : mln::Function_v2v<mysqrt>
{
  typedef unsigned short result;
  result operator()(unsigned short c) const
  {
    return result( std::sqrt(float(c)) );
  }
};



int main()
{
  using namespace mln;
  const unsigned size = 50;

  /// image 1d test
  {
    image1d<unsigned short> ima(size);
    image1d<unsigned short> out(size);

    debug::iota(ima);
    out = level::transform(ima, mysqrt());

    box_fwd_piter_<point1d> p(out.domain());
    for_all(p)
      mln_assertion((unsigned short)std::sqrt(ima(p)) == out(p));
  }


  /// image 2d test
  {
    image2d<unsigned short> ima(size, size);
    image2d<unsigned short> out(size, size);

     debug::iota(ima);
     out = level::transform(ima, mysqrt());

     box_fwd_piter_<point2d> p(out.domain());
     for_all(p)
       mln_assertion((unsigned short)std::sqrt(ima(p)) == out(p));
  }

  /// Another image2d test
  {
     image2d<unsigned short> ima(size, size);

     level::fill_with_value(ima, 51);
     level::transform(ima, mysqrt());

  }

  /// image 3d test
  {
    image3d<unsigned short> ima(size, size, size);
    image3d<unsigned short> out(size, size, size);

    debug::iota(ima);
    out = level::transform(ima, mysqrt());

    box_fwd_piter_<point3d> p(out.domain());
    for_all(p)
      mln_assertion((unsigned short)std::sqrt(ima(p)) == out(p));
  }

  /// pw image test
  {

    const pw::image<fun::p2v::iota_t, box2d> ima(fun::p2v::iota,
                                                 make::box2d(2,2, 5,5));
    image2d<short unsigned int> out(8, 8);

    level::fill(out, 0);
    out = level::transform(ima, mysqrt());
  }

  // flat image test
  {
    flat_image<short, box2d> ima(5, make::box2d(size, size));
    image2d<unsigned short> out(size, size);

    level::fill_with_value(ima, 51);
    out = level::transform(ima, mysqrt());

    box2d::piter p(out.domain());
    for_all(p)
      mln_assertion((unsigned short)std::sqrt(ima(p)) == out(p));
  }

  {
    typedef image2d<unsigned short> I;
    typedef image_if<I, fun::p2b::chess_t> II;

    I ima(size, size);
    I out(size, size);
    II ima_if = ima | fun::p2b::chess;

    level::fill_with_value(ima, 0);
    debug::iota(ima);
    out = level::transform(ima_if, mysqrt());

    II::piter p(ima_if.domain());
    for_all(p)
      mln_assertion((unsigned short)std::sqrt(ima_if(p)) == out(p));
  }

}
