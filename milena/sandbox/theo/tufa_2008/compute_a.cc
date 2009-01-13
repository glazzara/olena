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

/// \file sandbox/geraud/tufa/compute_a.cc

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/pw/all.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/level/compare.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/labeling/regional_minima.hh>

#include <mln/accu/count.hh>
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



  //-------------------------------   compute_a



  template <typename I, typename N, typename A>
  mln_ch_value(I, mln_result(A))
  compute_a(const I& f, const N& nbh, A, unsigned& n_regmins)
  {
    typedef p_array<mln_psite(I)> S;
    S s = level::sort_psites_increasing(f);
    // s maps increasing attributes.

    mln_ch_value(I, mln_site(I)) par;
    mln_ch_value(I, mln_site(I)) zpar;
    mln_ch_value(I, bool) deja_vu, flag;
    mln_ch_value(I, A) attr;

    n_regmins = f.domain().nsites();


    // Initialization.
    {
      // parent
      initialize(par, f);
      initialize(zpar, f);

      // deja_vu
      initialize(deja_vu, f);
      data::fill(deja_vu, false);

      // flag
      initialize(flag, f);
      data::fill(flag, true);

      // attr
      initialize(attr, f);
    }


    // First Pass.
    {
      mln_site(I) r;
      mln_fwd_piter(S) p(s);
      mln_niter(N) n(nbh, p);
      for_all(p)
	{
	  // Make-Set.
	  par(p) = p;
	  zpar(p) = p;
	  attr(p).take_as_init(p);

	  for_all(n)
	    if (f.domain().has(n) && deja_vu(n))
	      {
		r = find_root__(zpar, n);
		if (r != p)
		  {
		    // Fully compressed union.
		    zpar(r) = p;
		    attr(p).take(attr(r));

		    if (f(r) == f(p))
		      {
			// Weak-Union; only for flat zones.
			par(r) = p;

			if (flag(p) == false && flag(r) == false)
			  {
			    // Two non-reg-min components merge (same flat
			    // zone) so we had an extra invalidation.
			    ++n_regmins;
			  }
			flag(p) = flag(p) && flag(r);
			--n_regmins; // So we get the number of flat zones
			           // minus the non-reg-min flat zones.
		      }
		    else
		      {
			mln_invariant(f(r) < f(p));
			if (flag(p) == true)
			  --n_regmins; // Invalidation.
			flag(p) = false;
		      }
		  }
	      }
	  deja_vu(p) = true;
	}
    } // end of First Pass.


    std::cout << "n reg min = " << n_regmins << std::endl;


    {
      unsigned n = 0;
      mln_fwd_piter(S) p(s);
      for_all(p)
	if (par(p) == p && flag(p))
	  ++n;
      mln_assertion(n == n_regmins);
    }


    // The attr image is not correct on flat zones since there is
    // no back-propagation of the attribute value of the component
    // root.  For instance with f="v v v" we get attr="1 2 3"
    // instead of "3 3 3".  So a finalization is required.

    mln_ch_value(I, mln_result(A)) a;
    initialize(a, f);
    data::paste(attr, a);

    // Finalization.
    {
      mln_bkd_piter(S) p(s); // Reverse.

      unsigned n_non_compressed_par = 0;
      for_all(p)
	{
	  a(p) = a(par(p));
	  if (par(par(p)) != par(p))
	    ++ n_non_compressed_par;
	}
      std::cout << "n_non_compressed_par = " << n_non_compressed_par << std::endl;
    }

    // TODO: compress at least the reg minima!


    {
      image2d<unsigned> regmin;
      initialize(regmin, f);
      {
	unsigned i_regmin = 0;
	mln_bkd_piter(S) p(s);
	for_all(p)
	  {
	    if (par(p) == p)
	      {
		if (flag(p))
		  regmin(p) = ++i_regmin;
		else
		  regmin(p) = 0;
	      }
	    else
	      regmin(p) = regmin(par(p));
	  }
      }

      debug::println("f", f);

      debug::println("flag", flag);
      // We can see that some point are at true for components that
      // are not reg min;  flag is a candidate to be compressed...

      debug::println("regmin", regmin);



      // TODO:

      // On veut tester ici dans quel ordre on voit les
      // reg min lorsque a croit.  Pour tous les points d'un reg min,
      // est-ce que le root est vu en premier ?



      image2d<bool> seen;
      initialize(seen, f);
      data::fill(seen, false);

      s = level::sort_psites_increasing(a);
      mln_bkd_piter(S) p(s);
      for_all(p)
	{
	  if (regmin(p) == 0)
	    continue;
	  // p is in a regional minimum.
	  if (par(p) != p) // A non-root point.
	    {
	      mln_assertion(regmin(par(p)) != 0); // Root in a regional minimum.
	      mln_assertion(regmin(par(p)) == regmin(p)); // and the same as p.
	      mln_assertion(seen(par(p)));
	    }
	  seen(p) = true;
	}
      debug::println(seen);

// 	if (flag(p))
// 	  std::cout << a(p) << ' ' << p << ' ' << (par(p) == p) << std::endl;
      
    }

    return a;
  }



  //-------------------------------   filtering



