// Copyright (C) 2005, 2008 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef MLN_MORPHO_SHORTEST_PATH_WATERSHED_HH
# define MLN_MORPHO_SHORTEST_PATH_WATERSHED_HH

/* FIXME: This file comes from Olena proto-1.0, and needs some
   adjustments.

   Translate it into the Milena dialect, e.g.,
   - replace `oln_' and `oln::' by `mln_' and `mln::';
   - replace `internal::' by `impl::';
   - adjust the names of interfaces and types;
   - adjust other names;
   - adjust calling conventions;
   - adjust contracts (static/dynamic pre-/postconditions, etc.);
   - etc.  */

# include <queue>

# include <mln/data/fill.hh>
# include <mln/level/extrema_components.hh>
# include <mln/morpho/lower_completion.hh>
# include <mln/trait/value_.hh>

namespace mln {

  namespace morpho {

    namespace internal {

      // FIXME: To be rewritten. Moreover, the distance d(p, q) is not
      // taken into account in this version of cost (but it should not
      // bother us as long as we are using first-order neighborhoods).
      template <typename I>
      int
      cost (const abstract::image_with_nbh<I>& im,
	    const oln_type_of(I, point)& p, const oln_type_of(I, point)& q)
      {
	mlc_is_a(I, abstract::scalar_valued_image)::ensure();

	oln_type_of(I, niter) n(im);

	int ls_p = 0;
	for_all_n_of_p (n, p)
	  if (im.hold(n) and int(im[p] - im[n]) > ls_p)
	    ls_p = im[p] - im[n];

	int ls_q = 0;
	for_all_n_of_p (n, q)
	  if (im.hold(n) and int(im[q] - im[n]) > ls_q)
	    ls_q = im[q] - im[n];

	if (im[p] > im[q])
	  return ls_p;
	else if (im[q] > im[p])
	  return ls_q;
	else
	  return (ls_p + ls_q) / 2;
      }


      /// Shortest path watershed transform implementation.
      template <typename DestValue, typename I>
      typename ch_value_type<I, DestValue>::ret
      shortest_path_watershed_(const abstract::image_with_nbh<I>& input)
      {
	// FIXME: Ensure the input image has scalar values.

	const DestValue wshed = min_val(DestValue);

	// We keep a track of already processed points.
	typename ch_value_type<I, bool>::ret processed (input.size(),
							input.nbh_get());
	data::fill (processed, false);

	// Initialize output with the minima components.
	typename ch_value_type<I, DestValue>::ret output =
	  level::minima_components<DestValue>(input);

	// Distance.
	typedef ntg_cumul_type(DestValue) cumul_type;
	typedef typename ch_value_type<I, cumul_type>::ret dist_type;
	dist_type dist (input.size(), input.nbh_get());
	data::fill(dist, ntg_max_val(DestValue));
	// Initialize distance with values of minima, and mark these
	// points as processed (remember that points of INPUT who have
	// a value greater than ntg_min_val(DestValue) belongs to a
	// minimum).
	oln_type_of(I, piter) p(input.size());
	for_all_p (p)
	  if (output[p] > ntg_min_val(DestValue))
	    dist[p] = input[p];

	// Ordered queue.
	typedef oln_type_of(I, point) point_type;
	typedef
	  std::priority_queue< point_type,
			       std::vector<point_type>,
			       util::greater_point<dist_type> >
	  ordered_queue_type;
	ordered_queue_type q(util::make_greater_point(dist));
	// Fill the ordered queue with the points of the border of the
	// minima of the (lower complete) input.
	for_all_p (p)
	  if (output[p] > ntg_min_val(DestValue))
	    {
	      bool border_p = false;
	      oln_type_of(I, niter) n(input);
	      for_all_n_of_p (n, p)
		if (input.hold(n) and
		    output[n] == ntg_min_val(DestValue))
		  {
		    // P is both in a minima and adjacent to a non minima:
		    // it's on the border of a minima.
		    border_p = true;
		    break;
		  };
	      if (border_p)
		q.push(p);
	      else
		// Inner points of minima have already been processed.
		processed[p] = true;
	    }

	while (!q.empty())
	  {
	    point_type p = q.top();
	    q.pop();
	    if (processed[p])
	      continue;
	    processed[p] = true;

	    oln_type_of(I, niter) n(input);
	    for_all_n_of_p (n, p)
	      {
		if (input.hold(n))
		  if (dist[p] + internal::cost(input, p, n) < dist[n])
		    {
		      dist[n] = dist[p] + internal::cost(input, p, n);
		      output[n] = output[p];
		      q.push(n);

		    }
		  else if (output[n] != wshed and
			   dist[p] + internal::cost(input, p, n) == dist[n] and
			   output[n] != output[p])
		    {
		      output[n] = wshed;
		      q.push(n);
		    }
	      }
	  }

	return output;
      }

    } // End of namespace mln::morpho::internal.


    /*! Watershed transform w.r.t. topographical distance based on
	image integration via the Dijkstra-Moore shortest path
	algorithm.

	This facade lets the user should whether the lower completion
	pass shoud be done before actually computing the watershed
	transform.  Unless you really what you're doing, you probably
	want to proceed to this lower completion.

	\param input              input image
	\param lower_completion_p whether \a input should be turned into a
				  lower complete image  */
    template <typename DestValue, typename I>
    typename ch_value_type<I, DestValue>::ret
    shortest_path_watershed(const abstract::image_with_nbh<I>& input,
			    bool lower_completion_p = true)
    {
      mlc_is_a(I, abstract::scalar_valued_image)::ensure();

      if (lower_completion_p)
	{
	  // Get a lower complete image of the input.
	  typedef ntg_cumul_type(DestValue) cumul_type;
	  typename ch_value_type<I, cumul_type>::ret lc =
	    lower_completion<cumul_type>(input);
	  return internal::shortest_path_watershed_<DestValue>(lc);
	}
      else
	return internal::shortest_path_watershed_<DestValue>(input);
    }

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_SHORTEST_PATH_WATERSHED_HH
