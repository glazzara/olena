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

#include <mln/core/image/image3d.hh>
#include <mln/core/alias/neighb3d.hh>
#include <mln/make/box3d.hh>

#include "tools/connectivity_numbers_3d.hh"

// To avoid heavy recompilation times due to the huge size of the
// look-up table, only fetch the table's declaration.  Its
// implementation is compiled separately and linked to this program.
#define MLN_INCLUDE_ONLY
# include "tools/simple_point3d_lut_26_6.hh"
#undef MLN_INCLUDE_ONLY


int
main()
{
  using namespace mln;

  // FIXME: This part is very similar to
  // milena/tests/topo/connectivity_numbers_2d.cc and
  // milena/tools/connectivity_numbers_3d.hh.  Factor.

  typedef image3d<bool> I;
  // B must be a model of mln::Box.
  typedef mln_domain_(I) B;
  typedef mln_psite_(I) P;

  B b = make::box3d(-1,-1,-1, 1,1,1);
  I fg_ima(b, 0);
  I bg_ima(b, 0);
  P p(0, 0, 0);

  const unsigned dim = 3;
  const unsigned max_nneighbs = mlc_pow_int(3, dim) - 1;
  const unsigned nconfigs = mlc_pow_int(2, max_nneighbs);

  typedef neighb3d N;
  N nbh = c26();

  int exit_status = EXIT_SUCCESS;

  for (config_3d_t i = 0; i < nconfigs; ++i)
  {
    /* Create the local i-th configuration around P.
	
       Note that the value corresponding to P is always `false', to
       prevent the connection of two components through P.  */
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

    // Compute the simple point criterion for (26, 6)-connectivity
    conn_number_t conn_number_fg = connectivity_number_3d__26_6_one(fg_ima);
    conn_number_t conn_number_bg = connectivity_number_3d__6_26_one(bg_ima);
    bool conn_number_computed = conn_number_fg == 1 &&conn_number_bg == 1;


    // Retrieve the simple point criterion for (26, 6)-connectivity
    // number using the LUT.
    conn_number_t conn_number_from_lut;
    {
      // ``Unpack'' the i-th value.

      // FIXME: Redundant with some code from
      // mln/topo/is_simple_point3d.hh.

      size_t pos = i;

      /* `simple_point3d_lut_26_6' is a packed bit array.  We must
	 first retrieve the cell ``containing'' the index `pos'.  */
      size_t cell = pos / 8;
      /* FIXME: We use hard-coded neighborhoods here instead of using
	 nbh_fg_ and nbh_bg_ (for this first test version).  */
      unsigned char pack = mln::topo::simple_point3d_lut_26_6[cell];

      /* Compute the position of the neighborhood configuration within
	 the packed set of valued and create a mask corresponding to
	 this position (offset).  */
      size_t offset = pos % 8;
      unsigned char mask = 1 << offset;

      /* If the pack contains a 1 at position `offset', then `pack &
	 mask' is a non-null value.  Otherwise it is zero.  We can
	 therefore use it to create a boolean return value.  */
      bool result = pack & mask;
      conn_number_from_lut = result;
    }

    // Compare them.
    if (conn_number_computed != conn_number_from_lut)
      {
	std::cout
	  << "Inconsistency for the " << i << "-th configuration: "
	  << conn_number_computed << " != " << conn_number_from_lut
	  << std::endl;
	exit_status = EXIT_FAILURE;
      }
  }

  return exit_status;
}
