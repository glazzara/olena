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

#ifndef MLN_CANVAS_DISTANCE_FRONT_HH
# define MLN_CANVAS_DISTANCE_FRONT_HH

/// \file mln/canvas/distance_front.hh
///
/// Discrete distance canvas by front propagation.

# include <vector>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/data/fill.hh>
# include <mln/accu/max.hh>


namespace mln
{

  namespace canvas
  {

    /// Discrete distance canvas by front propagation.
    template <typename I,
	      typename N, typename W, typename D,
	      typename F>
    mln_ch_value(I, D)
      distance_front(const Image<I>& input,
		     const Neighborhood<N>& nbh, const Weighted_Window<W>& w_win, D max,
		     F& functor);


# ifndef MLN_INCLUDE_ONLY


    template <typename I,
	      typename N, typename W, typename D,
	      typename F>
    inline
    mln_ch_value(I, D)
      distance_front(const Image<I>& input_,
		     const Neighborhood<N>& nbh_, const Weighted_Window<W>& w_win_, D max,
		     F& functor)
    {
      trace::entering("canvas::distance_front");

      const I& input = exact(input_);
      const N& nbh   = exact(nbh_);
      const W& w_win = exact(w_win_);

      mln_precondition(input.has_data());
      // mln_precondition(w_win.is_valid());

      typedef mln_site(I) P;
      typedef std::vector<P> bucket_t;

      // Distance map.
      mln_ch_value(I, D) dmap;
      initialize(dmap, input);
      data::fill(dmap, max);

      // Mod determination.
      unsigned mod;
      {
	accu::max<unsigned> m;
	for (unsigned i = 0; i < w_win.size(); ++i)
	  m.take(w_win.w(i));
	mod = unsigned(m) + 1;
      }

      // Aux data.
      std::vector<bucket_t> bucket(mod);
      unsigned bucket_size = 0;

      // Initialization.
      {
	functor.init(input); // <-- init
	mln_piter(I) p(input.domain());
	mln_niter(N) n(nbh, p);
	for_all(p)
	  if (functor.inqueue_p_wrt_input_p(input(p))) // <-- inqueue_p_wrt_input_p
	    {
	      dmap(p) = 0;
	      for_all(n)
		if (input.domain().has(n) &&
		    functor.inqueue_p_wrt_input_n(input(n))) // <-- inqueue_p_wrt_input_n
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
	    
		if (dmap(p) == max)
		  {
		    // Saturation so stop.
		    bucket_size = bucket_d.size(); // So at end bucket_size == 0.
		    break;
		  }

		if (dmap(p) < d)
		  // p has already been processed, having a distance less than d.
		  continue;

		for_all(q)
		  if (dmap.domain().has(q) && dmap(q) > d)
		    {
		      unsigned d_ = d + q.w();
		      if (d_ < dmap(q))
			{
			  dmap(q) = d_;
			  functor.process(p, q); // <- process
			  bucket[d_ % mod].push_back(q);
			  ++bucket_size;
			}
		    }
	      }
	    bucket_size -= bucket_d.size();
	    bucket_d.clear();
	  }

	trace::exiting("canvas::distance_front");
	return dmap;
      }
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_DISTANCE_FRONT_HH
