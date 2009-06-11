// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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
#include <mln/data/sort_psites.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/tree/data.hh>

#include <mln/accu/math/count.hh>

#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/closing_area.hh>
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


  template <typename I, typename A, typename N>
  mln_concrete(I) filtering(const I& f, const A& a, const N& nbh, mln_value(A) lambda)
  {
    typedef p_array<mln_psite(I)> S;
    S s = data::sort_psites_increasing(a);

    // s maps increasing attributes.

    mln_concrete(I) out;
    initialize(out, f);

    mln_ch_value(I, mln_site(I)) par;
    mln_ch_value(I, bool) deja_vu;
    {
      initialize(par, f);
      mln_piter(A) p(par.domain());
      for_all(p)
	par(p) = p;
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
	  for_all(n)
	    if (a.domain().has(n) && deja_vu(n))
	      {
		r = find_root__(par, n);
		if (r != p)
		  if (f(r) == f(p) || a(r) < lambda)
		    par(r) = p; // Union.
	      }
	  deja_vu(p) = true;
	}
    }

    // Second pass.
    {
      mln_bkd_piter(S) p(s);
      for_all(p)
	if (par(p) == p)
	  out(p) = f(p);
	else
	  out(p) = out(par(p));
    }
    return out;
  }


} // mln




int main()
{
  using namespace mln;
  using value::int_u8;

  int_u8 n;

  typedef image2d<int_u8> I;
  I f;
  io::pgm::load(f, "../../../img/fly.pgm");
  debug::println("f =", f);

  debug::println("ref =", morpho::closing_area(f, c4(), 10));

  typedef p_array<point2d> S;
  S s = data::sort_psites_decreasing(f);

  // Children go towards lower levels so leafs are regional minima.
  // We get a min-tree so that we can perform morphological closings. 

  morpho::tree::data<I,S> t(f, s, c4());
  accu::math::count< util::pix<I> > attr;

  image2d<unsigned> a = morpho::tree::compute_attribute_image(attr, t);
  I g = filtering(f, a, c4(), 10);
  debug::println("g =", g);

  {
    S s = data::sort_psites_decreasing(g);
    morpho::tree::data<I,S> t(g, s, c4());
    image2d<unsigned> a_g = morpho::tree::compute_attribute_image(attr, t);
    debug::println("a(f) =", a);
    debug::println("a(g) =", a_g);
  }

}
