// Copyright (C) 2005, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_MORPHO_COMPONENTS_HH
# define MLN_MORPHO_MORPHO_COMPONENTS_HH

// FIXME: This file seems to contain the same algorithm as
// mln/labeling/level.hh.  Merge or remove to avoid duplication.

// FIXME: Provide neighborhood-aware versions of these algorithms.

# include <queue>

# include <mln/trait/value_.hh>
# include <mln/trait/ch_value.hh>

# include <mln/level/fill.hh>

# include <mln/morpho/includes.hh>


namespace mln {

  namespace morpho {

    /** Compute the level components of \a input.

        \p DestValue is the value type of the output.

	The result is an image where each level component is given a
	value between \c mln_value(DestValue) and \c ncomps (the
	number of level components).

        \param[in]  input   The image whose minima are computed.
        \param[in]  win     The window describing the connexity of \a input.
        \param[out] ncomps  The number of minima found by the algorithm.
        \return             The image of components

	\pre \p DestValue is large enough to hold a number of values
	equal to \c nminima + 1, so as to tag all the minima points
	plus the non-minima points.  */
    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    level_components(const Image<I>& input, const Window<W>& win,
		     unsigned& ncomps);

    /// \brief Like the 3-argument version of mln::moprho::level_components,
    /// without the out argument \c ncomps.
    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    level_components(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    level_components(const Image<I>& input_, const Window<W>& win_,
		     unsigned& ncomps)
    {
      /* FIXME: Ensure the input image has scalar values.  */

      /* FIXME: Errors due to a too small DestValue type should be
	 reported more explicitely.  Currently, we abort with messages
	 akin to this one:

           Assertion failed:
           (unsigned(i) <= mln::trait::value_< enc_ >::max()),
           function operator=, file ../../../../milena/mln/value/int_u.hh,
           line 243.

	 This is not user-friendly at all!  */

      const I& input = exact(input_);
      const W& win = exact(win_);

      mln_ch_value(I, DestValue) labels(input.domain());
      mln_ch_value(I, bool) processed(input.domain());
      level::fill (processed, false);

      DestValue cur_label = mln_min(DestValue);

      typedef mln_point(I) point_type;
      std::queue<point_type> queue;

      mln_piter(I) p(input.domain());
      for_all(p)
	if (!processed(p))
	  {
	    labels(p) = cur_label;
	    processed(p) = true;
	    queue.push(p);
	    while (!queue.empty())
	      {
		point_type s = queue.front();
		queue.pop();
		mln_qiter(W) q(win, s);
		for_all(q)
		  if (input.has(q) and !processed(q) and input(q) == input(s))
		    {
		      labels(q) = cur_label;
		      processed(q) = true;
		      queue.push(q);
		    }
	      }
	    ++cur_label;
	  }
      // Update ncomps.
      ncomps = cur_label;
      return labels;
    }

    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    level_components(const Image<I>& input, const Window<W>& win)
    {
      // Dummy value.
      unsigned ncomps;
      return level_components<DestValue>(input, win, ncomps);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_MORPHO_COMPONENTS_HH
