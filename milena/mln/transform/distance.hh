// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_TRANSFORM_DISTANCE_HH
# define MLN_TRANSFORM_DISTANCE_HH

/// \file mln/transform/distance.hh
///
/// Discrete distance transform.
///
/// \todo Make a canvas out of it.

# include <vector>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/level/fill.hh>
# include <mln/accu/max.hh>



namespace mln
{

  namespace transform
  {

    /*! Discrete distance transform.
     *
     * FIXME: doc.
     */
    template <typename D, typename I, typename N, typename W>
    mln_ch_value(I, D)
    distance(D, const Image<I>& input,
	     const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename D, typename I, typename N, typename W>
	mln_ch_value(I, D)
	  distance(D, const Image<I>& input_,
		   const Neighborhood<N>& nbh_, const Weighted_Window<W>& w_win_)
	{
	  trace::entering("transform::impl::generic::distance");

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);
	  const W& w_win = exact(w_win_);

	  mln_precondition(input.has_data());

	  const D Max = mln_max(D);
    
	  typedef mln_site(I) P;
	  typedef std::vector<P> bucket_t;

	  mln_ch_value(I, D) output;
	  initialize(output, input);
	  level::fill(output, Max);

	  // Modulus determination.
	  unsigned mod;
	  {
	    mln::accu::max<unsigned> accu;
	    P p;
	    mln_qiter(W) q(w_win, p);
	    for_all(q)
	      accu.take(q.w());
	    mod = accu.to_result() + 1;
	  }

	  std::vector<bucket_t> bucket;
	  bucket.resize(mod);
	  unsigned bucket_size = 0;

	  // Initialization.
	  {
	    mln_piter(I) p(input.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	      if (input(p) == true)
		{
		  output(p) = literal::zero;
		  for_all(n)
		    if (input.domain().has(n) && input(n) == false)
		      {
			bucket[0].push_back(p);
			++bucket_size;
			break;
		      }
		}
	  }

	  // Propagation.
	  {
	    P p;
	    mln_qiter(W) q(w_win, p);

	    for (unsigned d = 0; bucket_size != 0; ++d)
	      {
		bucket_t& bucket_d = bucket[d % mod];
		for (unsigned i = 0; i < bucket_d.size(); ++i)
		  {
		    p = bucket_d[i];
		    if (output(p) < d)
		      continue;

		    for_all(q)
		      if (output.domain().has(q) && output(q) > d)
			{
			  if (unsigned(Max - q.w()) < d) // Saturation => Stop!
			    {
			      // trace::warning...
			      trace::exiting("transform::impl::generic::distance");
			      return output;
			    }
			  unsigned d_ = d + q.w();

			  if (d_ < output(q))
			    {
			      output(q) = d_;
			      bucket[d_ % mod].push_back(q);
			      ++bucket_size;
			    }
			}
		  }
		bucket_size -= bucket_d.size();
		bucket_d.clear();
	      }
	  }

	  trace::exiting("transform::impl::generic::distance");
	  return output;

	}

      } // end of namespace mln::transform::impl::generic

    } // end of namespace mln::transform::impl


    // Facade.

    template <typename D, typename I, typename N, typename W>
    inline
    mln_ch_value(I, D)
    distance(D, const Image<I>& input,
	     const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win)
    {
      trace::entering("transform::distance");

      // FIXME: tests.
      mln_ch_value(I, D) output = impl::generic::distance(D(), input,
							  nbh, w_win);

      trace::exiting("transform::distance");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_DISTANCE_HH
