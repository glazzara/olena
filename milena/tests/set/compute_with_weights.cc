// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/set/compute_with_weights.cc
///
/// Tests on mln::set::compute_with_weights.

#include <mln/core/site_set/p_set.hh>
#include <mln/core/image/image2d.hh>
#include <mln/accu/center.hh>
#include <mln/set/compute_with_weights.hh>


int main()
{
  using namespace mln;

  {
    bool vals[] = { 1, 1, 0,
		    1, 1, 1,
		    0, 1, 1 };
    image2d<bool> msk = make::image2d(vals);
    accu::center<point2d,point2d> a;
    mln_assertion(set::compute_with_weights(a, msk) == point2d(1,1));
  }

  {
    unsigned vals[] = { 3, 1, 0,
			1, 5, 1,
			0, 1, 3 };
    image2d<unsigned> w = make::image2d(vals);
    accu::center<point2d,point2d> a;
    mln_assertion(set::compute_with_weights(a, w) == point2d(1,1));
  }

  {
    unsigned ws[] = { 1, 5, 0,
		      0, 5, 0,
		      0, 5, 1 };
    image2d<unsigned> w = make::image2d(ws);

    unsigned ls[] = { 0, 1, 2,
		      0, 1, 2,
		      0, 1, 2 };
    image2d<unsigned> l = make::image2d(ls);

    accu::center<point2d,point2d> a;
    util::array<point2d> p = set::compute_with_weights(a, w, l, 2);

    for (unsigned l = 0; l <= 2; ++l)
      mln_assertion(p[l] == point2d(l, l));
  }

}
