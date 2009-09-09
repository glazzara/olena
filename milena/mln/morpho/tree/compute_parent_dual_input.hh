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

///
/// \brief Compute the parent image for the dual-input trees.
///
/// The dual-input trees are computed from two functions. It computes the tree
/// of the first image using the mask-based connectivity defined by the second
/// one.
///
/// TODO: The current implementation is for dual-input max tree. Make it generic for min and max trees.

#ifndef MLN_MORPHO_TREE_COMPUTE_PARENT_DUAL_INPUT_HH
# define MLN_MORPHO_TREE_COMPUTE_PARENT_DUAL_INPUT_HH

# include <mln/morpho/tree/compute_parent.hh>
#include <mln/debug/all.hh>


namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      template <typename I, typename M, typename N, typename S>
      mln_ch_value(I, mln_psite(I))
	compute_parent_dual_input(const Image<I>& f,
				  const Image<M>& m,
				  const Neighborhood<N>& nbh,
				  const Site_Set<S>& s_f,
				  const Site_Set<S>& s_m);

# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename I, typename M, typename N, typename S>
	void
	compute_parent_dual_tests(const Image<I>& f_,
				  const Image<M>& m_,
				  const Neighborhood<N>& nbh_,
				  const Site_Set<S>& s_f_,
				  const Site_Set<S>& s_m_)
	{
	  const I& f   = exact(f_);
	  const M& m   = exact(m_);
	  const N& nbh = exact(nbh_);
	  const S& s_f   = exact(s_f_);
	  const S& s_m   = exact(s_m_);

	  mlc_equal(mln_psite(I), mln_psite(M))::check();

	  mln_precondition(f.is_valid());
	  mln_precondition(m.is_valid());
	  mln_precondition(nbh.is_valid());
	  mln_precondition(f.domain() == m.domain());
	  mln_precondition(s_f == s_m);
	  mln_precondition(f.domain() == s_f);

	  (void) f;
	  (void) m;
	  (void) nbh;
	  (void) s_f;
	  (void) s_m;
	}

	template <typename I, typename L>
	inline
	mln_value(L) propagate_label(mln_psite(I) p,
				     const I& parent,
				     L& label)
	{
	  if (label(p) <= label(parent(p)) && parent(p) != p)
	    return (label(p) = propagate_label(parent(p), parent, label));
	  else
	    return label(p);
	}

	template <typename I, typename L>
	inline
	mln_value(L) get_label(mln_psite(I) p,
			       const I& parent,
			       L& label)
	{
	  while (label(p) <= label(parent(p)) && parent(p) != p)
	    p = parent(p);
	  return label(p);
	}

      } // end of namespace mln::morpho::tree::internal

      namespace impl
      {

	namespace generic
	{

	  /// TODO: make generic for min tree.
	  template <typename I, typename M, typename N, typename S>
	  inline
	  mln_ch_value(I, mln_psite(I))
	    compute_parent_dual_input(const Image<I>& f_,
				      const Image<M>& mask_,
				      const Neighborhood<N>& nbh_,
				      const Site_Set<S>& s_f_,
				      const Site_Set<S>& s_m_)
	  {
	    trace::entering("morpho::tree::impl::generic::compute_parent_dual_input");

	    typedef mln_psite(I) P;
	    typedef unsigned L;

	    const I& f = exact(f_);
	    const M& m = exact(mask_);
	    const S& s_f = exact(s_f_);
	    const S& s_m = exact(s_m_);

	    mln_ch_value(M, P) mparent;
	    mln_ch_value(I, P) fparent;
	    L mask_nlabel;

	    // 1st pass: Construction of the mask tree.
	    {
	      const N& nbh = exact(nbh_);

	      // Aux data.
	      mln_ch_value(M, bool) deja_vu;
	      mln_ch_value(M, P) zpar;

	      initialize(deja_vu, m);
	      initialize(zpar, m);
	      initialize(mparent, m);

	      data::fill(deja_vu, false);

	      mask_nlabel = m.domain().nsites();

	      // Body.
	      mln_bkd_piter(S) p(s_m); // Backward.
	      mln_niter(N) n(nbh, p);
	      for_all(p)
	      {
	  	// Make-Set.
	  	mparent(p) = p;
	  	zpar(p) = p;

	  	bool leaf = false;
	  	for_all(n)
	  	  if (f.domain().has(n) && deja_vu(n))
	  	    {
	  	      // Do-Union.
	  	      P r = internal::zfind_root(zpar, n);
	  	      if (r != p)
	  		{
	  		  mparent(r) = p;
	  		  zpar(r) = p;
	  		  if (!leaf)
	  		    {
	  		      leaf = true;
	  		      --mask_nlabel;
	  		    }
	  		}
	  	    }
	  	deja_vu(p) = true;
	      }
	    }

	    // 2nd pass: mask based tree construction
	    {
	      mln_ch_value(I, bool) deja_vu;
	      mln_ch_value(I, L) label;
	      p_array<P> representatives;
	      util::array<bool> activity;

	      initialize(deja_vu, f);
	      initialize(label, f);
	      initialize(fparent, f);
	      representatives.resize(mask_nlabel + 1);
	      activity.resize(mask_nlabel + 1, false);

	      data::fill(deja_vu, false);

	      L nlabel = 0;

	      mln_fwd_piter(S) p_m(s_m); // Forward.
	      mln_fwd_piter(S) p_f(s_f); // Forward.
	      p_m.start();
	      p_f.start();

	      for (unsigned i = 0; i < f.domain().nsites();)
		{
		  mln_assertion(p_m.is_valid() || p_f.is_valid());

		  if (p_m.is_valid() && deja_vu(p_m))
		    {
		      p_m.next();
		      continue;
		    }
		  if (p_f.is_valid() && deja_vu(p_f))
		    {
		      p_f.next();
		      continue;
		    }

		  if (!p_f.is_valid() || (p_m.is_valid() && m(p_m) < f(p_f)))
		    {
		      // Mask under (strict) f level.

		      // Make set.
		      fparent(p_m) = p_m;

		      L c_label;
		      // Propagate label & retrieve the current component label.
		      if (m(mparent(p_m)) == m(p_m) && mparent(p_m) != p_m)
			c_label = internal::propagate_label(p_m, mparent, label);
		      else
			{
			  mln_assertion(nlabel < mask_nlabel);
			  c_label = (label(p_f) = ++nlabel);
			}

		      //for debug
			{
			  std::cout << "Current: " << p_m << " from mask" << std::endl;
			  debug::println("Labels", label);
			  std::cout << "Repres: " << representatives << std::endl;
			  std::cout << "Activity: " << activity << std::endl;
			  std::cout << "Label: " << c_label << std::endl;
			}


		      // Attach nodes issued from contraction.
		      if (activity[c_label])
			{
			  fparent(p_m) = representatives[c_label];
			  std::cout << "Node: " << p_m << " attached to " << representatives[c_label] << std::endl;
			}

		      deja_vu(p_m) = true;
		      p_m.next();
		    }
		  else
		    {
		      // Mask upper (or equal) f level.
		      mln_invariant(!p_m.is_valid() || f(p_f) <= m(p_m));

		      // Make set.
		      fparent(p_f) = p_f;

		      L c_label, old_label;
		      // Propagate label & retrieve the current component label.
		      if (m(mparent(p_f)) == m(p_f) && mparent(p_f) != p_f)
			c_label = internal::propagate_label(p_f, mparent, label);
		      else
			{
			  mln_assertion(nlabel < mask_nlabel);
			  old_label = internal::get_label(p_f, mparent, label);
			  c_label = (label(p_f) = ++nlabel);
			}

		      // for debug
		      {
		      	std::cout << "Current: " << p_f << " from f" << std::endl;
		      	debug::println("Labels", label);
		      	std::cout << "Repres: " << representatives << std::endl;
		      	std::cout << "Activity: " << activity << std::endl;
		      	std::cout << "Label: " << c_label << std::endl;
		      }

		      // Clustering
		      if (activity[c_label])
			{
			  mln_assertion(f(representatives[c_label]) <= f(p_f));
			  fparent(p_f) = representatives[c_label];
			  std::cout << "Node: " << p_f << " attached to " << representatives[c_label] << std::endl;
			  if (f(representatives[c_label]) != f(p_f))
			    representatives[c_label] = p_f;
			}
		      else
			{
			  activity[c_label] = true;
			  representatives[c_label] = p_f;
			  if (activity[old_label])
			    {
			      fparent(p_f) = representatives[old_label];
			      std::cout << "Node: " << p_f << " attached to " << representatives[old_label] << std::endl;
			    }
			}

		      deja_vu(p_f) = true;
		      p_f.next();
		    }
		  ++i;
		}
	    }

	    debug::println("Parent", fparent);

	    trace::exiting("morpho::tree::impl::generic::compute_parent_dual_input");
	    return fparent;
	  }

	}  // end of namespace mln::morpho::tree::impl::generic

      }  // end of namespace mln::morpho::tree::impl

      // Dispatch.

      namespace internal
      {

	template <typename I, typename M, typename N, typename S>
	inline
	mln_ch_value(I, mln_psite(I))
	  compute_parent_dual_dispatch(const Image<I>& f,
				       const Image<M>& m,
				       const Neighborhood<N>& nbh,
				       const Site_Set<S>& s_f,
				       const Site_Set<S>& s_m)
	{
	  return impl::generic::compute_parent_dual_input(f, m, nbh, s_f, s_m);
	}

      }  // end of namespace mln::morpho::tree::internal

      // Facade.
      template <typename I, typename M, typename N, typename S>
      inline
      mln_ch_value(I, mln_psite(I))
	compute_parent_dual_input(const Image<I>& f,
				  const Image<M>& m,
				  const Neighborhood<N>& nbh,
				  const Site_Set<S>& s_f,
				  const Site_Set<S>& s_m)
      {
	trace::entering("morpho::tree::compute_parent_dual_input");

	internal::compute_parent_dual_tests(f, m, nbh, s_f, s_m);

	mln_ch_value(I, mln_psite(I)) output;
	output = internal::compute_parent_dual_dispatch(f, m, nbh, s_f, s_m);

	trace::exiting("morpho::tree::compute_parent_dual_input");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_TREE_COMPUTE_PARENT_DUAL_INPUT_HH
