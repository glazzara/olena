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

#include <iomanip>
#include <iostream>
#include <sstream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/core/routine/clone.hh>

#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/level/fill.hh>
#include <mln/debug/println.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/accu/bbox.hh>
#include <mln/io/pgm/save.hh>


#include <mln/core/image/cast_image.hh>

namespace mln
{

  namespace my
  {


    void save_state(const image2d<unsigned char>& out)
    {
//       static int id = 0;
//       std::stringstream filename;
//       filename << "fllt_trace_" << std::setw(5) << std::setfill('0')
// 	       << std::right << id++ << ".ppm";

//       io::pgm::save(cast_image<value::int_u8>(out), filename.str());
      //io::pgm::save(out, filename.str());
    }

    void save_u(const image2d<value::int_u8>& u,
		const image2d<unsigned char>& is,
		box2d R_box)
    {
      static int id = 0;
      std::stringstream filename;
      filename << "fllt_u_" << std::setw(5) << std::setfill('0')
	       << std::right << id++ << ".ppm";

      image2d<value::int_u8> out = clone(u);
      const unsigned in_R = 255;

      mln_assertion(R_box.is_valid());
      mln_piter_(box2d) p(R_box);
      for_all(p)
	if (is(p) == in_R)
	  out(p) = 255;

      io::pgm::save(out, filename.str());
      //io::pgm::save(out, filename.str());
    }

    template <typename I, typename Nbh>
    void fllt(const Image<I>& input_, const Neighborhood<Nbh>& nbh_)
    {
      const I& input = exact(input_);
      const Nbh& nbh = exact(nbh_);

      unsigned l = 0, l_max;
      mln_ch_value(I, unsigned) reg_min =
	labeling::regional_minima(input, nbh, l_max);

      // Variables.
      I u = mln::clone(input);
      mln_point(I) x0;
      mln_value(I) g, gN;
      image2d<unsigned char> is(input.domain());
      image2d<bool> tagged(input.domain());
      const unsigned in_R = 255, in_N = 2, in_O = 0;

      typedef p_array<mln_point(I)> arr_t;
      arr_t A;
      arr_t N[256];

      accu::bbox<mln_point(I)> R_box;

      level::fill(tagged, false);
      mln_piter(I) min(input.domain());
      min.start();
      // Step 1.
    step_1:
      {
#ifdef FLLTDEBUG
	std::cout << "Step 1" << std::endl;
#endif
	if (l == l_max)
	  return;
	l += 1;
	for(min.next(); min.is_valid(); min.next())
	  if (reg_min(min) !=0 && !tagged(min))
	    break;

#ifdef FLLTDEBUG
	std::cout << "min local : " << min << std::endl;
#endif

	if (!min.is_valid())
	  goto end;

	x0 = min;
	g = input(x0);
      }

      // Step 2.
    step_2:
      {
#ifdef FLLTDEBUG
	std::cout << "Step 2" << std::endl;
#endif
	R_box.init();
	level::fill(is, in_O);
	A.clear();
	A.append(x0);
	for (unsigned i = 0; i < 256; ++i)
	  N[i].clear();
      }

      // Step 3.
    step_3:
      {
#ifdef FLLTDEBUG
	std::cout << "Step 3" << std::endl;
#endif
	mln_piter(arr_t) a(A);
	mln_niter(Nbh) x(nbh, a);

	// Stop.
	if (A.nsites() == 0)
	  goto end;

	// R <- R U A
#ifdef FLLTDEBUG
	      std::cout << "Add To R : ";
#endif
	for_all(a)
	  {
	    tagged(a) = true;
	    is(a) = in_R;
#ifdef FLLTDEBUG
	      std::cout << a;
#endif
	  }
#ifdef FLLTDEBUG
	std::cout << std::endl;
#endif


#ifdef FLLTDEBUG
	std::cout << "points of A : " << A.nsites() << std::endl;
#endif
	mln_assertion(A.nsites() > 0);
	R_box.take(A.bbox());
	mln_assertion(R_box.is_valid());

#ifdef FLLTTRACE
	save_state(is);
	save_u(u, is, R_box);
#endif
	// N <- N U { x in nbh of A and not in R }
#ifdef FLLTDEBUG
	      std::cout << "Add To N : ";
#endif
	for_all(a)
	  for_all(x)
  	    if (u.has(x) && (is(x) == in_O))
	    {
	      is(x) = in_N;
	      N[u(x)].append(x);
#ifdef FLLTDEBUG
	      std::cout << x;
#endif
	    }
#ifdef FLLTDEBUG
	std::cout << std::endl;
#endif
	// gN = min u(x) for all x in N
	unsigned i;
	for (i = 0; i < 256; ++i)
	  if (N[i].nsites() != 0)
	  {
	    gN = i;
	    break;
	  }
#ifdef FLLTDEBUG
	std::cout << "gN = " << gN << ", g = " << g << std::endl;
#endif

	// Stop if N is empty.
#ifdef FLLTDEBUG
	std::cout << "i =   " << i << std::endl;
#endif
	if (i == 256)
	  goto step_4c;
	mln_assertion(N[gN].nsites() > 0);

	// FIXME: update the number of CC of the border of R
      }

      // Step 4.
    step_4:
      {
	// a)
	if (g < gN)
	{
#ifdef FLLTDEBUG
	  std::cout << "Step 4a" << std::endl;
#endif
	  // FIXME: DO the hole thing.

	  g = gN;

	  A = N[gN];
	  N[gN].clear();
	  mln_assertion(A.nsites() > 0);
	  mln_assertion(N[gN].nsites() == 0);
	  goto step_3;
	}
	// b)
	else if (g == gN)
	{
#ifdef FLLTDEBUG
	  std::cout << "Step 4b" << std::endl;
#endif
	  A = N[gN];
	  N[gN].clear();
	  mln_assertion(A.nsites() > 0);
	  mln_assertion(N[gN].nsites() == 0);
	  goto step_3;
	}
	// c)
	else
	  {
	  step_4c:
#ifdef FLLTDEBUG
	    std::cout << "Step 4c" << std::endl;
#endif
	    mln_assertion(R_box.is_valid());
	    mln_piter(box_<mln_point(I)>) p(R_box);
	    for_all(p)
	      if (is(p) == in_R)
		u(p) = g;
	    goto step_1;
	  }
      }
    end:
      {
      }
    }

  } // end of namespace mln::my

} // end of namespace mln


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;

  io::pgm::load(lena, "small.pgm");


//   int_u8 vs[9][9] = {

//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },
//     {  2, 2, 2, 2, 2, 2, 2, 2, 2  },

//   };

  //image2d<int_u8> lena = make::image2d(vs);

  my::fllt(lena, c4());
  io::pgm::save(lena, "./out.pgm");

}
