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

/*! \file tests/core/safe_image.cc
 *
 * \brief Tests on mln::safe_image.
 */

#include <mln/core/image/image2d.hh>
#include <mln/core/image/safe.hh>


int main()
{
  using namespace mln;

  typedef image2d<int> I;
  I ima(1, 1);
  point2d
    in = make::point2d(0, 0),
    out = make::point2d(-999, -999);

  {
    safe_image<I> ima_ = safe(ima, 7);

    ima_(in) = 51;
    mln_assertion(ima_(in) == 51);
    
    ima_(out) = 0;
    mln_assertion(ima_(out) == 7);

    // test "image_adaptor_<..>::operator I() const"
    I ima2 = ima_;
    const I ima3 = ima_;
  }

  {
    safe_image<const I> ima_ = safe(ima, 7);

    ima(in) = 51;
    mln_assertion(ima_(in) == 51);
    mln_assertion(ima_(out) == 7);
  }

}
