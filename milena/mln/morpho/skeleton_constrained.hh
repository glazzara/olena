// Copyright (C) 2008, 2009, 2011, 2012 EPITA Research and Development
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

#ifndef MLN_MORPHO_SKELETON_CONSTRAINED_HH
# define MLN_MORPHO_SKELETON_CONSTRAINED_HH

/// \file
///
/// Compute a skeleton under constraints.
///
/// \todo Add an extension handling policy for the user to set it.
/// \todo The fast version does not give the exact result!

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/core/site_set/p_priority.hh>
# include <mln/extension/adjust_duplicate.hh>
# include <mln/extension/adjust_fill.hh>
# include <mln/data/fill.hh>

# include <mln/util/timer.hh>

namespace mln
{

  namespace morpho
  {

    /*! \brief Compute a skeleton under constraints.
     *
     * \ingroup mlnmorpho
     */
    template <typename I,
	      typename N, typename F,
	      typename K, typename R>
    mln_ch_value(I, bool)
    skeleton_constrained(const Image<I>& input,
			 const Neighborhood<N>& nbh, const F& is_simple,
			 const Image<K>& constraint, const Image<R>& priority);



# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      namespace generic
      {

	template <typename I,
		  typename N, typename F,
		  typename K, typename R>
	inline
	mln_ch_value(I, bool)
	  skeleton_constrained(const Image<I>& input_,
			       const Neighborhood<N>& nbh_, const F& is_simple,
			       const Image<K>& constraint_, const Image<R>& priority_)
	{
	  mln_trace("morpho::skeleton_constrained");

	  const I& input      = exact(input_);
	  const N& nbh        = exact(nbh_);
	  const K& constraint = exact(constraint_);
	  const R& priority   = exact(priority_);

	  mln_precondition(input.is_valid());
	  mln_precondition(nbh.is_valid());
	  mln_precondition(constraint.is_valid());
	  mln_precondition(priority.is_valid());

	  typedef mln_value(I) V;
	  mlc_is(V, bool)::check();

	  extension::adjust_duplicate(input, nbh);
	  extension::adjust_duplicate(constraint, nbh);
	  extension::adjust_duplicate(priority, nbh);

	  // FIXME: Tests!

	  typedef mln_psite(I) P;
	  typedef p_queue_fast<P> Q;
	  p_priority<mln_value(R), Q> q;

	  mln_concrete(I) output;

	  // Initialization.
	  {
	    output = duplicate(input);
	    extension::adjust_duplicate(output, nbh);

	    mln_piter(I) p(input.domain());
	    for_all(p)
	      if (input(p) == false &&
		  is_simple.check(input, p)) // <-- is_simple.check
                                      // p is a simple point of the background.
	      {
		q.push(priority(p), p);
	      }
	  }

	  // Propagation.
	  {
	    P p;

	    mln_niter(N) n(nbh, p);
	    while (! q.is_empty())
	    {
	      p = q.pop_front();
	      for_all(n)
		if (output.has(n) &&
		    output(n) == true &&
		    constraint(n) == false &&
		    is_simple.check(output, n)) // <-- is_simple.check
		{
		  output(n) = false; // Remove n from object.
		  q.push(priority(n), n);
		}
	    }
	  }

	  return output;
	}

      } // end of namespace mln::morpho::impl::generic


	template <typename I,
		  typename N, typename F,
		  typename K, typename R>
	inline
	mln_ch_value(I, bool)
	skeleton_constrained_fast(const Image<I>& input_,
				  const Neighborhood<N>& nbh_,
				  const F& is_simple,
				  const Image<K>& constraint_,
				  const Image<R>& priority_)
	{
	  mln_trace("morpho::skeleton_constrained_fast");

	  const I& input      = exact(input_);
	  const N& nbh        = exact(nbh_);
	  const K& constraint = exact(constraint_);
	  const R& priority   = exact(priority_);

	  mln_precondition(input.is_valid());
	  mln_precondition(nbh.is_valid());
	  mln_precondition(constraint.is_valid());
	  mln_precondition(priority.is_valid());

	  typedef mln_value(I) V;
	  mlc_is(V, bool)::check();

	  // Whatever the value of the extension, results of this fast
	  // version may differ from the generic one. Indeed, we
	  // cannot check whether a site belongs to the image or not,
	  // so we try to not take border site in consideration by
	  // setting their value to false.
	  extension::adjust_fill(input, nbh, false);
	  extension::adjust_fill(constraint, nbh, false);

	  // FIXME: Tests!

	  typedef p_queue_fast<unsigned> Q;
	  p_priority<mln_value(R), Q> q;

	  mln_concrete(I) output;

	  // Initialization.
	  {
	    output = duplicate(input);
	    extension::adjust_fill(output, nbh, false);

	    mln_pixter(const I) p_in(input);
	    for_all(p_in)
	      if (p_in.val() == false &&
		  is_simple.check__(input, p_in)) // <-- is_simple.check
		                                  // p is a simple point of the background.
	      {
		q.push(priority.element(p_in.offset()), p_in);
	      }
	  }

	  // Propagation.
	  {
	    util::array<int> dp = offsets_wrt(input, nbh.foreground());
	    const unsigned n_nbhs = dp.nelements();
	    while (! q.is_empty())
	    {
	      unsigned p = q.pop_front();

	      for (unsigned i = 0; i < n_nbhs; ++i)
	      {
		unsigned n = p + dp[i];

		if (output.element(n) == true &&
		    constraint.element(n) == false &&
		    is_simple.check__(output, n)) // <-- is_simple.check
		{
		  output.element(n) = false; // Remove n from object.
		  q.push(priority.element(n), n);
		}
	      }
	    }
	  }

	  return output;
	}


    } // end of namespace mln::morpho::impl


    namespace internal
    {

      template <typename I,
		typename N, typename F,
		typename K, typename R>
      inline
      mln_ch_value(I, bool)
      skeleton_constrained_dispatch(
	mln::trait::image::value_access::any,
	mln::trait::image::value_storage::any,
	const Image<I>& input,
	const Neighborhood<N>& nbh,
	const F& is_simple,
	const Image<K>& constraint,
	const Image<R>& priority)
      {
	return impl::generic::skeleton_constrained(input, nbh,
						   is_simple,
						   constraint,
						   priority);
      }

      template <typename I,
		typename N, typename F,
		typename K, typename R>
      inline
      mln_ch_value(I, bool)
      skeleton_constrained_dispatch(
	mln::trait::image::value_access::direct,
	mln::trait::image::value_storage::one_block,
	const Image<I>& input,
	const Neighborhood<N>& nbh,
	const F& is_simple,
	const Image<K>& constraint,
	const Image<R>& priority)
      {
	return impl::skeleton_constrained_fast(input, nbh,
					       is_simple,
					       constraint,
					       priority);
      }


    } // end of namespace mln::morpho::internal


    template <typename I,
	      typename N, typename F,
	      typename K, typename R>
    inline
    mln_ch_value(I, bool)
    skeleton_constrained(const Image<I>& input,
			 const Neighborhood<N>& nbh, const F& is_simple,
			 const Image<K>& constraint, const Image<R>& priority)
    {
      mln_trace("morpho::skeleton_constrained");

      mln_ch_value(I, bool)
	output = internal::skeleton_constrained_dispatch(
	  mln_trait_image_value_access(I)(),
	  mln_trait_image_value_storage(I)(),
	  input, nbh, is_simple, constraint, priority);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_SKELETON_CONSTRAINED_HH
