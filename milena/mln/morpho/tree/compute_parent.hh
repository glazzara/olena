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

      /// Compute a tree with a parent relationship between sites.
      ///
      /// Warning: \p s translates the ordering related to the
      /// "natural" childhood relationship.  The parenthood is thus
      /// inverted w.r.t. to \p s.
      ///
      /// It is very convenient since all processing upon the parent
      /// tree are performed following \p s (in the default "forward"
      /// way).
      ///
      /// FIXME: Put it more clearly...
      ///
      /// The parent result image verifies: \n
      /// - p is root iff parent(p) == p \n
      /// - p is a node iff either p is root or f(parent(p)) != f(p).

      template <typename I, typename N, typename S>
      mln_ch_value(I, mln_psite(I))
      compute_parent(const Image<I>& f, const Neighborhood<N>& nbh,
		     const Site_Set<S>& s);



# ifndef MLN_INCLUDE_ONLY


      // Tests.


      namespace internal
      {
      
	template <typename I, typename N, typename S>
	void
	compute_parent_tests(const Image<I>& f_,
			     const Neighborhood<N>& nbh_,
			     const Site_Set<S>& s_)
	{
	  const I& f   = exact(f_);
	  const N& nbh = exact(nbh_);
	  const S& s   = exact(s_);

	  mln_precondition(f.is_valid());
	  mln_precondition(nbh.is_valid());
	  mln_precondition(s == f.domain());

	  (void) f;
	  (void) nbh;
	  (void) s;
	}


	// Z-Find-Root.

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

      }  // end of namespace mln::morpho::tree::internal



      // Implementations.


      namespace impl
      {

	namespace generic
	{

	  template <typename I, typename N, typename S>
	  inline
	  mln_ch_value(I, mln_psite(I))
	  compute_parent(const Image<I>& f_,
			 const Neighborhood<N>& nbh_,
			 const Site_Set<S>& s_)
	  {
	    trace::entering("morpho::tree::impl::generic::compute_parent");

	    typedef mln_psite(I) P;

	    const I& f   = exact(f_);
	    const N& nbh = exact(nbh_);
	    const S& s   = exact(s_);

	    // Tests.
	    internal::compute_parent_tests(f, nbh, s);

	    // Auxiliary data.
	    mln_ch_value(I, bool) deja_vu;
	    mln_ch_value(I, P) parent;
	    mln_ch_value(I, P) zpar;

	    initialize(deja_vu, f);
	    initialize(parent, f);
	    initialize(zpar, f);

	    // Initialization.
	    data::fill(deja_vu, false);

	    // Body.
	    mln_bkd_piter(S) p(s);
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

	    trace::exiting("morpho::tree::impl::generic::compute_parent");
	    return parent;
	  }

	}  // end of namespace mln::morpho::tree::impl::generic

      }  // end of namespace mln::morpho::tree::impl


      // Dispatch.

      namespace internal
      {

	template <typename I, typename N, typename S>
	inline
	mln_ch_value(I, mln_psite(I))
	compute_parent_dispatch(const Image<I>& f,
				const Neighborhood<N>& nbh,
				const Site_Set<S>& s)
	{
	  return impl::generic::compute_parent(f, nbh, s);
	}

      }  // end of namespace mln::morpho::tree::internal


      // Facade.

      template <typename I, typename N, typename S>
      inline
      mln_ch_value(I, mln_psite(I))
      compute_parent(const Image<I>& f, const Neighborhood<N>& nbh,
		     const Site_Set<S>& s)
      {
	trace::entering("morpho::tree::compute_parent");

	internal::compute_parent_tests(f, nbh, s);

	mln_ch_value(I, mln_psite(I)) output;
	output = internal::compute_parent_dispatch(f, nbh, s);

	trace::exiting("morpho::tree::compute_parent");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    }  // end of namespace mln::morpho::tree

  }  // end of namespace mln::morpho

}  // end of namespace mln


#endif // ! MLN_MORPHO_TREE_COMPUTE_PARENT_HH
