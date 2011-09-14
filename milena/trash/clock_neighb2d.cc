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

namespace mln
{
  template <typename I, typename N>
  void test_fwd(Image<I>& input_,
		const Neighborhood<N>& nbh)
  {
    I& input = exact (input_);
    point2d	p (1,1);
    int		v = 1;

    mln_fwd_niter(N)   n (nbh, p);
    for_all (n)
      {
	std::cout << n << std::endl;
	input(n) = v++;
      }

  }

  template <typename I, typename N>
  void test_bkd(Image<I>& input_,
		const Neighborhood<N>& nbh)
  {
    I& input = exact (input_);
    point2d	p (1,1);
    int		v = 1;

    mln_bkd_niter(N)   n (nbh, p);
    for_all (n)
      {
	std::cout << n << std::endl;
	input(n) = v++;
      }

  }

}


int main ()
{
  using namespace mln;


  image2d<int>  ima(3,3);
  dpoint2d	dp (1,0);

  test_fwd(ima, cc4(dp));
  debug::println(ima);
  test_bkd(ima, cc4(dp));
  debug::println(ima);


  dpoint2d	dp2 (-1,-1);

  test_fwd(ima, cc8(dp2));
  debug::println(ima);
  test_bkd(ima, cc8(dp2));
  debug::println(ima);

}
