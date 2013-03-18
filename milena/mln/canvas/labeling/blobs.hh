// Copyright (C) 2009, 2010, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_CANVAS_LABELING_BLOBS_HH
# define MLN_CANVAS_LABELING_BLOBS_HH

/// \file
///
/// Canvas for connected component labeling of the binary objects of a
/// binary image using a queue-based algorithm.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>
# include <mln/core/site_set/p_queue_fast.hh>

# include <mln/extension/fill.hh>

# include <mln/util/pix.hh>

namespace mln
{

  namespace canvas
  {

    namespace labeling
    {

      /*! \brief Canvas for connected component labeling of the binary
          objects of a binary image using a queue-based algorithm.

          \param[in]  input      The input image.
	  \param[in]  nbh        The connexity of the objects.
	  \param[out] nlabels    The Number of labels. Its value is set in
	                         the algorithms.
	  \param[in,out] functor A functor computing data while labeling.

	  \return The label image.

          \pre The input image has to be binary (checked at compile-time).

          A fast queue is used so that the algorithm is not recursive and
          can handle large binary objects (blobs).

	  \ingroup modcanvaslabeling
      */
      template <typename I, typename N, typename L, typename F>
      mln_ch_value(I, L)
      blobs(const Image<I>& input, const Neighborhood<N>& nbh,
	    L& nlabels, F& functor);



# ifndef MLN_INCLUDE_ONLY


      // Implementations.

      namespace impl
      {

	// Generic implementation.

	namespace generic
	{

	  template <typename I, typename N, typename L, typename F>
	  mln_ch_value(I, L)
	  blobs(const Image<I>& input_, const N& nbh, L& nlabels, F& functor)
	  {
	    const I& input = exact(input_);

	    typedef mln_psite(I) P;

	    P cur;
	    mln_niter(N) n(nbh, cur);
	    p_queue_fast<P> qu;
	    const L zero = literal::zero;

	    // Initialization.
	    nlabels = literal::zero;
	    typedef mln_ch_value(I, L) out_t;
	    out_t output;
	    initialize(output, input);
	    data::fill(output, zero);

	    extension::fill(input, false);

	    functor.init(); // <-- functor.init()

	    // Loop.
	    mln_piter(I) p(input.domain());
	    for_all(p)
	      if (input(p) && output(p) == zero) // Object point, not labeled yet.
	      {
		// Label this point component.
		if (nlabels == mln_max(L))
		{
		  debug::trace::warning("labeling aborted! Too many labels \
for this label type: nlabels > max(label_type).");

		  return output;
		}
		++nlabels;
		functor.new_label(nlabels); // <-- functor.new_label()
		mln_invariant(qu.is_empty());
		qu.push(p);
		output(p) = nlabels;
		functor.process_p(p); // <-- functor.process_p()
		                      // output(p) == nlabels
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
		      functor.process_n(n); // <-- functor.process_n()
                                            // output(n) == nlabels
		    }
		}
		while (! qu.is_empty());
	      }

	    functor.finalize(); // <-- functor.finalize()

	    return output;
	  }

	} // end of namespace mln::labeling::impl::generic



      } // end of namespace mln::canvas::labeling::impl


      // Facade.

      template <typename I, typename N, typename L, typename F>
      inline
      mln_ch_value(I, L)
      blobs(const Image<I>& input_, const Neighborhood<N>& nbh_,
	    L& nlabels, F& functor)
      {
	mln_trace("labeling::blobs");
	mlc_equal(mln_trait_image_kind(I),
		  mln::trait::image::kind::binary)::check();
	const I& input = exact(input_);
	const N& nbh = exact(nbh_);
	mln_precondition(input.is_valid());

	// The only implementation is the generic one.
	mln_ch_value(I, L)
	  output = impl::generic::blobs(input, nbh, nlabels, functor);

	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::labeling

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_LABELING_BLOBS_HH
