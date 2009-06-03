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

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/io/pgm/load.hh>

#include <mln/debug/println.hh>
#include <mln/debug/iota.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/data/fill.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

#include <mln/accu/volume.hh>
#include <mln/accu/count.hh>


namespace mln
{

  template <typename I>
  inline
  mln_psite(I) find_root__(I& parent, const mln_psite(I)& x)
  {
    if (parent(x) == x)
      return x;
    else
      return parent(x) = find_root__(parent, parent(x));
  }


  template <typename A, typename I, typename N>
  mln_concrete(I)
  n_cmpt(A a, const I& f, const N& nbh, unsigned lambda)
  {
    // Compute attribute image.
    //-------------------------

    typedef p_array<mln_psite(I)> S;
    typedef mln_ch_value(I, mln_result(A)) Attr;

    S s = data::sort_psites_decreasing(f);
    morpho::tree::data<I,S> t(f, s, nbh);
    Attr attr = morpho::tree::compute_attribute_image(a, t);
    s = data::sort_psites_increasing(attr);
    debug::println("attr =", attr);


    // Compute regional minima of attribute image. 
    //--------------------------------------------

    unsigned label;
    mln_ch_value(I, unsigned) min_v = labeling::regional_minima(attr, nbh, label);
    debug::println("regmin =", min_v);

    // number of minima
    unsigned cmpts = label;
    if (lambda > cmpts)
      std::cout << "Warning: lambda value is too high!" << std::endl;

    std::cout << cmpts << std::endl;


    // two pass algo
    //--------------

    // init fused image
    mln_ch_value(I, bool) fused;
    initialize(fused, attr);
    mln::data::fill(fused, false);

    // prepare union find
    typedef mln_psite(I) P;

    //deja_vu
    mln_ch_value(I, bool) deja_vu;
    initialize(deja_vu, f);
    data::fill(deja_vu, false);

    //parent
    mln_ch_value(I, P) parent(attr.domain());
    {
      mln_fwd_piter(S) p(s);
      for_all(p)
        {
          parent(p) = p;

          // Mandatory since we propagate fused
          if (min_v(p) != 0) // p in a reg min of the attribute image
            fused(p) = true; // ok
        }
    }

    // UNION FIND ON ATTR
    mln_fwd_piter(S) p(s);
    mln_niter(N) n(nbh, p);
    for_all(p)
      {
        // std::cout << p << std::endl;
        for_all(n)
	  {
	    if (attr.domain().has(n) && deja_vu(n))
	      {
		P r = find_root__(parent, n);
		if (r != p)
		  {
		    // std::cout << "neighb: " << n << std::endl;
		    // std::cout << "v(r): " << attr(r) << " v(p): " 
		    //           << attr(p) << " f(p): " << fused(p) << std::endl;

		    //if (attr(r) != attr(p)) // r and p have different attrs
		    // This check was wrong.
		    // you just fused with a minima.
		    // then you fuse with a neighbor already fused that has the same attr as you
		    // a)  1 50 1
		    //     2 2  2
		    // we don't have ( attr(r) == attr(p) ) => not fused(r)
		    // even if r and p are not minima

		    // problem :
		    // - when fusing minima    : same level, both fused. NOT DEC
		    // - when fusing cmpts (a) : same level, both fused. DEC

		    // One cmpt less if

// 		    if (fused(r) && min_v(p) == 0) // p is not a minima
// 		      if (fused(p)) // p already belong to a cmpt (fused for an another n)
// 			if (cmpts >= lambda) // union is still allowed

		    if (fused(r) && min_v(p) == 0 // p is not a minima
			&& fused(p) // p already belong to a cmpt (fused for an another n)
			&& cmpts >= lambda) // union is still allowed
			  {
			    --cmpts;
			    // std::cout << "dec" << std::endl;
			  }

		    mln_invariant(fused(r) || attr(r) == attr(p));

		    // Union made if
		    if (cmpts >= lambda || // union is still allowed or
			! fused(r) || // r not fused or
			! fused(p) || // p not fused or
			min_v(p) != 0) // p is a minima
		      {
			// std::cout << "fusing " << p << " with " << n << std::endl;
			parent(r) = p;

			// This test is mandatory. Sometimes (--_) points are fused
			// tough none of these points belongs to a component (attached
			// to a local minima).  In this case fused(p) must not be true
			// since it can be fused again without removing a component.
			// looking if r is fused should be enough.
			// This test force minima to be initialized fused.
			if (fused(r))
			  fused(p) = true;


			// If I try to fuse with something never fused I am on a plateau.
			// not fused(r) => ( attr(r) == attr(p) )
			mln_invariant(fused(r) || attr(r) == attr(p));
			// fused(r) and ( attr(r) == attr(p) ) happen on minima plateau.

			// fused(n) = true; // useless ? probably yes because when we
			// want to know that we really fuse component, we look at
			// fused(r) not n.
			// fused(n) is not an invariant: --_. And it is ok (I think).
			// We don't have to retro-propagate fused.


			// std::cout << "attr " << attr(p) << " - " << cmpts << std::endl;
			//debug::println(fused);
		      }
		  }
	      }
	  }
        deja_vu(p) = true;
      }

    mln_ch_value(I,value::int_u<16>) iota(f.domain());
    debug::iota(iota);

    std::cout << std::endl;
    std::cout << "cmpts: " << cmpts << std::endl;

    // second pass
    mln_concrete(I) output;
    initialize(output, f);
    {
      mln_bkd_piter(S) p(s);
      for_all(p)
	if (parent(p) == p) // p is root.
	  output(p) = iota(p);
	else
	  output(p) = output(parent(p));
    }

    return output;
  }

} // end of namespace mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm n" << std::endl;
  std::cerr << "n: expected number of basins" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  typedef image2d<int_u8> I;
  I f;
  io::pgm::load(f, argv[1]);
  debug::println("f =", f);

  unsigned n = std::atoi(argv[2]);

  accu::count< util::pix<I> > acc;
  // accu::volume<I> acc;

  image2d<int_u8> g = n_cmpt(acc, f, c4(), n + 1);
  debug::println(g);
}
