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

#ifndef MLN_TRANSFORM_DISTANCE_GEODESIC_HH
# define MLN_TRANSFORM_DISTANCE_GEODESIC_HH

/// \file mln/transform/distance_geodesic.hh
///
/// Discrete geodesic distance transform.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/p_queue_fast.hh>
# include <mln/level/fill.hh>



namespace mln
{

  namespace transform
  {

    /// Discrete geodesic distance transform.
    template <typename I, typename N, typename D>
    mln_ch_value(I, D)
    distance_geodesic(const Image<I>& input, const Neighborhood<N>& nbh, D max);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename N, typename D>
	mln_ch_value(I, D)
	  distance_geodesic(const Image<I>& input_, const Neighborhood<N>& nbh_, D max)
	{
	  trace::entering("transform::impl::generic::distance_geodesic");

	  const I& input = exact(input_);
	  const N& nbh   = exact(nbh_);

	  mln_precondition(input.has_data());

	  mln_ch_value(I, D) output;
	  initialize(output, input);

	  typedef mln_site(I) P;
	  p_queue_fast<P> q;

	  // Initialization.
	  {
	    level::fill(output, max);
	    mln_piter(I) p(input.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	      if (input(p) == true) // p in object
		{
		  output(p) = 0;
		  for_all(n)
		    if (input.domain().has(n) && input(n) == false) // n in background
		      {
			q.push(p);
			break;
		      }
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
		  if (input.domain().has(n) && output(n) == max)
		    {
		      output(n) = output(p) + 1;
		      if (output(n) == max)
			{
			  // Saturation so stop.
			  q.clear();
			  break;
			}
		      q.push(n);
		    }
	      }
	  }

	  trace::exiting("transform::impl::generic::distance_geodesic");
	  return output;
	}

      } // end of namespace mln::transform::impl::generic

    } // end of namespace mln::transform::impl


    // Facade.

    template <typename I, typename N, typename D>
    inline
    mln_ch_value(I, D)
      distance_geodesic(const Image<I>& input, const Neighborhood<N>& nbh, D max)
    {
      trace::entering("transform::distance_geodesic");

      // FIXME: tests.

      mln_ch_value(I, D) output;
      output = impl::generic::distance_geodesic(input, nbh, max);

      trace::exiting("transform::distance_geodesic");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_TRANSFORM_DISTANCE_HH
