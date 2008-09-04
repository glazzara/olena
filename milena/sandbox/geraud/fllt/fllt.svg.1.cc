// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/core/clone.hh>

#include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/level/fill.hh>
#include <mln/debug/println.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/accu/bbox.hh>

#include <mln/literal/black.hh>
#include <mln/literal/white.hh>
#include <mln/literal/colors.hh>

#include <sstream>


namespace mln
{

  namespace my
  {

    template <typename N_t>
    unsigned compute_gN(const N_t& N)
    {
      for (unsigned i = 0; i < 256; ++i)
	if (N[i].npoints() != 0)
	  return i;
      mln_invariant(0);
      return 0;
    }


    template <typename I, typename A_t>
    void save(const I& is, const A_t& A)
    {
      static unsigned counter = 0;
      using value::rgb8;

      image2d<rgb8> temp(is.domain());
      level::fill(temp, literal::black);

      mln_piter(I) p(is.domain());
      for_all(p)
	switch (is(p)) {
	case 1: // R
	  temp(p) = literal::red;
	  break;
	case 2: // N
	  temp(p) = literal::green;
	  break;
	}

      mln_piter(A_t) a(A);
      for_all(a) // A
	temp(a) = literal::blue;

      std::stringstream filename;
      filename << "./temp_" << ++counter << ".ppm";
      io::ppm::save(temp, filename.str());
    }


    template <typename I, typename Nbh>
    void fllt(const Image<I>& input_, const Neighborhood<Nbh>& nbh_)
    {
      const I& input = exact(input_);
      const Nbh& nbh = exact(nbh_);

      unsigned l = 0, l_max;
      mln_ch_value(I, unsigned) reg_min = labeling::regional_minima(input, nbh, l_max);

      // Variables.
      I u = mln::clone(input);
      mln_point(I) x0;
      mln_value(I) g, gN;
      image2d<unsigned char> is(input.domain());
      const unsigned in_R = 1, in_N = 2, in_O = 0;
      
      typedef p_array<mln_point(I)> arr_t;
      arr_t A;
      arr_t N[256];

      accu::bbox<mln_point(I)> R_box;

      
      // Step 1.
    step_1:
      {
	if (l == l_max)
	  return;
	l += 1;
	mln_piter(I) p(input.domain());
	for_all(p)
	  if (reg_min(p) == l)
	    break;
	x0 = p;
	g = input(x0);
      }

      // Step 2.
    step_2:
      {
	R_box.init();
	level::fill(is, in_O);
	A.append(x0);
      }
	  
      // Step 3.
    step_3:
      {
	mln_piter(arr_t) a(A);
	mln_niter(Nbh) x(nbh, a);

	my::save(is, A);

	// R <- R U A
	for_all(a)
	  is(a) = in_R;
	R_box.take(A.bbox());

	// N <- N U { x in nbh of A and not in R }
	for_all(a)
	  for_all(x)
  	    if (u.has(x) && is(x) == in_O)
	      {
		N[u(x)].append(x);
		is(x) = in_N;
	      }
	// gN = min u(x) for all x in N
	gN = compute_gN(N);

	// FIXME: update the number of CC of the border of R
      }

      // Step 4.
    step_4:
      {
	// a)
	if (g < gN)
	  {
	    // FIXME: DO the hole thing.
	    A = N[g];
	    N[g].clear();
	    g = gN;
	    gN = compute_gN(N);
	    goto step_3;
	  }
	// b)
	else if (g == gN)
	  {
	    A = N[g];
	    N[g].clear();
	    g = gN;
	    gN = compute_gN(N);
	    goto step_3;
	  }
	// c)
	else
	  {
	    mln_piter(box_<mln_point(I)>) p(R_box);
	    for_all(p)
	      if (is(p) == in_R)
		u(p) = g;
	    goto step_1;
	  }
      }

    }

  } // end of namespace mln::my

} // end of namespace mln


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;
  io::pgm::load(lena, "../../img/tiny.pgm");

  my::fllt(lena, c4());
  io::pgm::save(lena, "./out.pgm");

}
