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

/*! \file tests/metal_vec.cc
 *
 * \brief Test on mln::algebra::vec.
 */

#include <mln/algebra/vec.hh>
#include <mln/value/int_u8.hh>


struct nu {};

int main()
{
  using namespace mln;

  algebra::vec<3,int> v_int = make::vec(3, 6, 7);
  algebra::vec<3,float> v_f = make::vec(2.6, 1.9, 5.2);

  mln_assertion((v_int + v_f) == ((v_f + v_int)));
  mln_assertion((v_f / 3) == ((3 * v_f) / 9));

  value::int_u8 i = 3;
  mln_assertion((i * v_f) == (value::scalar(i) * v_f));

  mln_assertion((literal::zero + v_f) == v_f);
}
