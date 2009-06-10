// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/value/rgb8.hh>
#include <mln/literal/all.hh>


int main()
{
  using namespace mln;
  using value::rgb8;
  using value::rgb;

  using literal::blue;
  using literal::white;

  {
    rgb8 v;
    v.red() = 0;
    v.green() = 1;
    v.blue() = 2;
    rgb8 c(0, 1, 2);

    // Exercise some operators.
    rgb8 c1(200,100,0);
    rgb8 c2(100,200,50);
    c1 + c2;
    c1 - c2;
    /* FIXME: Doesn't work.  Calls an operator* triggering an overflow
       error.  We have to check where `interop' types should be used
       instead of `equiv' types to avoid such a behavior.  */
//     3 * c1;
//     c1 * 3;
    c1 / 5;

    mln_sum_(rgb8) sum = literal::zero;
    sum = sum + c;
    c = c + c;

    std::cout << c << std::endl;
    std::cout << v << std::endl;
    std::cout << sum << std::endl;

    mln_assertion(c == c);
    mln_assertion(c == 2 * v);
    v.green() = 255;
    std::cout << v << std::endl;

    mln_assertion(v != c);

    rgb<20> b = blue;
    std::cout << b << std::endl;

    rgb<20> w = white;
    std::cout << w << std::endl;

    mln_assertion(b != w);
  }
}
