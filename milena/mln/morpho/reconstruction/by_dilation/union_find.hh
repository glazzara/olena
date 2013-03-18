// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_MORPHO_RECONSTRUCTION_BY_DILATION_UNION_FIND_HH
# define MLN_MORPHO_RECONSTRUCTION_BY_DILATION_UNION_FIND_HH

# include <cstdlib>

# include <iostream>
# include <vector>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>
# include <mln/data/compare.hh>
# include <mln/data/sort_psites.hh>


namespace mln
{

  namespace morpho
  {

    namespace reconstruction
    {

      namespace by_dilation
      {


	template <typename I, typename J, typename N>
	mln_concrete(I)
	union_find(const Image<I>& f, const Image<J>& g,
		   const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY


	// Tests.

	namespace internal
	{

	  template <typename I, typename J, typename N>
	  inline
	  void
	  union_find_tests(const Image<I>& f_, const Image<J>& g_,
			   const Neighborhood<N>& nbh_)
	  {
	    const I& f = exact(f_);
	    const J& g = exact(g_);
	    const N& nbh = exact(nbh_);

	    mln_precondition(f.is_valid());
	    mln_precondition(g.is_valid());
	    mln_precondition(nbh.is_valid());

	    mln_precondition(f.domain() == g.domain()); // FIXME: Relax?
	    mln_precondition(f <= g);

	    // mlc_equal(mln_value(I), mln_value(J))::check(); // FIXME: Too strong!
	    // FIXME: Also check that we have a total ordering for values.

	    (void) f;
	    (void) g;
	    (void) nbh;
	  }



	  template <typename Par>
	  inline
	  mln_site(Par) find_root(Par& parent, mln_site(Par) x)
	  {
	    if (parent(x) == x)
	      return x;
	    else
	      return parent(x) = find_root(parent, parent(x));
	  }


	} // end of namespace mln::morpho::reconstruction::by_dilation::internal


	// Implementations.

	namespace impl
	{

	  namespace generic
	  {

	    template <typename I, typename J, typename N>
	    inline
	    mln_concrete(I)
	    union_find(const Image<I>& f_, const Image<J>& g_,
		       const Neighborhood<N>& nbh_)
	    {
	      mln_trace("morpho::reconstruction::by_dilation::impl::generic::union_find");

	      const I& f = exact(f_);
	      const J& g = exact(g_);
	      const N& nbh = exact(nbh_);

	      internal::union_find_tests(f, g, nbh);


	      typedef mln_site(I)  P;
	      typedef mln_value(I) V;

	      // Auxiliary data.
	      p_array<P> s;
	      mln_ch_value(I, bool) deja_vu;
	      mln_ch_value(I, P)    parent;
	      mln_concrete(I)       output;

	      // Initialization.
	      {
		initialize(output, f);
		data::fill(output, f);
		initialize(parent, f);
		initialize(deja_vu, f);
		data::fill(deja_vu, false);

		s = data::sort_psites_decreasing(g);
	      }

	      // First pass.
	      {
		for (unsigned i = 0; i < s.nsites(); ++i)
		  {
		    P p = s[i];
		    parent(p) = p; // Make-Set.
		    mln_niter(N) n(nbh, p);
		    for_all(n)
		    {
// 		      if (f.domain().has(n))
// 			mln_invariant(deja_vu(n)
// 				      ==
// 				      (g(n) > g(p) || (g(n) == g(p)
// 						       && util::ord_strict(n, p))));
		      if (f.domain().has(n) && deja_vu(n))
			{
			  // Do-Union.
			  P r = internal::find_root(parent, n);
			  if (r != p)
			    {
			      if (g(r) == g(p) || g(p) >= output(r)) // Equivalence test.
				{
				  parent(r) = p;
				  if (output(r) > output(p))
				    output(p) = output(r); // Increasing criterion.
				}
			      else
				output(p) = mln_max(V);
			    }
			}
		    }
		    deja_vu(p) = true;
		  }
	      }

	      // Second pass.
	      {
		for (int i = s.nsites() - 1; i >= 0; --i)
		  {
		    P p = s[i];
		    if (parent(p) == p)
		      {
			if (output(p) == mln_max(V))
			  output(p) = g(p);
		      }
		    else
		      output(p) = output(parent(p));
		  }
	      }

	      mln_postcondition(output >= f);
	      mln_postcondition(output <= g);

	      return output;
	    }


	  } // end of namespace mln::morpho::reconstruction::by_dilation::impl::generic

	} // end of namespace mln::morpho::reconstruction::by_dilation::impl


	// Dispatch.

	namespace internal
	{

	  template <typename I, typename J, typename N>
	  inline
	  mln_concrete(I)
	  union_find_dispatch(trait::image::kind::logic,
			      const Image<I>& f, const Image<J>& g,
			      const Neighborhood<N>& nbh)
	  {
	    (void) f;
	    (void) g;
	    (void) nbh;

	    // FIXME: Not yet implemented.
	    std::cerr
	      << __FILE__ << ":" << __LINE__ << ": error:\n"
	      "mln::morpho::reconstruction::by_dilation::internal::\n"
	      "  union_find_dispatch(mln::trait::image::kind::logic,\n"
	      "                      const mln::Image<I>&,\n"
	      "                      const mln::Image<J>&,\n"
	      "                      const mln::Neighborhood<N>&)\n"
	      "not implemented."
	      << std::endl;
	    std::abort();
	  }

	  template <typename I, typename J, typename N>
	  inline
	  mln_concrete(I)
	  union_find_dispatch(trait::image::kind::any,
			      const Image<I>& f, const Image<J>& g,
			      const Neighborhood<N>& nbh)
	  {
	    return impl::generic::union_find(f, g, nbh);
	  }

	  template <typename I, typename J, typename N>
	  inline
	  mln_concrete(I)
	  union_find_dispatch(const Image<I>& f, const Image<J>& g,
			      const Neighborhood<N>& nbh)
	  {
	    return union_find_dispatch(mln_trait_image_kind(I)(),
				       f, g, nbh);
	  }

	} // end of namespace mln::morpho::reconstruction::by_dilation::internal


	// Facade.

	template <typename I, typename J, typename N>
	inline
	mln_concrete(I)
	union_find(const Image<I>& f, const Image<J>& g,
		   const Neighborhood<N>& nbh)
	{
	  mln_trace("morpho::reconstruction::by_dilation::union_find");

	  internal::union_find_tests(f, g, nbh);

	  mln_concrete(I) output;
	  output = internal::union_find_dispatch(f, g, nbh);

	  return output;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::morpho::reconstruction::by_dilation

    } // end of namespace mln::morpho::reconstruction

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RECONSTRUCTION_BY_DILATION_UNION_FIND_HH
