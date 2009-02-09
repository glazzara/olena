// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CANVAS_LABELING_HH
# define MLN_CANVAS_LABELING_HH

/// \file mln/canvas/labeling.hh
///
/// Connected component labeling of the object part in a binary image.
///
/// \todo Make the fastest version work.

# include <mln/core/concept/image.hh>
# include <mln/data/fill.hh>
# include <mln/literal/zero.hh>
# include <mln/convert/to_upper_window.hh>
# include <mln/extension/adjust_fill.hh>

# include <mln/level/sort_psites.hh>
# include <mln/level/sort_offsets.hh>


namespace mln
{

  namespace canvas
  {

    template <typename I, typename N, typename L,
	      typename F>
    mln_ch_value(I, L)
    labeling_video(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
		   F& functor);


    template <typename I, typename N, typename L,
	      typename F>
    mln_ch_value(I, L)
    labeling_sorted(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
		    F& functor, bool increasing);




# ifndef MLN_INCLUDE_ONLY

    // Tests.

    namespace internal
    {

      template <typename I, typename N, typename L,
		typename F>
      void
      labeling_tests(const Image<I>& input_, const Neighborhood<N>& nbh_, const L& nlabels,
		     const F& f)
      {
	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);

	mln_precondition(input.is_valid());
	// mln_precondition(nbh.is_valid());

	(void) input;
	(void) nbh;
	(void) nlabels;
	(void) f;
      }

    } // end of namespace mln::canvas::internal



    // Implementations.

    namespace impl
    {

      namespace generic
      {

	template <typename I>
	static inline
	mln_psite(I)
	find_root(I& parent, const mln_psite(I)& x)
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent, parent(x));
	}

	template <typename I, typename N, typename L,
		  typename S, typename F>
	mln_ch_value(I, L)
	  labeling(const Image<I>& input_, const Neighborhood<N>& nbh_, L& nlabels,
		   const Site_Set<S>& s_, F& f)
	{
	  trace::entering("canvas::impl::generic::labeling");

	  // FIXME: Test?!

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);
	  const S& s     = exact(s_);

	  // Local type.
	  typedef mln_psite(I) P;

	  // Auxiliary data.
	  mln_ch_value(I, bool) deja_vu;
	  mln_ch_value(I, P)    parent;

	  // Output.
	  mln_ch_value(I, L) output;
	  bool status;

	  // Initialization.
	  {
	    initialize(deja_vu, input);
	    mln::data::fill(deja_vu, false);

	    initialize(parent, input);

	    initialize(output, input);
	    mln::data::fill(output, L(literal::zero));
	    nlabels = 0;

	    f.init(); // Client initialization.
	  }

	  // First Pass.
	  {
	    mln_fwd_piter(S) p(s);
	    mln_niter(N) n(nbh, p);
	    for_all(p) if (f.handles(p))
	      {
		// Make-Set.
		parent(p) = p;
		f.init_attr(p);

		for_all(n)
		  if (input.domain().has(n) && deja_vu(n))
		    {
		      if (f.equiv(n, p))
			{
			  // Do-Union.
			  P r = find_root(parent, n);
			  if (r != p)
			    {
			      parent(r) = p;
			      f.merge_attr(r, p);
			    }
			}
		      else
			f.do_no_union(n, p);
		    }
		deja_vu(p) = true;
	      }
	  }

	  // Second Pass.
	  {
	    mln_bkd_piter(S) p(s);
	    for_all(p) if (f.handles(p))
	      {
		if (parent(p) == p) // if p is root
		  {
		    if (f.labels(p))
		      {
			if (nlabels == mln_max(L))
			  {
			    status = false;
			    return output;
			  }
			output(p) = ++nlabels;
		      }
		  }
		else
		  output(p) = output(parent(p));
	      }
	    status = true;
	  }

