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

/// \file sandbox/geraud/tufa/steps.cc

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/tree/data.hh>

#include <mln/accu/count.hh>
#include <set>

#include <mln/labeling/regional_minima.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/level/fill.hh>
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


  template <typename I, typename A, typename N>
  void run_run(const I& f, const A& a, const N& nbh)
  {
    typedef p_array<mln_psite(I)> S;
    S s = level::sort_psites_increasing(a);
    // s maps increasing attributes.

    mln_ch_value(I, mln_site(I)) par;
    mln_ch_value(I, bool) deja_vu;
    mln_ch_value(I, std::set<unsigned>) labels;
    unsigned nbassins, current_n;


    // Initialization.
    {
      mln_piter(A) p(f.domain());

      // parent
      initialize(par, f);
      for_all(p)
	par(p) = p;

      // deja_vu
      initialize(deja_vu, f);
      level::fill(deja_vu, false);

      // labels
      mln_ch_value(I, unsigned) regmin = labeling::regional_minima(a, nbh,
								   nbassins);
      initialize(labels, f);
      for_all(p)
	if (regmin(p) != 0) // p in a reg min of the attribute image
	  labels(p).insert(regmin(p));
    }

    current_n = nbassins;

    // First pass.
    {
      mln_site(I) r;
      mln_fwd_piter(S) p(s);
      mln_niter(N) n(nbh, p);
      for_all(p)
	{
	  for_all(n)
	    if (a.domain().has(n) && deja_vu(n))
	      {
		r = find_root__(par, n);
		if (r != p)
		  {
		    par(r) = p; // Union.

		    if (labels(r).empty())
		      // No-op.
		      ;
		    else
		      if (labels(p).empty())
			labels(p) = labels(r);
		      else
			if (labels(p) == labels(r))
			  // No-op.
			  ;
			else
			  {
			    labels(p).insert(labels(r).begin(), labels(r).end());
			    --current_n;
			  }		    
		  }
	      }
	  deja_vu(p) = true;
	}
    }

    std::cout << std::endl;
    std::cout << "end = " << current_n << std::endl;
  }


} // mln




int main(int, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  int_u8 n;

  typedef image2d<int_u8> I;
  I f;
  io::pgm::load(f, argv[1]);

  typedef p_array<point2d> S;
  S s = level::sort_psites_decreasing(f);

  // Children go towards lower levels so leafs are regional minima.
  // We get a min-tree so that we can perform morphological closings. 

  morpho::tree::data<I,S> t(f, s, c4());
  accu::count< util::pix<I> > attr;

  image2d<unsigned> a = morpho::tree::compute_attribute_image(attr, t);
  run_run(f, a, c4());

}
