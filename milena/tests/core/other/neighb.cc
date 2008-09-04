// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/core/other/neighb.cc
/// \brief Tests on mln::neighb<D> specializations.

#include <mln/core/alias/neighb1d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/neighb3d.hh>

using namespace mln;

template <typename N>
void test(const Neighborhood<N>& nbh, const mln_point(N)& p_ref,
	  unsigned size)
{
  mln_niter(N) n(nbh, p_ref);
  unsigned i = 0;
  for_all(n)
    ++i;
  mln_assertion(i == size);
}

int main()
{
  test(c2(), make::point1d(0), 2);

  test(c4(), make::point2d(0, 0), 4);
  test(c8(), make::point2d(0, 0), 8);

  test( c6(), make::point3d(0, 0, 0), 6);
  test(c18(), make::point3d(0, 0, 0), 18);
  test(c26(), make::point3d(0, 0, 0), 26);
}
