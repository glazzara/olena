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

/*! \file tests/norm_l2.cc
 *
 * \brief Tests on mln::norm::l2.
 */

#include <iostream>

#include <mln/norm/l2.hh>

#include <mln/metal/vec.hh>

int main()
{
  using namespace mln;
  
  int t1[] = {2, 6, -1, 2};

  std::cout << norm::l2(t1) << std::endl;

  int t2[] = {2, -6, -1, 2};

  std::cout << norm::l2_distance(t1, t2) << std::endl;

  metal::vec<4,float> v = make::vec(2, 6, -1, 2);

  std::cout << norm::l2(v);
}