	  trace::exiting("canvas::impl::generic::labeling");
	  return output;
	}

      } // end of namespace mln::canvas::impl::generic



      // Fastest video version.

      template <typename I>
      static inline
      unsigned
      find_root_fastest(I& parent, unsigned x)
      {
	if (parent.element(x) == x)
	  return x;
	else
	  return parent.element(x) = find_root_fastest(parent, parent.element(x));
      }


      template <typename I, typename N, typename L,
	       typename F>
      mln_ch_value(I, L)
      labeling_video_fastest(const Image<I>& input_, const Neighborhood<N>& nbh_,
			     L& nlabels, F& f)
      {
	trace::entering("canvas::impl::labeling_video_fastest");

	// FIXME: Test?!

	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);

	extension::adjust(input, nbh);

	// Auxiliary data.
	mln_ch_value(I, bool) deja_vu;
	mln_ch_value(I, unsigned) parent;

	// Output.
	mln_ch_value(I, L) output;
	bool status;

	// Initialization.
	{
	  initialize(deja_vu, input);
   	  mln::data::fill(deja_vu, true);
	  extension::fill(deja_vu, false); // So that the extension is ignored.

	  initialize(parent, input);

	  initialize(output, input);
	  mln::data::fill(output, L(literal::zero));
	  nlabels = 0;

	  f.init_(); // Client initialization.
	}

	// First Pass.
	{
	  util::array<int> dp = negative_offsets_wrt(input, nbh);
	  const unsigned n_nbhs = dp.nelements();

	  mln_pixter(const I) px(input);
	  for_all(px)
	  {
	    unsigned p = px.offset();
	    if (! f.handles_(p))
	      continue;

	    // Make-Set.
	    parent.element(p) = p;
	    f.init_attr_(p);
	    for (unsigned i = 0; i < n_nbhs; ++i)
	    {
	      unsigned n = p + dp[i];
 	      if (deja_vu.element(n)) // Only false in the external border.
		{
		  if (f.equiv_(n, p))
		    {
		      // Do-Union.
		      unsigned r = find_root_fastest(parent, n);
		      if (r != p)
			{
			  parent.element(r) = p;
			  f.merge_attr_(r, p);
			}
		    }
		  else
		    f.do_no_union_(n, p);
	      }
	    }
	  }
	}

	// Second Pass.
	{
	  mln_bkd_pixter(const I) px(input);
	  for_all(px)
	  {
	    unsigned p = px.offset();
	    if (! f.handles_(p))
	      continue;
	    if (parent.element(p) == p) // if p is root
	      {
		if (f.labels_(p))
		  {
		    if (nlabels == mln_max(L))
		      {
			status = false;
			return output;
		      }
		    output.element(p) = ++nlabels;
		  }
	      }
	    else
	      output.element(p) = output.element(parent.element(p));
	  }
	  status = true;
	}

	trace::exiting("canvas::impl::labeling_video_fastest");
	return output;
      }



      // Fastest sorted version

      template <typename I, typename N, typename L,
		typename S, typename F>
      mln_ch_value(I, L)
      labeling_sorted_fastest(const Image<I>& input_, const Neighborhood<N>& nbh_, L& nlabels,
			      const S& s, F& f)
      {
	trace::entering("canvas::impl::labeling_sorted_fastest");

	// FIXME: Test?!

	const I& input = exact(input_);
	const N& nbh   = exact(nbh_);

	extension::adjust(input, nbh);

	// Local type.
	typedef mln_psite(I) P;

	// Auxiliary data.
	mln_ch_value(I, bool) deja_vu;
	mln_ch_value(I, unsigned)    parent;

	// Output.
	mln_ch_value(I, L) output;
	bool status;

	// Initialization.
	{
	  initialize(deja_vu, input);
	  mln::data::fill(deja_vu, false);
	  extension::fill(deja_vu, false); // So that the extension is ignored.

	  initialize(parent, input);

	  initialize(output, input);
	  mln::data::fill(output, L(literal::zero));
	  nlabels = 0;

	  f.init_(); // Client initialization.
	}

	util::array<int> dp = offsets_wrt(input, nbh);
	const unsigned n_nbhs = dp.nelements();

	const unsigned n_points = s.nelements();

	// First Pass.
	{

	  for (unsigned i = 0; i < n_points; ++i)
	    {
	      unsigned p = s[i];
	      if (! f.handles_(p))
		continue;

	      // Make-Set.
	      parent.element(p) = p;
	      f.init_attr_(p);

	      for (unsigned j = 0; j < n_nbhs; ++j)
		{
		  unsigned n = p + dp[j];
		  if (! deja_vu.element(n))
		    continue;

		  if (f.equiv_(n, p))
		    {
		      // Do-Union.
		      unsigned r = find_root_fastest(parent, n);
		      if (r != p)
			{
			  parent.element(r) = p;
			  f.merge_attr_(r, p);
			}
		    }
		  else
		    f.do_no_union_(n, p);
		}
	      deja_vu.element(p) = true;

	    }
	}

	// Second Pass.
	{
	  for (int i = n_points - 1; i >=0; --i)
	    {
	      unsigned p = s[i];
	      if (! f.handles_(p))
		continue;

	      if (parent.element(p) == p) // if p is root
		{
		  if (f.labels_(p))
		    {
		      if (nlabels == mln_max(L))
			{
			  status = false;
			  return output;
			}
		      output.element(p) = ++nlabels;
		    }
		}
	      else
		output.element(p) = output.element(parent.element(p));
	    }
	  status = true;
	}

	trace::exiting("canvas::impl::labeling_sorted_fastest");
	return output;
      }

    } // end of namespace mln::canvas::impl



    // Dispatch.

    namespace internal
    {

      // Video

      template <typename I, typename N, typename L,
		typename F>
      inline
      mln_ch_value(I, L)
      labeling_video_dispatch(metal::false_,
			      const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
			      F& functor)
      {
	return impl::generic::labeling(input, nbh, input.domain(), nlabels,
				       functor);
      }

      template <typename I, typename N, typename L,
	        typename F>
      inline
      mln_ch_value(I, L)
      labeling_video_dispatch(metal::true_,
			      const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
			      F& functor)
      {
	return impl::labeling_video_fastest(input, nbh, nlabels, functor);
      }

      template <typename I, typename N, typename L,
	        typename F>
      inline
      mln_ch_value(I, L)
      labeling_video_dispatch(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
			      F& functor)
      {
	enum {
	  test = mlc_equal(mln_trait_image_speed(I),
	      trait::image::speed::fastest)::value
	    &&
	    mln_is_simple_neighborhood(N)::value
	};
	return labeling_video_dispatch(metal::bool_<test>(),
				       input, nbh, nlabels,
				       functor);
      }


      // Sorted dispatch.

      template <typename I, typename N, typename L, typename F>
      inline
      mln_ch_value(I, L)
      labeling_sorted_dispatch(metal::false_,
			       const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
			       F& functor, bool increasing)
      {
	p_array<mln_psite(I)> s =
	  increasing ?
	  level::sort_psites_increasing(input) :
	  level::sort_psites_decreasing(input);
	return impl::generic::labeling(input, nbh, nlabels, s,
				       functor);
      }

      template <typename I, typename N, typename L, typename F>
      inline
      mln_ch_value(I, L)
      labeling_sorted_dispatch(metal::true_,
			       const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
			       F& functor, bool increasing)
      {
	util::array<unsigned> s =
	  increasing ?
	  level::sort_offsets_increasing(input) :
	  level::sort_offsets_decreasing(input);
	return impl::labeling_sorted_fastest(input, nbh, nlabels, s,
					     functor);
      }

      template <typename I, typename N, typename L, typename F>
      inline
      mln_ch_value(I, L)
      labeling_sorted_dispatch(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
			       F& functor, bool increasing)
      {
	enum {
	  test = mlc_equal(mln_trait_image_speed(I),
	      trait::image::speed::fastest)::value
	    &&
	    mln_is_simple_neighborhood(N)::value
	};
	return labeling_sorted_dispatch(metal::bool_<test>(),
					input, nbh, nlabels,
					functor, increasing);
      }


    } // end of namespace mln::canvas::internal



    // Facades.


    template <typename I, typename N, typename L,
	     typename F>
    inline
    mln_ch_value(I, L)
    labeling_video(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
		   F& functor)
    {
      trace::entering("canvas::labeling_video");

      internal::labeling_tests(input, nbh, nlabels, functor);

      mln_ch_value(I, L) output;
      output = internal::labeling_video_dispatch(input, nbh, nlabels,
						 functor);

      trace::exiting("canvas::labeling_video");
      return output;
    }


    template <typename I, typename N, typename L,
	     typename F>
    inline
    mln_ch_value(I, L)
    labeling_sorted(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels,
		    F& functor, bool increasing)
    {
      trace::entering("canvas::labeling_sorted");

      internal::labeling_tests(input, nbh, nlabels, functor);

      mln_ch_value(I, L) output;
      output = internal::labeling_sorted_dispatch(input, nbh, nlabels,
						  functor, increasing);

      trace::exiting("canvas::labeling_sorted");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_LABELING_HH
