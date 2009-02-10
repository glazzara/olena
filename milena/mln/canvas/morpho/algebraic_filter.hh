// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CANVAS_MORPHO_ALGEBRAIC_FILTER_HH
# define MLN_CANVAS_MORPHO_ALGEBRAIC_FILTER_HH

/// \file mln/canvas/morpho/algebraic_filter.hh
///
/// Canvas for morphological algebraic filters.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/accumulator.hh>

# include <mln/data/fill.hh>
# include <mln/extension/adjust_fill.hh>

# include <mln/level/sort_psites.hh>
# include <mln/level/sort_offsets.hh>



namespace mln
{

  namespace canvas
  {

    namespace morpho
    {


      template <typename I, typename N, typename A>
      mln_concrete(I)
      algebraic_filter(const Image<I>& input,
		       const Neighborhood<N>& nbh,
		       const Accumulator<A>& accu,
		       const mln_result(A)& lambda,
		       bool increasing);



# ifndef MLN_INCLUDE_ONLY


      // Implementations.


      namespace impl
      {

	// Generic version.

	namespace generic
	{

	  template <typename I>
	  inline
	  mln_psite(I)
	  find_root(I& parent, const mln_psite(I)& x)
	  {
	    if (parent(x) == x)
	      return x;
	    else
	      return parent(x) = find_root(parent, parent(x));
	  }

	  template <typename I, typename N, typename S, typename A>
	  inline
	  mln_concrete(I)
	  algebraic_filter(const Image<I>& input_,
			   const Neighborhood<N>& nbh_,
			   const Accumulator<A>& accu_,
			   const mln_result(A)& lambda,
			   const Site_Set<S>& s_)
	  {
	    trace::entering("canvas::morpho::impl::generic::algebraic_filter");

	    // FIXME: Tests?

	    const I& input = exact(input_);
	    const N& nbh   = exact(nbh_);
	    const S& s     = exact(s_);

	    mln_concrete(I) output;
	    initialize(output, input);

	    // Local type.
	    typedef mln_psite(I) P;

	    // Auxiliary data.
	    mln_ch_value(I, bool)  deja_vu;
	    mln_ch_value(I, bool)  activity;
	    mln_ch_value(I, P)     parent;
	    mln_ch_value(I, A)     data;


	    // Initialization.
	    {
	      initialize(deja_vu, input);
	      mln::data::fill(deja_vu, false);
	      initialize(activity, input);
	      mln::data::fill(activity, true);
	      initialize(parent, input);
	      initialize(data, input);
	    }

	    // First pass.
	    {
	      mln_fwd_piter(S) p(s);
	      mln_niter(N) n(nbh, p);
	      for_all(p)
	      {
		// Make-Set.
		parent(p) = p;
		data(p).take_as_init(p);

		for_all(n)
		  if (input.domain().has(n) && deja_vu(n))
		    {
		      // Do-Union.
		      P r = find_root(parent, n);
		      if (r != p)
			{
			  if (input(r) == input(p) || (activity(r) && data(r) < lambda)) // Equiv(r, p)
			    // Either a flat zone or the component of r is still growing.
			    {
			      /* FIXME: Same remark as above concerning the
				 initialization of data(p); instead of

				 data(p).take(data(r));

				 we should (or could) have

				 unite_data(p, r);

				 so as to keep the generic aspect of this canvas
				 (as long as the set of acceptable types for the
				 template parameter A is not bound).  */
			      data(p).take(data(r));
			      parent(r) = p;
			      if (activity(r) == false)
				activity(p) = false;
			    }
			  else
			    activity(p) = false;
			}
		    }
		deja_vu(p) = true;
	      }
	    }

	    // Second pass.
	    {
	      mln_bkd_piter(S) p(s);
	      for_all(p)
		if (parent(p) == p) // p is root.
		  output(p) = input(p);
		else
		  output(p) = output(parent(p));
	    }

	    /*
	      Change 2nd pass into:
	      for_all(p) if (not is_root(p)) output(p) = output(parent(p));
	      and add in init:
	      mln::data::fill(output, input);
	    */
	    trace::exiting("canvas::morpho::impl::generic::algebraic_filter");

	    return output;
	  }

	} // end of namespace mln::canvas::morpho::impl::generic



	// Fastest version.


	template <typename I>
	inline
	unsigned
	find_root_fastest(I& parent, unsigned x)
	{
	  if (parent.element(x) == 0)
	    return x;
	  else
	    return parent.element(x) = find_root_fastest(parent, parent.element(x));
	}

	template <typename I, typename N, typename A>
	inline
	mln_concrete(I)
	algebraic_filter_fastest(const Image<I>& input_,
				 const Neighborhood<N>& nbh_,
				 const Accumulator<A>& accu_,
				 const mln_result(A)& lambda,
				 const util::array<unsigned>& s)
	{
	  trace::entering("canvas::morpho::impl::algebraic_filter_fastest");

	  // FIXME: Tests?

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);