//   template <typename I, typename A, typename N>
//   mln_concrete(I) filtering(const I& f, const A& a, const N& nbh,
// 			    unsigned n_regmins, unsigned n_wanted)
//   {
//     typedef p_array<mln_psite(I)> S;
//     S s = level::sort_psites_increasing(a);

//     // s maps increasing attributes.

//     mln_concrete(I) out;
//     initialize(out, f);

//     mln_ch_value(I, mln_site(I)) par;
//     mln_ch_value(I, bool) deja_vu, flag;

//     // Initialization.
//     {
//       initialize(par, f);
//       mln_piter(A) p(par.domain());
//       for_all(p)
// 	par(p) = p;
//       initialize(deja_vu, f);
//       data::fill(deja_vu, false);

//       // flag
//       initialize(flag, f);
//       data::fill(flag, true);
//     }


//     int counter = 0;
//     // We are trying to count the number of merges of regional minima...


//     // First Pass.
//     {
//       mln_site(I) r;
//       mln_fwd_piter(S) p(s);
//       mln_niter(N) n(nbh, p);
//       for_all(p)
// 	{
// 	  for_all(n)
// 	    if (a.domain().has(n) && deja_vu(n))
// 	      {
// 		r = find_root__(par, n);
// 		if (r != p)
// 		  if (a(r) == a(p))
// 		    {
// 		      par(r) = p; // Union.
// 		      if (flag(r) == true && flag(p) == true)
// 			--counter;
// 		      flag(p) = flag(p) && flag(r);
// 		    }
// 		  else // a(r) != a(p)
// 		    {
// 		      if (flag(r) == true && flag(p) == true)
// 			++counter;
// 		      mln_invariant(a(p) > a(r));
// 		      flag(p) = false;
// 		    }
// 	      }
// 	  deja_vu(p) = true;
// 	}
//       std::cout << counter << std::endl;
//     }

// //     // Second Pass.
// //     {
// //       mln_bkd_piter(S) p(s);
// //       for_all(p)
// // 	if (par(p) == p)
// // 	  out(p) = f(p);
// // 	else
// // 	  out(p) = out(par(p));
// //     }

//     return out;
//   }




} // mln




int main(int, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;
  I f;
  io::pgm::load(f, argv[1]);
  // debug::println(f);

  accu::count<point2d> area;
  unsigned n_regmins;
  image2d<unsigned> a = compute_a(f, c4(), area, n_regmins);
  // debug::println(a);

//   {
//     // Test of 'n_regmins'.
//     unsigned ref;
//     labeling::regional_minima(f, c4(), ref);
//     mln_assertion(n_regmins == ref);
//   }

//   {
//     // Test of 'a'.
//     typedef p_array<point2d> S;
//     S s = level::sort_psites_decreasing(f);
//     morpho::tree::data<I,S> t(f, s, c4());
//     accu::count< util::pix<I> > area;
//     image2d<unsigned> ref = morpho::tree::compute_attribute_image(area, t);
//     mln_assertion(a == ref);
//   }





//   unsigned n_wanted = 10;
//   I g = filtering(f, a, c4(), n_regmins, n_wanted);

}
