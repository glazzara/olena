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

/*! \file tests/histo.cc
 *
 * \brief Tests on mln::accu::histo<S> and mln::histo::data<S>.
 */

#include <iterator>

#include <mln/core/image2d_b.hh>
#include <mln/core/image1d_b.hh>

#include <mln/value/int_u8.hh>

#include <mln/debug/iota.hh>
#include <mln/accu/histo.hh>
#include <mln/histo/compute.hh>

#include <mln/debug/println.hh>

#include <mln/convert/to_image.hh>

int main()
{
  using namespace mln;
  using value::int_u8;

  {
    accu::histo< value::set<bool> > h;

    for (unsigned i = 0; i < 5; ++i)
      h.take(false);
    for (unsigned i = 0; i < 2; ++i)
      h.take(true);
    h.untake(true);

    mln_assertion(h[0] * 10 + h[1] == 51);
    mln_assertion(h(false) * 10 + h(true) == 51);
  }

  {
    image2d_b<int_u8> ima(3, 3);
    debug::iota(ima);
    ima(make::point2d(0,1)) = 255;
    debug::println(ima);
    histo::data< value::set<int_u8> > h = histo::compute(ima);
    std::cout << h << std::endl;

    image1d_b<std::size_t> ima2 = convert::to_image(h);
    debug::println(ima2);
  }
}
