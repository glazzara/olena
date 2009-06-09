// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/core/image/image2d.hh>

#include <mln/debug/iota.hh>

int main()
{
  using namespace mln;

  {
    box2d b(point2d(2, 1), point2d(4, 3));
    image2d<int> ima(b, 2);

    mln_pixter_(image2d<int>) p(ima);
    for_all(p)
      std::cout << p << std::endl;
  }

  {
    box2d b(point2d(2, 1), point2d(6, 3));
    image2d<int> ima(b, 1);
    debug::iota(ima);

    {
      mln_fwd_piter_(image2d<int>) pi(ima.domain());
      mln_fwd_pixter_(image2d<int>) p(ima);
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
      mln_assertion(i == b.nsites());
    }

    {
      mln_bkd_piter_(image2d<int>) pi(ima.domain());
      mln_bkd_pixter_(image2d<int>) p(ima);
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
      mln_assertion(i == b.nsites());
    }
  }
}
