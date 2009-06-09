// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/value/int_u8.hh>
#include <mln/value/set.hh>



template <typename T>
void test()
{
  typedef mln::value::set<T> S;
  S s;

  mln_fwd_viter(S) v(s);
  for_all(v)
    std::cout << v << ' ';
  std::cout << std::endl;

  mln_bkd_viter(S) w(s);
  for_all(w)
    std::cout << w << ' ';
  std::cout << std::endl;
}



int main()
{
  using namespace mln;

  test<bool>();
  // FIXME: Dead code?
//   test<unsigned char>();
//   test<unsigned short>();
//   test<value::int_u8>();
}