	  extension::adjust(input, nbh);

	  mln_concrete(I) output;
	  initialize(output, input);

	  // Local type.
	  typedef mln_psite(I) P;

	  // Auxiliary data.
	  mln_ch_value(I, bool)  deja_vu;
	  mln_ch_value(I, bool)  activity;
	  mln_ch_value(I, unsigned) parent;
	  mln_ch_value(I, A)     data;

	  // Initialization.
	  {
	    initialize(deja_vu, input);
	    mln::data::fill(deja_vu, false);
	    extension::fill(deja_vu, false); // So that the extension is ignored.
	    initialize(activity, input);
	    mln::data::fill(activity, true);
	    initialize(parent, input);
	    mln::data::fill(parent, 0);
	    initialize(data, input);
	  }

	  util::array<int> dp = offsets_wrt(input, nbh);
	  const unsigned n_nbhs = dp.nelements();

	  const unsigned n = s.nelements();

	  // First pass.
	  {

	    for (unsigned i = 0; i < n; ++i)
	      {
		unsigned p = s[i]; // An offset.

		// Make set.
		data.element(p).take_as_init(); // FIXME: Very bad!

		for (unsigned j = 0; j < n_nbhs; ++j)
		  {
		    unsigned n = p + dp[j];
		    if (! deja_vu.element(n))
		      continue;

		    unsigned r = find_root_fastest(parent, n);
		    if (r != p)
		      {
			if (input.element(r) == input.element(p)
			    || (activity.element(r) && data.element(r) < lambda))
			  {
			    data.element(p).take(data.element(r));
			    parent.element(r) = p;
			    if (activity.element(r) == false)
			      activity.element(p) = false;
			  }
			else
			  activity.element(p) = false;
		      }
		  }

		deja_vu.element(p) = true;
	      }

	  }

	  // Second pass.
	  {
	    for (int i = n - 1; i >= 0 ; --i)
	      {
		unsigned p = s[i];
		if (parent.element(p) == 0) // p is root.
		  output.element(p) = input.element(p);
		else
		  output.element(p) = output.element(parent.element(p));
	      }
	  }
	  trace::exiting("canvas::morpho::impl::algebraic_filter_fastest");
	  return output;
	}


      } // end of namespace mln::canvas::morpho::impl



      // Dispatch.


      namespace internal
      {
	template <typename I, typename N, typename A>
	inline
	mln_concrete(I)
	algebraic_filter_dispatch(metal::false_,
				  const Image<I>& input,
				  const Neighborhood<N>& nbh,
				  const Accumulator<A>& accu,
				  const mln_result(A)& lambda,
				  bool increasing)
	{
	  p_array<mln_psite(I)> s = increasing ?
	    level::sort_psites_increasing(input) :
	    level::sort_psites_decreasing(input);
	  return impl::generic::algebraic_filter(input, nbh, accu, lambda,
						 s);
	}

	template <typename I, typename N, typename A>
	inline
	mln_concrete(I)
	algebraic_filter_dispatch(metal::true_,
				  const Image<I>& input,
				  const Neighborhood<N>& nbh,
				  const Accumulator<A>& accu,
				  const mln_result(A)& lambda,
				  bool increasing)
	{
	  util::array<unsigned> s = increasing ?
	    level::sort_offsets_increasing(input) :
	    level::sort_offsets_decreasing(input);
	  return impl::algebraic_filter_fastest(input, nbh, accu, lambda,
						s);
	}

	template <typename I, typename N, typename A>
	inline
	mln_concrete(I)
	algebraic_filter_dispatch(const Image<I>& input,
				  const Neighborhood<N>& nbh,
				  const Accumulator<A>& accu,
				  const mln_result(A)& lambda,
				  bool increasing)
	{
	  enum {
	    test = (mlc_equal(mln_trait_image_speed(I),
			      trait::image::speed::fastest)::value
		    &&
		    mln_is_simple_neighborhood(N)::value
		    &&
		    mlc_equal(mln_trait_accumulator_when_pix(A),
			      trait::accumulator::when_pix::use_none)::value)
	  };
	  return algebraic_filter_dispatch(metal::bool_<test>(),
					   input, nbh, accu, lambda,
					   increasing);
	}

      } // end of namespace mln::canvas::morpho::internal



      // Facade.


      template <typename I, typename N, typename A>
      inline
      mln_concrete(I)
      algebraic_filter(const Image<I>& input,
		       const Neighborhood<N>& nbh,
		       const Accumulator<A>& accu,
		       const mln_result(A)& lambda,
		       bool increasing)
      {
	return internal::algebraic_filter_dispatch(input, nbh, accu, lambda,
						   increasing);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::morpho

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_MORPHO_ALGEBRAIC_FILTER_HH
