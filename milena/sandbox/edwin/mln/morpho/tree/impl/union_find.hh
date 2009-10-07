// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_IMPL_UNION_FIND_HH
# define MLN_MORPHO_TREE_IMPL_UNION_FIND_HH

# include <mln/data/sort_psites.hh>
# include <mln/util/ctree/ctree.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      namespace impl
      {

	template <typename T, typename I, typename N>
	util::ctree::ctree<I>
	union_find(const tag::tree::tree_t<T>&,
		   const Image<I>& f,
		   const Neighborhood<N>& nbh);
      }


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename I>
	p_array<mln_psite(I)>
	inline
	sort_sites_set(const tag::tree::max_t&, const I& f)
	{
	  p_array<mln_psite(I)> data =
	    data::sort_psites_increasing(f);
	  return data;
	}

	template <typename I>
	p_array<mln_psite(I)>
	inline
	sort_sites_set(const tag::tree::min_t&, const I& f)
	{
	  p_array<mln_psite(I)> data =
	    data::sort_psites_decreasing(f);
	  return data;
	}

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

      }


      namespace impl
      {

	namespace generic
	{

	  template <typename T, typename I, typename N>
	  util::ctree::ctree<I>
	  union_find(const tag::tree::tree_t<T>&,
		     const Image<I>& f_,
		     const Neighborhood<N>& nbh_)
	  {
	    trace::entering("morpho::tree::impl::generic::union_find");

	    typedef mln_psite(I) P;
	    typedef p_array<mln_psite(I)> S;

	    const I& f   = exact(f_);
	    const N& nbh = exact(nbh_);
	    S s = internal::sort_sites_set(T (), f);

	    // Auxiliary data.
	    mln_ch_value(I, bool) deja_vu;
	    mln_ch_value(I, P) parent;
	    mln_ch_value(I, unsigned) area;
	    mln_ch_value(I, P) zpar;
	    unsigned nb_nodes = s.nsites();

	    initialize(deja_vu, f);
	    initialize(parent, f);
	    initialize(area, f);
	    initialize(zpar, f);

	    // Initialization.
	    data::fill(deja_vu, false);
	    data::fill(area, 0);

	    // Body.
	    mln_bkd_piter(S) p(s); // Backward.
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
			area(p) += area(r);
			if (f(r) != f(p))
			  ++area(p);
			else
			  --nb_nodes;
		      }
		  }
	      deja_vu(p) = true;
	    }

	    util::ctree::ctree<I> tree(f, s, parent, area, nb_nodes);

	    trace::exiting("morpho::tree::impl::generic::union_find");
	    return tree;
	  }

	}

      }
# endif // ! MLN_INCLUDE_ONLY
    }

  }

}
#endif // !MLN_MORPHO_TREE_IMPL_UNION_FIND_HH
