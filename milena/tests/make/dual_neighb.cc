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

/// \file tests/make/dual_neighb.cc
///
/// Tests on mln::make::dual_neighb.

#include <mln/make/dual_neighb.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/opt/at.hh>



template <typename I, typename N>
unsigned count(const I& ima, const N& nbh)
{
  mln_piter(I) p(ima.domain());
  mln_niter(N) n(nbh, p);
  unsigned c = 0;
  for_all(p)
    for_all(n)
      ++c;
  return c;
}


int main()
{
  using namespace mln;

  image2d<bool> ima(1, 2, 1);
  opt::at(ima, 0, 0) = true;
  opt::at(ima, 0, 1) = false;

  mln_assertion( count(ima, make::dual_neighb(ima, c4(), c8()))
		 == c4().size() + c8().size() );

  mln_assertion( make::dual_neighb(ima, c4(), c8()).foreground() == c4() );
  mln_assertion( make::dual_neighb(ima, c4(), c8()).background() == c8() );

  // We can observe that the neighboord is not restricted by the
  // respective domains defined by ima(p) == false and ima(p) == true:
  // for instance, at (0,0) we are in the *object* (ima(0,0) == true),
  // thus the neighborhood is c4(), yet from p we browse neighbors n
  // that are in the *background* (ima(n) == false).
}
