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

/*! \file tests/border/mirror.cc
 *
 * \brief Tests on mln::border::mirror.
 */

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/border/mirror.hh>

#include <mln/debug/println_with_border.hh>

using namespace mln;

int
main (void)
{
  {
    image1d<int> ima(2, 3);
    debug::iota(ima);
    border::mirror(ima);
    mln_assertion(ima.element(0) == 2);
    mln_assertion(ima.element(1) == 2);
    mln_assertion(ima.element(2) == 1);
    mln_assertion(ima.element(3) == 1);
    mln_assertion(ima.element(4) == 2);
    mln_assertion(ima.element(5) == 2);
    mln_assertion(ima.element(6) == 1);
    mln_assertion(ima.element(7) == 1);
  }


  image2d<int> ima(2, 3, 2);

  debug::iota(ima);
  border::mirror(ima);

  mln_assertion(ima.element( 0) == 1);
  mln_assertion(ima.element( 1) == 1);
  mln_assertion(ima.element( 2) == 4);
  mln_assertion(ima.element( 3) == 5);
  mln_assertion(ima.element( 4) == 6);
  mln_assertion(ima.element( 5) == 3);
  mln_assertion(ima.element( 6) == 3);
  mln_assertion(ima.element( 7) == 1);
  mln_assertion(ima.element( 8) == 1);
  mln_assertion(ima.element( 9) == 1);
  mln_assertion(ima.element(10) == 2);
  mln_assertion(ima.element(11) == 3);
  mln_assertion(ima.element(12) == 3);
  mln_assertion(ima.element(13) == 3);
  mln_assertion(ima.element(14) == 2);
  mln_assertion(ima.element(15) == 1);
  mln_assertion(ima.element(16) == 1);
  mln_assertion(ima.element(17) == 2);
  mln_assertion(ima.element(18) == 3);
  mln_assertion(ima.element(19) == 3);
  mln_assertion(ima.element(20) == 2);
  mln_assertion(ima.element(21) == 5);
  mln_assertion(ima.element(22) == 4);
  mln_assertion(ima.element(23) == 4);
  mln_assertion(ima.element(24) == 5);
  mln_assertion(ima.element(25) == 6);
  mln_assertion(ima.element(26) == 6);
  mln_assertion(ima.element(27) == 5);
  mln_assertion(ima.element(28) == 4);
  mln_assertion(ima.element(29) == 4);
  mln_assertion(ima.element(30) == 4);
  mln_assertion(ima.element(31) == 5);
  mln_assertion(ima.element(32) == 6);
  mln_assertion(ima.element(33) == 6);
  mln_assertion(ima.element(34) == 6);
  mln_assertion(ima.element(35) == 4);
  mln_assertion(ima.element(36) == 4);
  mln_assertion(ima.element(37) == 1);
  mln_assertion(ima.element(38) == 2);
  mln_assertion(ima.element(39) == 3);
  mln_assertion(ima.element(40) == 6);
  mln_assertion(ima.element(41) == 6);
}

