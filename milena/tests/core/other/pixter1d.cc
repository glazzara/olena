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

/// \file tests/core/other/pixter1d.cc
/// \brief Tests on 1-D image pixters.

#include <mln/core/image/image1d.hh>

#include <mln/debug/iota.hh>

int main()
{
  using namespace mln;

  {
    box1d b(make::point1d(2), make::point1d(4));
    image1d<int> ima(b, 2);

    mln_pixter_(image1d<int>) p(ima);
    for_all(p)
      std::cout << p << std::endl;
  }

  {
    box1d b(make::point1d(2), make::point1d(6));
    image1d<int> ima(b, 1);

    debug::iota(ima);

    {
      mln_fwd_piter_(image1d<int>) pi(ima.domain());
      mln_fwd_pixter_(image1d<int>) p(ima);
      pi.start();
      p.start();
      unsigned i = 0;
      while (pi.is_valid())
	{
	  mln_assertion(ima(pi) == p.val());
	  pi.next();
	  p.next();
	  ++i;
	}
      mln_assertion(i == b.npoints());
    }

    {
      mln_bkd_piter_(image1d<int>) pi(ima.domain());
      mln_bkd_pixter_(image1d<int>) p(ima);
      pi.start();
      p.start();
      unsigned i = 0;
      while (pi.is_valid())
	{
	  mln_assertion(ima(pi) == p.val());
	  pi.next();
	  p.next();
	  ++i;
	}
      mln_assertion(i == b.npoints());
    }
  }
}
