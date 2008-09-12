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

/*! \file tests/border/duplicate.cc
 *
 * \brief Tests on mln::border::duplicate.
 */

#include <mln/core/image/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/border/duplicate.hh>


using namespace mln;

int
main (void)
{
  image2d<int> ima(3, 3, 1);

  debug::iota (ima);
  border::duplicate (ima);

  mln_assertion(ima.element( 0) == 1);
  mln_assertion(ima.element( 1) == 1);
  mln_assertion(ima.element( 2) == 2);
  mln_assertion(ima.element( 3) == 3);
  mln_assertion(ima.element( 4) == 3);
  mln_assertion(ima.element( 5) == 1);
  mln_assertion(ima.element( 6) == 1);
  mln_assertion(ima.element( 7) == 2);
  mln_assertion(ima.element( 8) == 3);
  mln_assertion(ima.element( 9) == 3);
  mln_assertion(ima.element(10) == 4);
  mln_assertion(ima.element(11) == 4);
  mln_assertion(ima.element(12) == 5);
  mln_assertion(ima.element(13) == 6);
  mln_assertion(ima.element(14) == 6);
  mln_assertion(ima.element(15) == 7);
  mln_assertion(ima.element(16) == 7);
  mln_assertion(ima.element(17) == 8);
  mln_assertion(ima.element(18) == 9);
  mln_assertion(ima.element(19) == 9);
  mln_assertion(ima.element(20) == 7);
  mln_assertion(ima.element(21) == 7);
  mln_assertion(ima.element(22) == 8);
  mln_assertion(ima.element(23) == 9);
  mln_assertion(ima.element(24) == 9);

}

