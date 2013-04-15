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

#ifndef MLN_MORPHO_TREE_IMLP_DUAL_UNION_FIND_HH
# define MLN_MORPHO_TREE_IMLP_DUAL_UNION_FIND_HH

///
/// \brief  The generic dual input tree algorithm for high quantized image.
///
/// This implementation is based on tarjan's union method, so that
/// image quantization does not impact on the computation time.
///
/// TODO: Think about how to extend f domain in a more
/// generic way. The actual implementation doubles the size of the
/// first dimension. It implies a boxed domain.
///
/// TODO: Use the less functor. The actual implementation is for max-tree.

# include <mln/data/fill.hh>

# include <mln/geom/nsites.hh>
# include <mln/geom/translate.hh>

# include <mln/morpho/tree/data.hh>

# include <mln/util/timer.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      namespace impl
      {

	namespace generic
	{

	  /// Compute a tree using union-find method.
	  ///
	  /// \param f The original image.
	  /// \param m The connectivity mask.
	  /// \param s_f The sorted site set of \p f
	  /// \param s_m The sorted site set of \p m.
	  /// \param nbh The neighborhood of the mask.
	  ///
	  /// \return The tree structure.
	  ///
	  template <typename I, typename S, typename N>
	  data< I, p_array<mln_psite(I)> >
	  dual_union_find(const Image<I>& f,
			  const Image<I>& m,
			  const Site_Set<S>& s_f,
			  const Site_Set<S>& s_m,
			  const Neighborhood<N>& nbh);

	}  // end of namespace mln::morpho::tree::impl::generic

      } // end of namespace mln::morpho::tree::impl

# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {
	// For benchmark purpose. More than 50% of the time is spent
	// in find_root function.
	static util::timer t_prop;


	template <typename I>
	inline
	mln_psite(I) find_root(I& zpar,
			       const mln_psite(I)& p)
	{
	  if (zpar(p) == p)
	    return p;

	  t_prop.resume();
	  mln_psite(I) x = zpar(p);
	  while (zpar(x) != x)
	      x = zpar(x);

	  mln_psite(I) tmp;
	  for (mln_psite(I) y = p; y != x; y = tmp)
	    {
	      tmp = zpar(y);
	      zpar(y) = x;
	    }
	  t_prop.stop();

	  return x;
	}

	template <typename I>
	inline
	void
	update_m_parent(const I& f,
			mln_ch_value(I, mln_psite(I))& parent,
			mln_ch_value(I, bool)& deja_vu,
			p_array< mln_psite(I) >& sset,
			const mln_domain(I)& d_ext,
			const mln_psite(I)& p)
	{
	  typedef mln_psite(I) P;

	  P q = parent(p);
	  P x = parent(q);

	  mln_assertion(d_ext.has(q));

	  while (d_ext.has(x) && f(q) == f(x) && q != x)
	    {
	      q = x;
	      x = parent(q);
	    }

	  if (!d_ext.has(x))
	    {
	      if (f(x) == f(parent(x)))
		x = (parent(q) = parent(x));
	      if (f(q) != f(x))
		{
		  x = q;
		  if (!deja_vu(q))
		    {
		      deja_vu(q) = true;
		      sset.append(q);
		    }
		}

	    }
	  else
	    {
	      if (x != q)
		{
		  update_m_parent(f, parent, deja_vu, sset, d_ext, q);
		  x = q;
		}
	      if (!deja_vu(q))
		{
		  deja_vu(q) = true;
		  sset.append(q);
		}
	    }

	  for (P i = p, tmp = parent(i); i != q; i = tmp, tmp = parent(i))
	    parent(i) = x;
	}

      } // end of namespace mln::morpho::tree::internal

      namespace impl
      {

	namespace generic
	{


	  template <typename I, typename S, typename N>
	  data< I, p_array<mln_psite(I)> >
	  dual_union_find(const Image<I>& f_,
			  const Image<I>& m_,
			  const Site_Set<S>& s_f_,
			  const Site_Set<S>& s_m_,
			  const Neighborhood<N>& nbh_)
	  {
	    mln_trace("morpho::tree::impl::generic::dual_union_find");

	    util::timer tm;
	    tm.start();
	    internal::t_prop.reset();

	    typedef mln_psite(I) P;
	    typedef unsigned L;

	    const I& f = exact(f_);
	    const I& m = exact(m_);
	    const S& s_f = exact(s_f_);
	    const S& s_m = exact(s_m_);
	    const N& nbh = exact(nbh_);

	    // Aux data.
	    mln_psite(I)::delta dp(literal::zero);
	    mln_domain(I) d_f = f.domain();
	    mln_domain(I) d_ext = f.domain(); // translate dp
	    mln_domain(I) d = f.domain();

	    // Extend the domain.
	    dp[0] = d.pmax()[0] - d.pmin()[0] + 1;
	    d.pmax() += dp;
	    d_ext.pmin() += dp;
	    d_ext.pmax() += dp;

	    // Data.
	    mln_concrete(I)		fext;
	    mln_ch_value(I, P)		parent;
	    p_array<mln_psite(I)>	s;

	    // Initialization.
	    fext = geom::translate(m, dp.to_vec(), f, d);
	    initialize(parent, fext);
	    s.reserve(geom::nsites(fext));

	    //Process
	    {
	      // Aux data.
	      mln_ch_value(I, bool)	deja_vu;
	      mln_ch_value(I, P)	zpar;

	      initialize(deja_vu, fext);
	      initialize(zpar, fext);
	      mln::data::fill(deja_vu, false);

	      mln_bkd_piter(S) p_f(s_f); // Backward.
	      mln_bkd_piter(S) p_m(s_m); // Backward.
	      p_f.start();
	      p_m.start();

	      // Main loop.
	      while (p_m.is_valid() || p_f.is_valid())
		{
		  mln_bkd_piter(S)& it = (!p_f.is_valid() || (p_m.is_valid() && f(p_f) <= m(p_m))) ? p_m : p_f;

		  P p = it;
		  P ext = p + dp;

		   // std::cout << "-------------------" << std::endl;
		   // std::cout << "Take " << p << " of value " << (&it == &p_m ? m(p) : f(p))
		   // 	    << " from " << (&it == &p_m ? "mask" : "f") << std::endl;
		   // debug::println("Parent: ", parent);
		   // debug::println("Zpar: ", zpar);

		  mln_assertion(!(deja_vu(p) && deja_vu(ext)));
		  if (deja_vu(ext)) // Seen by mask before f.
		    {
		      mln_assertion(m(p) >= f(p));
		      // Make set.
		      parent(p) = p;
		      zpar(p) = p;

		      P r = internal::find_root(zpar, ext);
		      zpar(r) = p;
		      parent(r) = p;

		      deja_vu(p) = true;
		    }
		  else if (deja_vu(p)) // Seen by f before mask.
		    {
		      mln_assertion(f(p) > m(p));
		      parent(p) = ext;
		      zpar(p) = ext;
		      parent(ext) = ext;
		      zpar(ext) = ext;

		      mln_niter(N) n(nbh, ext);
		      for_all(n)
			if (d_ext.has(n) && deja_vu(n))
			  {
			    P r = internal::find_root(zpar, n);
			    //std::cout << "Root: " << r << std::endl;
			    if (r != ext)
			      {
				parent(r) = ext;
				zpar(r) = ext;
			      }
			  }
		      deja_vu(ext) = true;
		    }
		  else if (f(p) <= m(p)) // First time p encountered.
		    {
		      zpar(ext) = ext;
		      mln_niter(N) n(nbh, ext);
		      for_all(n)
			if (d_ext.has(n) && deja_vu(n))
			  {
			    P r = internal::find_root(zpar, n);
			    if (r != ext)
			      {
				zpar(r) = ext;
				parent(r) = ext;
			      }
			  }
		      deja_vu(ext) = true;
		    }
		  else
		    {
		      deja_vu(p) = true;
		    }
		  it.next();
		}
	    }
	    std::cout << ">> MAJ zpar: " << internal::t_prop << " s" << std::endl;
	    std::cout << "Parent construction: " << tm << " s" << std::endl;
	    tm.restart();

	    // Canonization
	    {
	      mln_ch_value(I, bool) deja_vu(d_ext);
	      mln::data::fill(deja_vu, false);
	      mln_fwd_piter(S) p(s_f); // Forward.
	      for_all(p)
	      {
		P q = parent(p);
		if (!f.domain().has(q))
		  internal::update_m_parent(fext, parent, deja_vu, s, d_ext, p);
		else if (fext(parent(q)) == f(q))
		  parent(p) = parent(q);
		s.append(p);

		mln_assertion((q = parent(p)) == parent(q) || fext(q) != fext(parent(q)));
	      }
	    }
	    std::cout << "Canonization: " << tm << " s" << std::endl;

	    //mln_postcondition(internal::compute_parent_postconditions(fext, s, parent));

	    tree::data<I, p_array<mln_psite(I)> > tree(fext, parent, s);

	    return tree;
	  }

	}  // end of namespace mln::morpho::tree::impl::generic

      } // end of namespace mln::morpho::tree::impl

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_MORPHO_TREE_IMLP_DUAL_UNION_FIND_HH
