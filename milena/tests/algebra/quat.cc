// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <iostream>

#include <mln/core/contract.hh>

#include <mln/algebra/quat.hh>
#include <mln/norm/l2.hh>


int main()
{
  using namespace mln;

  algebra::quat q1(3.f, 4.f, 1.6f, 0.5f);
  algebra::quat q2(1.2f, make::vec(3, 6, 4));
  algebra::quat q3(make::vec(1.3, 3., -6., 4.));

  std::cout << q1 << std::endl;
  std::cout << q2 << std::endl;
  std::cout << q3 << std::endl;

  std::cout << q1.s() << std::endl;

  q1.s() = 2.6f;
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
