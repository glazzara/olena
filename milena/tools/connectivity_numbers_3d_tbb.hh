// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \brief Elements to compute 3D connectivity numbers in parallel,
/// using Intel Threading Building Blocks.

#ifndef TOOLS_CONNECTIVITY_NUMBERS_3D_TBB_HH
# define TOOLS_CONNECTIVITY_NUMBERS_3D_TBB_HH

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

#include "connectivity_numbers_3d.hh"


/*--------------------------------------------------.
| Parallel computation of 3D connectivity numbers.  |
`--------------------------------------------------*/

// The core of the loop within connectivity_numbers_3d_tbb below.
template <typename F>
struct connectivity_numbers_3d_tbb_range
{
  typedef mln::neighb3d N;

  connectivity_numbers_3d_tbb_range(F f, const N& nbh, conn_numbers_t& numbers)
    : f_(f), nbh_(nbh), numbers_(numbers)
  {
  }

  void operator()(const tbb::blocked_range<config_3d_t>& r) const
  {
    using namespace mln;

    typedef image3d<bool> I;
    // B must be a model of mln::Box.
    typedef mln_domain(I) B;
    typedef mln_psite(I) P;

    B b = make::box3d(-1,-1,-1, 1,1,1);
    I ima(b, 0);
    P p(0, 0, 0);

    const unsigned dim = 3;
    const unsigned max_nneighbs = mlc_pow_int(3, dim) - 1;
    const unsigned nconfigs = mlc_pow_int(2, max_nneighbs);

    for (config_3d_t i = r.begin(); i != r.end(); ++i)
      {
	// Create the local i-th configuration around P.
	data::fill(ima, false);
	config_3d_t tmp = i;
	mln_fwd_niter_(N) n(nbh_, p);
	for_all(n)
	{
	  if (tmp % 2)
	    ima(n) = true;
	  tmp = tmp >> 1;
	}
	numbers_[i] = f_(ima);
      }
  }

private:
  F f_;
  // NBH is copied (not taken as a reference) to avoid thread-unsafe
  // accesses to this neighborhood (see the long explanation in
  // connectivity_numbers_3d.hh).
  N nbh_;
  conn_numbers_t& numbers_;
};

/// Canevas to compute 3D connectivity numbers (in parallel).
///
/// \param f  A function computing the connectivity number for each
///           configuration (as a 3x3x3 3D image).
template <typename F>
conn_numbers_t
connectivity_numbers_3d_tbb(F f)
{
  // FIXME: Code duplicated from connectivity_numbers_3d_tbb_range.
  const unsigned dim = 3;
  const unsigned max_nneighbs = mlc_pow_int(3, dim) - 1;
  const unsigned nconfigs = mlc_pow_int(2, max_nneighbs);

  typedef mln::neighb3d N;
  N nbh = mln::c26();
  conn_numbers_t numbers(nconfigs, 0);
  tbb::parallel_for(tbb::blocked_range<config_3d_t>(0, nconfigs),
		    connectivity_numbers_3d_tbb_range<F>(f, nbh, numbers));
  return numbers;
}


/*-------------------------------------------------------------.
| (6, 26) configurations: 6-connected foreground, 26-connected |
| background.                                                  |
`-------------------------------------------------------------*/

conn_numbers_t
connectivity_numbers_3d_tbb__6_26()
{
  return connectivity_numbers_3d_tbb(connectivity_number_3d__6_26_one);
}


/*-------------------------------------------------------------.
| (26, 6) configurations: 26-connected foreground, 6-connected |
| background.                                                  |
`-------------------------------------------------------------*/

conn_numbers_t
connectivity_numbers_3d_tbb__26_6()
{
  return connectivity_numbers_3d_tbb(connectivity_number_3d__26_6_one);
}


/*---------------------------------------------------------------.
| (6+, 18) configurations: 6+-connected foreground, 18-connected |
| background.                                                    |
`---------------------------------------------------------------*/

conn_numbers_t
connectivity_numbers_3d_tbb__6p_18()
{
  return connectivity_numbers_3d_tbb(connectivity_number_3d__6p_18_one);
}


/*---------------------------------------------------------------.
| (18, 6+) configurations: 18-connected foreground, 6+-connected |
| background.                                                    |
`---------------------------------------------------------------*/

conn_numbers_t
connectivity_numbers_3d_tbb__18_6p()
{
  return connectivity_numbers_3d_tbb(connectivity_number_3d__18_6p_one);
}

#endif // ! TOOLS_CONNECTIVITY_NUMBERS_3D_TBB_HH
