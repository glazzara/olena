// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#include <iostream>
#include <xtd/optraits.hh>
#include <xtd/math.hh>


template <typename T>
static void foo()
{
  xtd_op_plus_trait(int, char) tmp;
}


template <typename E1, typename E2>
static
void test_plus(const xtd::abstract::fun_expr_<E1>& expr1,
	       const xtd::abstract::fun_expr_<E2>& expr2)
{
  const xtd_op_plus_trait(E1, E2)& tmp = expr1 + expr2;
  std::cout << tmp(1.) << std::endl;
}


int main()
{
  using xtd::cos;
  using xtd::sin;
  using xtd::_1;

  typeof(1+2u) tmp;
//   2 * cos(_1);

  test_plus(2 * cos(_1), sin(_1));
}
