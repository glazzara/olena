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

#include "border_fill.hh"
#include <mln/core/image2d_b.hh>
#include <mln/debug/println_with_border.hh>

using namespace mln;

int
main (void)
{
  std::cout << std::endl
	    << "Test 2d size=2x3 in int"
	    << std::endl
	    << std::endl;
  image2d_b<int> i2(2, 3);
  border::fill (i2, 5);
  debug::println_with_border(i2);
  std::cout << std::endl;
}
