// Copyright (C) 2007 EPITA Research and Development Laboratory
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
#include <mln/core/clock_neighb2d.hh>
#include <mln/debug/println.hh>
#include <mln/data/fill.hh>
#include <mln/labeling/level.hh>
#include <mln/core/alias/neighb2d.hh>

namespace mln
{
  template <typename I>
  unsigned testc4(const Image<I>& input_,
		  point2d p)
  {
    const I& input = exact (input_);
    bool previous_is_false;
    unsigned res = 0;

    dpoint2d dp(-1,-1);
    clock_neighb2d nbh = cc8(dp);


    // just to get the last previous_is_false.
    {
      mln_bkd_niter(clock_neighb2d)   b(nbh , p);
      b.start();
      previous_is_false = !input(b);
    }

    {
      mln_fwd_niter(clock_neighb2d)   n(nbh , p);
      for_all(n)
	{
	  //y in c4
	  bool x = input(n);
	  n.next();
	  mln_assertion(n.is_valid());
	  bool y = input(n);
	  if (!x && y)
	    ++res;
	  else
	    if (x && y && previous_is_false)
	      ++res;

	  previous_is_false = !y;

	}
    }
    return res == 0 ? 1 : res;
  }

  template <typename I>
  unsigned testc8(const Image<I>& input_,
		  point2d p)
  {
    const I& input = exact (input_);
    bool previous_is_false;
    unsigned res = 0;

    dpoint2d dp(-1,0);
    clock_neighb2d nbh = cc8(dp);


    // just to get the last previous_is_false.
    {
      mln_bkd_niter(clock_neighb2d)   b(nbh , p);
      b.start ();
      bool x = input(b);
      b.next();
      mln_assertion(b.is_valid());
      bool y = input(b);
      previous_is_false = !y && !x;
    }


    {
      mln_fwd_niter(clock_neighb2d)   n(nbh , p);
      for_all(n)
	{
	  //y in c4
	  bool x = input(n);
	  n.next();
	  mln_assertion(n.is_valid());
	  bool y = input(n);

	  if (!x && y)
	    ++res;
	  else
	    if (x && previous_is_false)
	      ++res;
	  previous_is_false = !y && !x;
	}
    }
    return res == 0 ? 0 : res - 1;
  }

}

int main ()
{
  using namespace mln;

  unsigned tmp = 0;

  image2d<bool>  ima(3,3);
  dpoint2d	dp (1,0);
  point2d	p (1,1);


  for (int a0 = 0; a0 < 2; ++a0)
    for (int a1 = 0; a1 < 2; ++a1)
      for (int a2 = 0; a2 < 2; ++a2)
	for (int a3 = 0; a3 < 2; ++a3)
	  for (int a4 = 0; a4 < 2; ++a4)
	    for (int a5 = 0; a5 < 2; ++a5)
	      for (int a6 = 0; a6 < 2; ++a6)
		for (int a7 = 0; a7 < 2; ++a7)

  {
    unsigned n;
    unsigned m;
    bool t[9] = {a0, a1, a2,
		 a3,  1, a4,
		 a5, a6, a7};


    data::fill(ima, t);

    image2d<unsigned> out = labeling::value(ima, false, c8(), n);
    tmp = testc4(ima, p);
    if (tmp != n)
      {
	std::cout << "C4--------------"<< std::endl;
	debug::println(ima);
	debug::println(out);
	std::cout << "tmp = " << tmp << std::endl;
	std::cout << "n = " << n << std::endl;
	std::cout << "C4--------------"<< std::endl;
      }


    bool u[9] = {a0, a1, a2,
		 a3,  0, a4,
		 a5, a6, a7};


    ///  0 before
    data::fill(ima, u);
    out = labeling::value(ima, false, c4(), m);

    ///  1 after
    data::fill(ima, t);
    image2d<unsigned> out2 = labeling::value(ima, false, c4(), n);

    int diff = n - m;
    if (diff < 0)
      diff = 0;
    tmp = testc8(ima, p);
    int temp = tmp;
    if (temp != diff)
      {
	std::cout << "C8--------------"<< std::endl;
	debug::println(ima);
	debug::println(out);
	debug::println(out2);
	std::cout << "tmp = " << tmp << std::endl;

	std::cout << "n = " << n << std::endl;
	std::cout << "m = " << m << std::endl;
	std::cout << "diff = " << diff << std::endl;
	std::cout << "C8--------------"<< std::endl;
      }

  }
}
