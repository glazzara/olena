// Copyright (C) 2006 EPITA Research and Development Laboratory
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

/// Test oln::image1d.

#include <cassert>
// FIXME: We should not include oln/basics1d.hh, but
// oln/core/1d/image1d.hh (and oln/core/1d/neigh1d.hh ?).
#include <oln/basics1d.hh>
#include <oln/level/fill.hh>

int
main()
{
  // Fill a 1D image using its iterator.
  oln::image1d<char> ima1(3);
  oln_vtype_(oln::image1d<char>, piter) p1(ima1.topo());
  for_all(p1)
    ima1(p1) = 1;

  // Fill a 1D image using a classic loop.
  oln::image1d<int> ima2(ima1.topo());
  for (unsigned i = 0; i < 3; ++i)
    ima2(oln::point1d(i)) = 2;

  // Fill a 1D image using the routine oln::level::fill.
  oln::image1d<long> ima3(ima1.topo());
  oln::level::fill(ima3, 3);


  // Add the three images.
  oln::image1d<long> sum(ima1.topo());
  oln_vtype_(oln::image1d<long>, piter) p(sum.topo());
  for_all(p)
    sum(p) = ima1(p) + ima2(p) + ima3(p);
  // And check the sum.
  for_all(p)
    assert(sum(p) == 6);
}
