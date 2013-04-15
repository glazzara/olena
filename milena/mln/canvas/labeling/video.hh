// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CANVAS_LABELING_VIDEO_HH
# define MLN_CANVAS_LABELING_VIDEO_HH

/// \file
///
/// Fast connected component labeling of the object part in a binary image.
///
/// \todo Can we get rid of 'deja_vu' (while playing with the border)
/// in the fastest video version?

# include <mln/core/concept/image.hh>
# include <mln/data/fill.hh>
# include <mln/literal/zero.hh>
# include <mln/extension/adjust_fill.hh>

# include <mln/canvas/labeling/internal/tests.hh>
# include <mln/canvas/labeling/internal/find_root_fastest.hh>
# include <mln/canvas/labeling/generic.hh>

namespace mln
{

  namespace canvas
  {

    namespace labeling
    {

      /*! \brief Fast connected component labeling of the object part
       *  in a binary image.
       *
       * \ingroup modcanvaslabeling
       */
      template <typename I, typename N, typename L, typename F>
      mln_ch_value(I, L)
      video(const Image<I>& input, const Neighborhood<N>& nbh,
	    L& nlabels, F& functor);


# ifndef MLN_INCLUDE_ONLY


      // Implementations.

      namespace impl
      {

	template <typename I, typename N, typename L, typename F>
	mln_ch_value(I, L)
	video_fastest(const Image<I>& input_,
		      const Neighborhood<N>& nbh_,
		      L& nlabels, F& f)
	{
	  mln_trace("canvas::impl::video_fastest");

	  // FIXME: Test?!

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);

	  extension::adjust(input, nbh);

	  // Auxiliary data.
	  mln_ch_value(I, bool) deja_vu;
	  mln_ch_value(I, unsigned) parent;

	  // Output.
	  mln_ch_value(I, L) output;

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
	    util::array<int> dp = positive_offsets_wrt(input, nbh);
	    const unsigned n_nbhs = dp.nelements();

	    mln_bkd_pixter(const I) px(input); // Backward.
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
		    unsigned r = internal::find_root_fastest(parent, n);
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
	    mln_fwd_pixter(const I) px(input); // Forward.
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
		    mln_trace_warning("labeling aborted! Too many labels for \
					this label type: nlabels >	\
					max(label_type).");
		    return output;
		  }
		  output.element(p) = ++nlabels;
		  f.set_new_label_(p, nlabels);
		}
	      }
	      else
	      {
		L lbl = output.element(parent.element(p));
		output.element(p) = lbl;
		f.set_label_(p, lbl);
	      }
	    }
	  }

	  f.finalize();
	  return output;
	}

      } // end of namespace mln::canvas::impl



      // Dispatch.

      namespace internal
      {

	template <typename I, typename N, typename L, typename F>
	inline
	mln_ch_value(I, L)
	video_dispatch(metal::false_,
		       const Image<I>& input,
		       const Neighborhood<N>& nbh, L& nlabels,
		       F& functor)
	{
	  return impl::generic::labeling(input, nbh, nlabels,
					 exact(input).domain(), functor);
	}

	template <typename I, typename N, typename L, typename F>
	inline
	mln_ch_value(I, L)
	video_dispatch(metal::true_,
		       const Image<I>& input,
		       const Neighborhood<N>& nbh, L& nlabels,
		       F& functor)
	{
	  return impl::video_fastest(input, nbh, nlabels, functor);
	}

	template <typename I, typename N, typename L, typename F>
	inline
	mln_ch_value(I, L)
	video_dispatch(const Image<I>& input,
		       const Neighborhood<N>& nbh, L& nlabels,
		       F& functor)
	{
	  enum {
	    test = mlc_equal(mln_trait_image_speed(I),
			     trait::image::speed::fastest)::value
	    &&
	    mln_is_simple_neighborhood(N)::value
	  };
	  return video_dispatch(metal::bool_<test>(),
				input, nbh, nlabels,
				functor);
	}


      } // end of namespace mln::canvas::internal



      // Facades.


      template <typename I, typename N, typename L, typename F>
      inline
      mln_ch_value(I, L)
      video(const Image<I>& input, const Neighborhood<N>& nbh,
	    L& nlabels, F& functor)
      {
	mln_trace("canvas::video");

	internal::labeling_tests(input, nbh, nlabels, functor);

	mln_ch_value(I, L) output;
	output = internal::video_dispatch(input, nbh, nlabels,
						   functor);

	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::labeling

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_LABELING_VIDEO_HH
