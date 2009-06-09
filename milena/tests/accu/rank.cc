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

#include <mln/accu/rank.hh>
#include <mln/value/int_u8.hh>



template <typename A>
void fill(mln::Accumulator<A>& accu_)
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
  using namespace mln;
  using value::int_u8;

  {
    accu::meta::rank r(4);
    accu::rank<int_u8> a = accu::unmeta(r, int_u8());
    mln_assertion(a.k() == 4);
//     Meta_Accumulator<accu::meta::rank>& R = r;
//     accu::unmeta(R, int_u8());
  }

  {
    accu::rank<int_u8> accu(0);
    fill(accu);
    mln_assertion(accu.to_result() == 1u);
  }
  {
    accu::rank<int_u8> accu(1);
    fill(accu);
    mln_assertion(accu.to_result() == 2u);
  }
  {
    accu::rank<int_u8> accu(2);
    fill(accu);
    mln_assertion(accu.to_result() == 2u);
  }
  {
    accu::rank<int_u8> accu(3);
    fill(accu);
    mln_assertion(accu.to_result() == 3u);
  }
  {
    accu::rank<int_u8> accu(4);
    fill(accu);
    mln_assertion(accu.to_result() == 4u);
  }
  {
    accu::rank<int_u8> accu(5);
    fill(accu);
    mln_assertion(accu.to_result() == 5u);
  }
  {
    accu::rank<int_u8> accu(6);
    fill(accu);
    mln_assertion(accu.to_result() == 5u);
  }
  {
    accu::rank<int_u8> accu(7);
    fill(accu);
    mln_assertion(accu.to_result() == 5u);
  }
  {
    accu::rank<bool> accu_bool(1);
    accu_bool.take(true);
    accu_bool.take(true);
    accu_bool.take(true);
    accu_bool.take(true);
    accu_bool.take(false);
    mln_assertion(accu_bool == true);
  }
}
