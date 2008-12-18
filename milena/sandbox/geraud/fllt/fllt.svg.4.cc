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
#include <mln/core/routine/clone.hh>
#include <mln/core/image_if_value.hh>
#include <mln/core/image/sub_image.hh>

#include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/level/fill.hh>
#include <mln/level/compare.hh>
#include <mln/debug/println.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/accu/bbox.hh>
#include <mln/geom/bbox.hh>
#include <mln/pw/all.hh>

#include <mln/literal/black.hh>
#include <mln/literal/white.hh>
#include <mln/literal/colors.hh>

#include <mln/opt/at.hh>

#include <sstream>


namespace mln
{

  namespace my
  {

    template <typename N_t, typename G>
    void update_gN(const N_t& N, G& gN)
    {
      for (unsigned g = 0; g < 256; ++g)
	if (N[g].nsites() != 0)
	  {
	    gN = g;
	    return;
	  }
      // if N is empty, gN is the max value.
      gN = 255;
    }


    template <typename N_t>
    void print_N(const N_t& N)
    {
      for (unsigned i = 0; i < 256; ++i)
	{
	  if (N[i].nsites() == 0)
	    continue;
	  std::cout << i << ": " << N[i] << std::endl;
	}
    }

    template <typename N_t>
    void clear_N(N_t& N)
    {
      for (unsigned i = 0; i < 256; ++i)
	N[i].clear();
    }


    
    template <typename T>
    image2d<T> enlarge(const image2d<T>& input, unsigned coef)
    {
      unsigned
	nrows_ = coef * geom::nrows(input),
	ncols_ = coef * geom::ncols(input);
      image2d<T> output(nrows_, ncols_);
      for (int row = 0; row < nrows_; ++row)
	for (int col = 0; col < ncols_; ++col)
	  output.at(row, col) = mln::opt::at(input, row / coef, col / coef);
      return output;
    }


    template <typename I>
    void save(const I& is, const std::string& name = "")
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
	case 3: // A
	  temp(p) = literal::blue;
	  break;
	}

      if (name == "")
	{
	  std::stringstream filename;
	  filename << "./temp_" << ++counter << ".ppm";
	  io::ppm::save(my::enlarge(temp, 10), filename.str());
	}
      else
	io::ppm::save(temp, name);
    }


    template <typename I, typename Nbh>
    void fllt(const Image<I>& input_, const Neighborhood<Nbh>& nbh_)
    {
      const I& input = exact(input_);
      const Nbh& nbh = exact(nbh_);

      unsigned l = 0, l_max;
      mln_ch_value(I, unsigned) reg_min = labeling::regional_minima(input, nbh, l_max);
      std::vector<bool> tag(l_max + 1, false);
      tag[0] = true;

      // Variables.
      I u = mln::clone(input);
      mln_point(I) x0;
      mln_value(I) g, gN;
      mln_fwd_piter(I) p(input.domain());
      p.start();

//       image2d<unsigned char> is(input.domain());
//       const unsigned in_R = 1, in_N = 2, in_A = 3, in_O = 0;
//       level::fill(is, in_O);
      
      image2d<bool> deja_vu(input.domain());
      level::fill(deja_vu, false);

      typedef p_array<mln_point(I)> arr_t;
      arr_t A;
      arr_t N[256];
      accu::bbox<mln_point(I)> N_box;

      unsigned n_step_1 = 0, n_step_3 = 0;
      
      // Step 1.
    step_1:
      {
	while (tag[reg_min(p)] && p.is_valid())
	  p.next();
	if (p.is_valid())
	  tag[reg_min(p)] = true; // To be processed.
	else
	  goto the_end;
	
	++n_step_1;
	x0 = p;
	g = input(x0);
      }

      // Step 2.
    step_2:
      {
	// R <- 0 and N <- 0
	if (N_box.is_valid() != 0)
	  {
// 	    level::fill((is | N_box.to_result()).rw(), in_O);
	    level::fill(deja_vu | N_box.to_result()), false);
	  }
	clear_N(N);
	N_box.init();

	// A <- { x0 }
	A.clear();
	A.append(x0);
// 	is(x0) = in_A;
	deja_vu(x0) = true;
      }
      
      // Step 3.
    step_3:
      {
	++n_step_3;

	mln_piter(arr_t) a(A);
	mln_niter(Nbh) x(nbh, a);


//   	my::save(is);


	// R <- R U A
	if (A.nsites() == 0)
	  goto the_end;

// 	for_all(a)
// 	{
// 	  mln_invariant(is(a) == in_A);
// 	  is(a) = in_R;
// 	}

	// N <- N U { x in nbh of A and not in R }
	for_all(a)
	  for_all(x)
	  {
// 	    if (u.has(x))
// 	      mln_invariant(is(x) != in_O || deja_vu(x) == false);

  	    //if (u.has(x) && is(x) == in_O)
  	    if (u.has(x) && !deja_vu(x))
	      {
		N[u(x)].append(x);
// 		is(x) = in_N;
		N_box.take(x);
		deja_vu(x) = true;
	      }
	  }
	// gN = min u(x) for all x in N
	update_gN(N, gN);

	// FIXME: update the number of CC of the border of R
      }

      // Step 4.
    step_4:
      {
	// a)
	if (g < gN)
	  {
	    g = gN;
	    // FIXME: DO the hole thing.
	    A = N[g];
// 	    mln_piter(arr_t) a(A);
// 	    for_all(a)
// 	    {
// 	      mln_invariant(is(a) == in_N);
// 	      is(a) = in_A;
// 	      // N_box is not re-computed so that we save time;
// 	      // N_box is always growing while looping from step 3.
// 	    }
	    N[g].clear();
	    goto step_3;
	  }
	// b)
	else if (g == gN)
	  {
	    A = N[g];
// 	    mln_piter(arr_t) a(A);
// 	    for_all(a)
// 	    {
// 	      mln_invariant(is(a) == in_N);
// 	      is(a) = in_A;
// 	    }
	    N[g].clear();
	    goto step_3;
	  }
	// c)
	else
	  {
	    // Here deja_vu is (R U N U A)
	    // we only want R

	    // yet A is empty (cause included in R)
	    //   so this test is ok: mln_invariant((is | in_A).nsites() == 0);

	    for (unsigned i = 0; i < 256; ++i)
	      if (N[i].nsites())
		level::fill(deja_vu | N[i]), false);
// 		{
// 		  mln_piter(arr_t) p(N[i]);
// 		  for_all(p)
// 		    deja_vu(p) = false;
// 		}

// 	    mln_invariant(deja_vu == ((pw::value(is) == pw::cst(in_R)) | input.domain()));

// 	    mln_piter(I) r(N_box);
// 	    for_all(r)
// 	      if (is(r) == in_R)
// 		u(r) = g;

	    mln_piter(I) r(N_box);
	    for_all(r)
	      if (deja_vu(r))
		u(r) = g;

	    goto step_1;
	  }
      }

    the_end:
      std::cout << n_step_1 << ' ' << n_step_3 << std::endl;

    }

  } // end of namespace mln::my

} // end of namespace mln


int main(int argc, char* argv[])
{
  if (argc != 2)
    {
      std::cerr << "usage: " << argv[0] << " filename" << std::endl;
      return 1;
    }

  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;
  io::pgm::load(lena, argv[1]);

  my::fllt(lena, c4());
  io::pgm::save(lena, "./out.pgm");

}
