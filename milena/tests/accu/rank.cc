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

/*! \file tests/accu/rank.cc
 *
 * \brief Tests on mln::accu::rank.
 */

#include <mln/accu/rank.hh>
#include <mln/value/int_u8.hh>

using namespace mln;

template <typename A>
void fill(Accumulator<A>& accu_)
{
  A& accu = exact(accu_);
  accu.take(2);
  accu.take(3);
  accu.take(1);
  accu.take(4);
  accu.take(5);
  accu.take(5);
  accu.take(2);
  accu.take(5);
}

int main()
{
  {
    accu::rank_<value::int_u8> accu(0, 8);
    fill(accu);
    mln_assertion(accu.to_result() == 1u);
  }
  {
    accu::rank_<value::int_u8> accu(1, 8);
    fill(accu);
    mln_assertion(accu.to_result() == 2u);
  }
  {
    accu::rank_<value::int_u8> accu(2, 8);
    fill(accu);
    mln_assertion(accu.to_result() == 2u);
  }
  {
    accu::rank_<value::int_u8> accu(3, 8);
    fill(accu);
    mln_assertion(accu.to_result() == 3u);
  }
  {
    accu::rank_<value::int_u8> accu(4, 8);
    fill(accu);
    mln_assertion(accu.to_result() == 4u);
  }
  {
    accu::rank_<value::int_u8> accu(5, 8);
    fill(accu);
    mln_assertion(accu.to_result() == 5u);
  }
  {
    accu::rank_<value::int_u8> accu(6, 8);
    fill(accu);
    mln_assertion(accu.to_result() == 5u);
  }
  {
    accu::rank_<value::int_u8> accu(7, 8);
    fill(accu);
    mln_assertion(accu.to_result() == 5u);
  }
  {
    accu::rank_<bool> accu_bool(1, 5);
    accu_bool.take(true);
    accu_bool.take(true);
    accu_bool.take(true);
    accu_bool.take(true);
    accu_bool.take(false);
    mln_assertion(accu_bool == true);
  }
}
