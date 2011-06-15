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

/// \brief Elements to compute a look-up table (LUT) showing whether a
/// 3D point is simple or not.

#ifndef TOOLS_SIMPLE_POINT3D_LUT_HH
# define TOOLS_SIMPLE_POINT3D_LUT_HH

#include "connectivity_numbers_3d.hh"


/// Type of the look-up table (LUT).
typedef std::vector<bool> simple_point_lut_t;


/*------------------------------------------------------------------.
| Sequential computation of a look-up table (LUT) showing whether a |
| 3D point is simple or not.                                        |
`------------------------------------------------------------------*/

/// Canevas to compute a look-up table (LUT) showing whether a 3D
/// point is simple or not.
///
/// \param fg_fun  A function computing the connectivity number for
///                each configuration (as a 3x3x3 3D image) of the
///                foreground.
/// \param bg_fun  A function computing the connectivity number for
///                each configuration (as a 3x3x3 3D image) of the
///                background.
template <typename FG, typename BG>
simple_point_lut_t
simple_point3d_lut(FG fg_fun, BG bg_fun)
{
  using namespace mln;

  typedef image3d<bool> I;
  // B must be a model of mln::Box.
  typedef mln_domain(I) B;
  typedef mln_psite(I) P;

  B b = make::box3d(-1,-1,-1, 1,1,1);
  I fg_ima(b, 0);
  I bg_ima(b, 0);
  P p(0, 0, 0);

  const unsigned dim = 3;
  const unsigned max_nneighbs = mlc_pow_int(3, dim) - 1;
  const unsigned nconfigs = mlc_pow_int(2, max_nneighbs);

  simple_point_lut_t simple_point_lut(nconfigs, false);

  typedef neighb3d N;
  N nbh = c26();

  for (config_3d_t i = 0; i < nconfigs; ++i)
    {
      /* Create the local i-th configuration around P.

	 Note that the value corresponding to P is always `false',
	 to prevent the connection of two components through P.  */
      data::fill(fg_ima, false);
      data::fill(bg_ima, false);
      config_3d_t tmp = i;
      mln_fwd_niter_(N) n(nbh, p);
      for_all(n)
      {
	if (tmp % 2)
	  fg_ima(n) = true;
	else
	  bg_ima(n) = true;
	tmp = tmp >> 1;
      }
      simple_point_lut[i] = (   fg_fun(fg_ima) == 1
			     && bg_fun(bg_ima) == 1);
    }
  return simple_point_lut;
}


/*-------------------------------------------------------------.
| (6, 26) configurations: 6-connected foreground, 26-connected |
| background.                                                  |
`-------------------------------------------------------------*/

simple_point_lut_t
simple_point3d_lut__6_26()
{
  return simple_point3d_lut(connectivity_number_3d__6_26_one,
			    connectivity_number_3d__26_6_one);
}


/*-------------------------------------------------------------.
| (26, 6) configurations: 26-connected foreground, 6-connected |
| background.                                                  |
`-------------------------------------------------------------*/

simple_point_lut_t
simple_point3d_lut__26_6()
{
  return simple_point3d_lut(connectivity_number_3d__26_6_one,
			    connectivity_number_3d__6_26_one);
}


/*---------------------------------------------------------------.
| (6+, 18) configurations: 6+-connected foreground, 18-connected |
| background.                                                    |
`---------------------------------------------------------------*/

simple_point_lut_t
simple_point3d_lut__6p_18()
{
  return simple_point3d_lut(connectivity_number_3d__6p_18_one,
			    connectivity_number_3d__18_6p_one);
}


/*---------------------------------------------------------------.
| (18, 6+) configurations: 18-connected foreground, 6+-connected |
| background.                                                    |
`---------------------------------------------------------------*/

simple_point_lut_t
simple_point3d_lut__18_6p()
{
  return simple_point3d_lut(connectivity_number_3d__18_6p_one,
			    connectivity_number_3d__6p_18_one);
}

/*----------------------.
| Helpers for drivers.  |
`----------------------*/

void
display_simple_point_lut(const simple_point_lut_t& simple_point_lut)
{
  const unsigned bits_per_byte = 8;

  // Set up the output format.
  std::cout << std::hex << std::setfill ('0');
  std::string prefix = "0x";

  size_t i = 0;
  while (i < simple_point_lut.size())
    {
      mln::value::int_u8 pack = 0;
      mln::value::int_u8 mask = 1;
      for (unsigned j = 0; i < simple_point_lut.size() && j < bits_per_byte;
	   ++j, ++i)
	{
	  if (simple_point_lut[i])
	    pack = pack | mask;
	  mask = mask << 1;
	}

      /* Display.

	 Note that except maybe after for the last iteration of the
	 outer loop, I is always a multiple of 8.  */
      std::cout << prefix << std::setw(2) << pack << ", ";
      if (i % 64  == 0) std::cout << std::endl;
      if (i % 256 == 0) std::cout << std::endl;
    }
}

#endif // ! TOOLS_SIMPLE_POINT3D_LUT_HH
