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

/*! \file tests/debug/println_with_border.cc
 *
 * \brief Test on mln::debug::println_with_border.
 */

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image1d.hh>
#include <mln/level/fill.hh>
#include <mln/debug/println.hh>
#include <mln/debug/println_with_border.hh>


using namespace mln;


int main()
{
  border::thickness = 3;

  {
    image2d<bool> msk(3, 3);
    msk.at(0, 0) = true;
    msk.at(1, 0) = true;
    msk.at(2, 0) = true;

    msk.at(0, 1) = true;
    msk.at(1, 1) = false;
    msk.at(2, 1) = true;

    msk.at(0, 2) = true;
    msk.at(1, 2) = true;
    msk.at(2, 2) = true;

    debug::println(msk);
    debug::println_with_border(msk);
  }

  {
    image1d<bool> msk(3);
    msk.at(0) = false;
    msk.at(1) = true;
    msk.at(2) = false;

    debug::println(msk);
    debug::println_with_border(msk);

  }

}
