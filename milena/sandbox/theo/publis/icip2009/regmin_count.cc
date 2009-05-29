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

/// \file sandbox/geraud/tufa/regmin_count.cc

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/data/sort_psites.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/labeling/flat_zones.hh>
#include <mln/data/fill.hh>

#include <mln/opt/at.hh>


namespace mln
{

  template <typename I>
  void println_par(const I& par)
  {
    int nr = par.nrows(), nc = par.ncols();
    for (int r = 0; r < nr; ++r)
      {
	for (int c = 0; c < nc; ++c)
	  if (opt::at(par, r,c) == point2d(r,c))
	    std::cout << "(    ) ";
	  else
	    std::cout << opt::at(par, r,c) << ' ';
	std::cout << std::endl;
      }
  }

  template <typename P>
  inline
  mln_value(P) find_root__(P& par, const mln_value(P)& x)
  {
    if (par(x) == x)
      return x;
    else
      return par(x) = find_root__(par, par(x));
  }


  template <typename I, typename N>
  unsigned regmin_count(const I& f, const N& nbh)
  {
    typedef p_array<mln_psite(I)> S;
    S s = data::sort_psites_increasing(f);
    // s maps increasing attributes.

    mln_ch_value(I, mln_site(I)) par;
    mln_ch_value(I, bool) deja_vu, flag;

    unsigned counter = f.domain().nsites();


    // Initialization.
    {
      mln_piter(I) p(f.domain());

      // parent
      initialize(par, f);
      for_all(p)
	par(p) = p;

      // flag
      initialize(flag, f);
      data::fill(flag, true);

      // deja_vu
      initialize(deja_vu, f);
      data::fill(deja_vu, false);
    }

    // First pass.
    {
      mln_site(I) r;
      mln_fwd_piter(S) p(s);
      mln_niter(N) n(nbh, p);
      for_all(p)
	{
	  unsigned loc = 0;
	  for_all(n)
	    if (f.domain().has(n) && deja_vu(n))
	      {
		r = find_root__(par, n);
		if (r != p)
		  {
		    if (f(r) == f(p))
		      {
			par(r) = p; // Union.
			if (flag(p) == false && flag(r) == false)
			  {
			    // Two non-reg-min components merge (same flat
			    // zone) so we had an extra invalidation.
			    ++counter;
			  }
			flag(p) = flag(p) && flag(r);
			--counter; // So we get the number of flat zones
			           // minus the non-reg-min flat zones.
		      }
		    else
		      {
			mln_invariant(f(r) < f(p));
			if (flag(p) == true)
			  {
			    ++loc;
			    --counter; // Invalidation.
			  }
			flag(p) = false;
		      }
		  }
	      }
	  mln_invariant(loc == 0 || loc == 1);
	  deja_vu(p) = true;
	}
    }

    return counter;
  }


} // mln




int main(int, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;
  I f;
  io::pgm::load(f, argv[1]);

  unsigned ref, n = regmin_count(f, c4());
  labeling::regional_minima(f, c4(), ref);
  
  if (n == ref)
    std::cout << "success: n regional minima = " << n << std::endl;
  else
    std::cout << "FAILURE: found = " << n << " v. ref = " << ref << std::endl;
}
