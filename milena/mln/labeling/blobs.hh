// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file mln/labeling/blobs.hh
 *
 * \brief Connected component labeling of the image objects at a given
 * blobs.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/level/fill.hh>
# include <mln/core/p_queue_fast.hh>



namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the binary objects of a binary
     *  image.
     *
     * \param[in]  input The input image.
     * \param[in]  nbh   The neighborhood.
     * \param[out] nlabels The number of labels.
     * \return  The label image.
     *
     * A fast queue is used so that the algorithm is not recursive and
     * can handle large binary objects (blobs).
     */
    template <typename I, typename N>
    mln_ch_value(I, unsigned)
      blobs(const Image<I>& input, const Neighborhood<N>& nbh, unsigned& nlabels);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // ...

    } // end of namespace mln::labeling::impl


    // Facade.

    template <typename I, typename N>
    mln_ch_value(I, unsigned)
      blobs(const Image<I>& input_, const Neighborhood<N>& nbh_, unsigned& nlabels)
    {
      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      mln_precondition(input.has_data());

      typedef mln_psite(I) P;

      P cur;
      mln_niter(N) n(nbh, cur);
      p_queue_fast<P> qu;

      // Initialization.
      nlabels = 0;
      mln_ch_value(I, unsigned) output;
      initialize(output, input);
      level::fill(output, 0);

      // Loop.
      mln_piter(I) p(input.domain());
      for_all(p)
	if (input(p) && ! output(p)) // Object point, not labeled yet.
	  {
	    // Label this point component.
	    ++nlabels;
	    mln_invariant(qu.is_empty());
	    qu.push(p);
	    output(p) = nlabels;
	    do
	      {
		cur = qu.front();
		qu.pop();
		for_all(n) if (input.has(n))
		  if (input(n) && ! output(n))
		    {
		      mln_invariant(! qu.has(n));
		      qu.push(n);
		      output(n) = nlabels;
		    }
	      }
	    while (! qu.is_empty());
	  }
      
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_BLOBS_HH
