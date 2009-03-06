// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file tests/level/sort_psites.cc
///
/// Tests on mln::level::sort_psites.

#include <mln/core/image/image2d.hh>
#include <mln/make/image2d.hh>
#include <mln/level/sort_psites.hh>
#include <mln/core/site_set/p_array.hh>


int main ()
{
  using namespace mln;

  int vals[] = { 0, 3, 4,
		 2, 2, 2,
		 0, 1, 4 };
  image2d<int> ima = make::image2d(vals);
  p_array<point2d> array_inc = level::sort_psites_increasing(ima);
  p_array<point2d> array_dec = level::sort_psites_decreasing(ima);

  {
    p_array<point2d>::fwd_piter p1(array_inc);
    p_array<point2d>::bkd_piter p2(array_dec);
    for_all_2(p1, p2)
      mln_assertion(ima(p1) == ima(p2));
  }

  p_array<point2d> array_inc_ref;
  p_array<point2d> array_dec_ref;

  typedef def::coord C;
  for (C i = 0; i < 3; ++i)
    for (C j = 0; j < 3; ++j)
      array_inc_ref.append(point2d(i, j));

  for (C i = 2; i >= 0; --i)
    for (C j = 2; j >= 0; --j)
      array_dec_ref.append(point2d(i, j));

  mln_assertion(array_inc == array_inc_ref);
  mln_assertion(array_dec == array_dec_ref);
}
