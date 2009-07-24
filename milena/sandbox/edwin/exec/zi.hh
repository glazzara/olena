// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef ZI_HH
# define ZI_HH


// From mln/canvas/distance_geodesic.hh

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/routine/duplicate.hh>
# include <mln/core/site_set/p_queue_fast.hh>
# include <queue>
# include <mln/data/fill.hh>
# include <mln/extension/adjust_fill.hh>


// Local.

#include <map>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pbm/load.hh>
#include <mln/labeling/blobs.hh>
#include <mln/debug/println.hh>




namespace mln
{

  // Theo's code

  // From  mln/canvas/distance_geodesic.hh
  //  and  mln/transform/internal/influence_zone_functor.hh
  //  and  mln/transform/influence_zone_geodesic.hh


  template <typename I, typename N>
  std::map< std::pair<mln_value(I),mln_value(I)>, unsigned >
  distances(// in:
	    const I& input, const N& nbh,
	    // out:
	    mln_ch_value(I, unsigned)& dmap,
	    mln_concrete(I)& iz)
  {
    trace::entering("canvas::impl::generic::distance_geodesic");

    typedef mln_value(I) L;
    std::map< std::pair<L,L>, unsigned > dist; // NEW

    const unsigned max = mln_max(unsigned);
    typedef mln_site(I) P;
    p_queue_fast<P> q;

    // Initialization.
    {
      iz = duplicate(input); // <-- init

      data::fill(dmap, max);

      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
	if (input(p) != 0) // <-- inqueue_p_wrt_input_p
	  {
	    ; // <-- init_p
	    dmap(p) = 0;
	    for_all(n)
	      if (input.domain().has(n) &&
		  input(n) == 0) // <-- inqueue_p_wrt_input_n
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
	  if (dmap(p) == max)
	    {
	      // Saturation so stop.
	      q.clear();
	      break;
	    }
	  for_all(n)
	  {
	    if (! input.domain().has(n))
	      continue;
	    if (dmap(n) == max)
	      {
		dmap(n) = dmap(p) + 1;
		iz(n) = iz(p); // <- process
		q.push(n);
	      }
	    else
	      {
		if (iz(n) != iz(p))
		  {
		    L l1 = iz(n), l2 = iz(p);
		    if (l1 > l2)
		      std::swap(l1, l2);
		    unsigned& d_ = dist[std::make_pair(l1,l2)];
		    unsigned d12 = dmap(p) + dmap(n) + 1;
		    if (d_ == 0 || d12 < d_)
		      d_ = d12;
		  }
	      }
	  }
	}
    }

    return dist;
  }

} // mln

# ifndef MLN_INCLUDE_ONLY

# endif // ! MLN_INCLUDE_ONLY

#endif // ! ZI_HH
