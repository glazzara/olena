// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <map>
#include <mln/value/proxy.hh>


template <typename T>
struct test
{
  // Interface required by mln::value::proxy.

  typedef int psite;
  typedef T value;

  void write_(const psite& p, const value& v)
  {
    // std::cout << "write value " << v << " at " << p << std::endl;
    data_[p] = v;
  }

  value read_(const psite& p) const
  {
    // std::cout << "read at " << p << " -> value " << data_[p] << std::endl;
    return data_[p];
  }

  mutable std::map<psite,value> data_;

  // Some extra interface of an Image.

  value operator()(const psite& p) const
  {
    return read_(p); // Read only!
  }

  mln::value::proxy<test> operator()(const psite& p)
  {
    mln::value::proxy<test> prx(*this, p);
    return prx;
  }
};



int main()
{
  using namespace mln;

  test<float> ima;

  int p = 3;

  ima(p) = 8.5;
  ima(p) = 6 * ima(p);
  mln_assertion(ima(p) == 51);

  test<int> ima_;
  ima_(p) = 51;
  ima_(p) = ima_(p);
  mln_assertion(ima_(p) == 51);
}
