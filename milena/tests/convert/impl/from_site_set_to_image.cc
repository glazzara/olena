// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file
///
/// Tests on mln::convert::impl::from_site_set_to_image.

#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/convert/from_to.hh>

#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  p_array<point2d> arr;
  point2d p(1,1);
  arr.append(p);
  dpoint2d dp[] = { right, right, down, down, left, left, up };
  for (unsigned i = 0; i < 7;  ++i)
    p += dp[i], arr.append(p);

  image2d<bool> ima;
  convert::impl::from_site_set_to_image(arr, ima);

  // debug::println(ima);
}
