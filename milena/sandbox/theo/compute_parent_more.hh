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

#ifndef MLN_MORPHO_TREE_COMPUTE_PARENT_HH
# define MLN_MORPHO_TREE_COMPUTE_PARENT_HH

/// \file mln/morpho/tree/compute_parent.hh
///
/// Compute a canonized tree from an image.
///
/// \todo Specialize for low quant (and try fastest).

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>


namespace mln
{

  namespace morpho
  {

    namespace tree
    {


      template <typename T>
      inline
      mln_psite(T)
      zfind_root(T& zpar, const mln_psite(T)& x)
      {
	mlc_equal(mln_value(T), mln_psite(T))::check();
	if (zpar(x) == x)
	  return x;
	else
	  return zpar(x) = zfind_root(zpar, zpar(x));
      }



      template <typename I, typename N, typename S>
      inline
      mln_ch_value(I, mln_psite(I))
	compute_parent_more(const Image<I>& f_,
			    const Neighborhood<N>& nbh_,
			    const Site_Set<S>& s_)
      {
	typedef mln_psite(I) P;

	const I& f   = exact(f_);
	const N& nbh = exact(nbh_);
	const S& s   = exact(s_);

	// Auxiliary data.
	mln_ch_value(I, bool) deja_vu;
	mln_ch_value(I, P) parent;
	mln_ch_value(I, P) zpar;

	initialize(deja_vu, f);
	initialize(parent, f);
	initialize(zpar, f);

	typedef mln_vset(S) Vs;
	typedef mln_pset(S) Ps;
	S roots;

	// Initialization.
	data::fill(deja_vu, false);
	unsigned nnodes = 0;

	// Body.
	mln_bkd_viter(Vs) v(s.values());
	for_all(v)
	{
	  if (s(v).is_empty())
	    continue;

	  mln_bkd_piter(Ps) p(s(v));
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	  {
	    // Make-Set.
	    parent(p) = p;
	    zpar(p) = p;
	    
	    for_all(n)
	      if (f.domain().has(n) && deja_vu(n))
		{
		  // Do-Union.
		  P r = internal::zfind_root(zpar, n);
		  if (r != p)
		    {
		      parent(r) = p;
		      zpar(r) = p;
		    }
		}
	    deja_vu(p) = true;
	  }


	  /*

	  // Canonization on the-fly.
	  {
	    // Intra-v:
	    mln_fwd_piter(Ps) p(s(v));
	    for_all(p)
	      parent(p) = parent(parent(p));

	    // Inter "previous v and current v":
	    mln_piter(Ps) r(v_roots);
	    for_all(r)
	      parent(r) = parent(parent(r));
	    v_roots.clear();

	    {
	      // New roots:
	      mln_piter(Ps) p(s(v));
	      for_all(p)
		if (parent(p) == p)
		  v_roots.insert(p);
	      nnodes += v_roots.nsites();
	    }
	  }

	  */

	  // Storing roots.
	  {
	    mln_piter(Ps) p(s(v));
	    for_all(p)
	      if (parent(p) == p)
		roots(v).insert(p);
	    nnodes += roots(v).nsites();
	  }

	} // end of "for_all(v)"


	// Canonization.
	{
	  mln_fwd_piter(S) p(s);
	  for_all(p)
	  {
	    P q = parent(p);
	    if (f(parent(q)) == f(q))
	      parent(p) = parent(q);
	  }
	}

	std::cout << "roots = " << roots << std::endl;
	std::cout << "nnodes = " << nnodes << std::endl;


	return parent;
      }


    }  // end of namespace mln::morpho::tree

  }  // end of namespace mln::morpho

}  // end of namespace mln


#endif // ! MLN_MORPHO_TREE_COMPUTE_PARENT_HH
