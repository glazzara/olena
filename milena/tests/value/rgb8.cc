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

/*! \file tests/value/rgb8.cc
 *
 * \brief Tests on mln::value::rgb8.
 */

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

    // FIXME: Does not compile because we lack ops!
    // mln_sum_(rgb8) sum;
    // sum + c;
    // c = c + c;

    std::cout << c << std::endl;
    std::cout << v << std::endl;

    mln_assertion(c == c);
    mln_assertion(c == v);
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
