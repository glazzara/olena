// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/border_fill.cc
 *
 * \brief Tests on mln::border::fill.
 */

#include "border_duplicate.hh"
#include <mln/core/image1d_b.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/println_with_border.hh>
#include <mln/debug/iota.hh>

using namespace mln;

int
main (void)
{
  std::cout << std::endl
	    << "Test 1d size= box [-6 .. -3]  with border=6 in value::int_u8"
	    << std::endl
	    << std::endl;

  box1d b(point1d(-6), point1d(-3));
  image1d_b<value::int_u8> ima(b, 6);
  debug::iota (ima);
  border::duplicate (ima);
  std::cout << std::endl;
  debug::println_with_border(ima);
  std::cout << std::endl;
}
