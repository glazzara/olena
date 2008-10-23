// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/*! \file tests/level/fill_with_value.cc
 *
 * \brief Tests on mln::level::fill_with_value
 */


#include <mln/level/fill_with_value.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/flat_image.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/image/sub_image.hh>
#include <mln/core/image/extension_val.hh>

#include <mln/value/rgb8.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/make/box2d.hh>

int main()
{
  using namespace mln;
  const unsigned size = 50;

  {
    typedef image1d<unsigned char> I;
    I ima(size);
    level::fill_with_value(ima, 51);
    mln_piter_(I) p(ima.domain());
    for_all(p)
      mln_assertion(ima(p) == 51);
  }


  {
    typedef image2d<unsigned char> I;
    I ima(size, size);
    level::fill_with_value(ima, 51);
    mln_piter_(I) p(ima.domain());
    for_all(p)
      mln_assertion(ima(p) == 51);
  }

  {
    typedef image3d<value::rgb8> I;
    I ima(size, size, size);
    level::fill_with_value(ima, value::rgb8(255, 0, 255));
    mln_piter_(I) p(ima.domain());
    for_all(p)
      mln_assertion(ima(p) == value::rgb8(255, 0, 255));
  }


  {
    flat_image<short, box2d> ima(5, make::box2d(2, 3));
    level::fill_with_value(ima, 51);
    box2d::piter p(ima.domain());
    for_all(p)
      mln_assertion(ima(p) == 51);
  }


  {
    typedef image2d<unsigned char> I;
    typedef image_if<I, fun::p2b::chess_t> II;

    I ima(size, size);
    level::fill_with_value(ima, 51);

    II ima_if = ima | fun::p2b::chess;
    level::fill_with_value(ima_if, 42);

    II::piter p(ima_if.domain());
    for_all(p)
      mln_assertion(ima_if(p) == 42);
  }

  {
    typedef image2d<int> I;
    typedef sub_image< image2d<int>, box2d > II;
    I ima(size, size);
    II sub_ima(ima, make::box2d(4,4, 10,10));

    level::fill_with_value(sub_ima,  5);

    II::piter p(sub_ima.domain());
    for_all(p)
      mln_assertion(sub_ima(p) == 5);
  }

  {
    typedef image2d<int> I;
    typedef extension_val< image2d<int> > II;
    I ima(size, size);
    II extend_ima(ima, 5);

    level::fill_with_value(extend_ima,  51);

    II::piter p(extend_ima.domain());
    for_all(p)
      mln_assertion(extend_ima(p) == 51);
  }

}
