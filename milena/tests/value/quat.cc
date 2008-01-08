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

/*! \file tests/value/quat.cc
 *
 * \brief Tests on mln::value::quat.
 */

#include <iostream>

#include <mln/value/quat.hh>


int main()
{
  using namespace mln;

  value::quat q1(3.f, 4.f, 1.6f, 0.5f);
  value::quat q2(1.2, make::vec(3, 6, 4));
  value::quat q3(make::vec(1.3, 3., -6., 4.));

  std::cout << q1 << std::endl;
  std::cout << q2 << std::endl;
  std::cout << q3 << std::endl;

  std::cout << q1.s() << std::endl;

  q1.s() = 2.6;
  std::cout << q1 << std::endl;

  std::cout << q1.v() << std::endl;

  q2.v() = make::vec(1.4, 5.9, 3.1);
  std::cout << q2 << std::endl;

  std::cout << q2 * q3 << std::endl;

  mln_assertion(! q3.is_unit());
  q3.set_unit();
  std::cout << q3 << std::endl;
  mln_assertion(q3.is_unit());

  std::cout << q2.conj() << std::endl;
  std::cout << q2.inv() << std::endl;
  std::cout << norm::l2(q2.to_vec()) << ' '<< norm::l2(q2.inv().to_vec())
	    << std::endl;
  std::cout << q2.inv().inv() << std::endl;

}
