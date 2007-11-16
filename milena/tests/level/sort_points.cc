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

/*! \file tests/level/sort_points.cc
 *
 * \brief Tests on mln::level::sort_points.
 */

#include <mln/core/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/level/sort_points.hh>
#include <mln/core/p_array.hh>


int main ()
{
  using namespace mln;

  image2d<int> ima(3, 3);
  debug::iota (ima);
  p_array<point2d> array_inc = level::sort_points_increasing(ima);
  p_array<point2d> array_dec = level::sort_points_decreasing(ima);

  p_array<point2d> array_inc_ref;
  p_array<point2d> array_dec_ref;

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      array_inc_ref.append(point2d(i, j));

  for (int i = 2; i >= 0; --i)
    for (int j = 2; j >= 0; --j)
      array_dec_ref.append(point2d(i, j));

  mln_assertion(array_inc == array_inc_ref);
  mln_assertion(array_dec == array_dec_ref);
}
