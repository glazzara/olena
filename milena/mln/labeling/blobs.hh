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

#ifndef MLN_LABELING_BLOBS_HH
# define MLN_LABELING_BLOBS_HH

/// \file mln/labeling/blobs.hh
///
/// Connected component labeling of the binary objects of a binary
/// image using a queue-based algorithm.
///
/// \todo Handle abort in a nice way...

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>
# include <mln/core/site_set/p_queue_fast.hh>



namespace mln
{

  namespace labeling
  {

    /// Connected component labeling of the binary objects of a binary
    ///  image.
    ///
    /// \param[in]  input    The input image.
    /// \param[in]  nbh      The connexity of the objects.
    /// \param[out] nlabels  The number of labels.
    /// \return              The label image.
    ///
    /// \pre The input image has to be binary (checked at compile-time).
    ///
    /// A fast queue is used so that the algorithm is not recursive and
    /// can handle large binary objects (blobs).
    ///
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    blobs(const Image<I>& input, const Neighborhood<N>& nbh,
	  L& nlabels);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename N, typename L>
	mln_ch_value(I, L)
	  blobs_(const I& input, const N& nbh, L& nlabels)
	{
	  typedef mln_psite(I) P;

	  P cur;
	  mln_niter(N) n(nbh, cur);
	  p_queue_fast<P> qu;
	  const L zero = literal::zero;

	  // Initialization.
	  nlabels = literal::zero;
	  mln_ch_value(I, L) output;
	  initialize(output, input);
	  data::fill(output, zero);

	  // Loop.
	  mln_piter(I) p(input.domain());
	  for_all(p)
	    if (input(p) && output(p) == zero) // Object point, not labeled yet.
	      {
		// Label this point component.
		if (nlabels == mln_max(L))
		  {
		    trace::warning("labeling aborted! Too much labels \
				    for this label type: nlabels > \
				    max(label_type).");

		    return output;
		  }
		++nlabels;
		mln_invariant(qu.is_empty());
		qu.push(p);
		output(p) = nlabels;
		do
		  {
		    cur = qu.front();
		    qu.pop();
		    for_all(n) if (input.has(n))
		      if (input(n) && output(n) == zero)
			{
			  mln_invariant(! qu.compute_has(n));
			  qu.push(n);
			  output(n) = nlabels;
			}
		  }
		while (! qu.is_empty());
	      }

	  return output;
	}

      } // end of namespace mln::labeling::impl::generic


      template <typename I, typename N, typename L>
      mln_ch_value(I, L)
	blobs_(const I& input, const N& nbh, L& nlabels)
      {
	// The only implementation is the generic one.
	return generic::blobs_(input, nbh, nlabels);
      }

    } // end of namespace mln::labeling::impl


    // Facade.

    template <typename I, typename N, typename L>
    inline
    mln_ch_value(I, L)
    blobs(const Image<I>& input_, const Neighborhood<N>& nbh_,
	  L& nlabels)
    {
      trace::entering("labeling::blobs");
      mlc_equal(mln_trait_image_kind(I),
		mln::trait::image::kind::binary)::check();
      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      mln_precondition(input.is_valid());

      mln_ch_value(I, L) output = impl::blobs_(input, nbh, nlabels);

      trace::exiting("labeling::blobs");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_BLOBS_HH
