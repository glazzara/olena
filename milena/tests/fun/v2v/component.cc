// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/fun/v2v/component.cc
///
/// \brief Test fun::v2v::component.

#include <mln/fun/v2v/component.hh>
#include <mln/algebra/vec.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/hsl.hh>


int main()
{
  using namespace mln;

  {
    fun::v2v::component<int, 0> f;
    int i = 51;
    mln_assertion(f(i) == 51);
  }
  {
    typedef int V[1];
    V v;
    v[0] = 51;
    fun::v2v::component<V, 0> f;
    mln_assertion(f(v) == 51);
  }
  {
    typedef algebra::vec<2,int> V;
    V v;
    v[0] = 51;
    fun::v2v::component<V, 0> f;
    mln_assertion(f(v) == 51);
  }
  {
    typedef value::rgb8 V;
    V v;
    v.red() = 51;
    fun::v2v::component<V, 0> f;
    mln_assertion(f(v) == 51);
  }
  {
    typedef value::hsl_<int,int,int> V;
    V v;
    v.hue() = 51;
    fun::v2v::component<V, 0> f;
    mln_assertion(f(v) == 51);
  }
}
