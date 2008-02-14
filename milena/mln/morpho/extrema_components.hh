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

#ifndef MLN_MORPHO_EXTREMA_COMPONENTS_HH
# define MLN_MORPHO_EXTREMA_COMPONENTS_HH

// FIXME: This file seems to contain the same algorithm as
// mln/labeling/regional_minima.hh and
// mln/labeling/regional_maxima.hh.  Merge or remove to avoid
// duplication.

// FIXME: Provide neighborhood-aware versions of these algorithms.

# include <map>
# include <queue>
# include <functional>

# include <mln/trait/ch_value.hh>

# include <mln/level/fill.hh>

# include <mln/morpho/includes.hh>
# include <mln/morpho/level_components.hh>


namespace mln {

  namespace morpho {

    /*--------------------------.
    | Minima level components.  |
    `--------------------------*/

    /** \brief Compute the minima level components of \a input.

        \p DestValue is the value type of the output.

	The result is an image where each minima is given a value
	between \c mln_value(DestValue) + 1 and \c nminima (the number
	of minima level components).  Points which do not belong to a
	minimum are gien the value mln_value(DestValue).

        \param[in]  input    The image whose minima are computed.
        \param[in]  win      The window describing the connexity of \a input.
        \param[out] nminima  The number of minima found by the algorithm.
        \return              The image of minima.

	\pre \p DestValue is large enough to hold a number of values
	equal to \c nminima + 1, so as to tag all the minima points
	plus the non-minima points.  */
    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    minima_components(const Image<I>& input, const Window<W>& win,
		      DestValue& nminima);

    /// \brief Like the 3-argument version of mln::moprho::minima_components,
    /// without the out argument \c nminima.
    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    minima_components(const Image<I>& input, const Window<W>& win);


    /*--------------------------.
    | Maxima level components.  |
    `--------------------------*/

    /** \brief Compute the maxima level components of \a input.

        \p DestValue is the value type of the output.

	The result is an image where each maxima is given a value
	between \c mln_value(DestValue) + 1 and \c nmaxima (the number
	of maxima level components).  Points which do not belong to a
	maximum are gien the value mln_value(DestValue).

        \param[in]  input    The image whose maxima are computed.
        \param[in]  win      The window describing the connexity of \a input.
        \param[out] nmaxima  The number of maxima found by the algorithm.
        \return              The image of maxima.

	\pre \p DestValue is large enough to hold a number of values
	equal to \c nmaxima + 1, so as to tag all the maxima points
	plus the non-maxima points.  */
    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    maxima_components(const Image<I>& input, const Window<W>& win,
		      DestValue& nmaxima);

    /// \brief Like the 3-argument version of mln::moprho::maxima_components,
    /// without the out argument \c nmaxima.
    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    maxima_components(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    namespace impl {

      /// Find the extrema level components of \a input, using \param
      /// Cmp as comparison function.
      template <typename DestValue, typename Cmp, typename I, typename W>
      mln_ch_value(I, DestValue)
      extrema_components(const Image<I>& input_, const Window<W>& win_,
			 DestValue& nextrema)
      {
	
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

	// Instantiate a comparison functor.
	Cmp cmp;

	// Compute level components.
	/* FIXME: Should we deduce cumul_type from DestValue?  Or
	   stick to unsigned?  */
	typedef unsigned cumul_type;
	typedef cumul_type comp_type;
	mln_ch_value(I, comp_type) comps =
	  level_components<comp_type>(input, win);
	/* FIXME: Does Milena provide better data structures to
	   store this information?  */
	std::set<comp_type> extrema;
	std::set<comp_type> non_extrema;

	// Search extrema components.
	mln_piter(I) p(input.domain());
	for_all(p)
	  {
	    comp_type comp = comps(p);
	    if (non_extrema.find(comp) == non_extrema.end())
	      {
		// A new level is a (potential) extrema by default.
		extrema.insert(comp);

		mln_qiter(W) q(win, p);
		for_all(q)
		{
		  if (input.has(q) and cmp(input(q), input(p)))
		    {
		      extrema.erase(comp);
		      non_extrema.insert(comp);
		      break;
		    }
		}
	      }
	  }

	// Update nextrema.
	nextrema = extrema.size();

	// Re-label the extrema.  label_map hold the assigned labels.
	std::map<comp_type, DestValue> label_map;
	{
	  DestValue cur_label = mln_min(DestValue) + 1;
	  for (typename std::set<comp_type>::const_iterator i = extrema.begin();
	       i != extrema.end();
	       ++i, ++cur_label)
	    label_map[*i] = cur_label;
	}
	mln_ch_value(I, DestValue) output(input.domain());
	level::fill (output, mln_min(DestValue));
	for_all(p)
	{
	  comp_type comp = comps(p);
	  if (label_map.find(comp) != label_map.end())
	    output(p) = label_map[comp];
	}
	return output;
      }

    } // end of namespace mln::morpho::impl.


    /*----------.
    | Facades.  |
    `----------*/

    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    minima_components(const Image<I>& input, const Window<W>& win,
		      DestValue& nminima)
    {
      /* FIXME: Ensure the input image has scalar values.  */
      typedef std::less< mln_value(I) > cmp_t;
      return impl::extrema_components<DestValue, cmp_t>(input, win, nminima);
    }

    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    minima_components(const Image<I>& input, const Window<W>& win)
    {
      // Dummy value.
      DestValue nminima;
      return minima_components<DestValue>(input, win, nminima);
    }


    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    maxima_components(const Image<I>& input, const Window<W>& win,
		      DestValue& nmaxima)
    {
      /* FIXME: Ensure the input image has scalar values.  */
      typedef std::greater< mln_value(I) > cmp_t;
      return impl::extrema_components<DestValue, cmp_t>(input, win, nmaxima);
    }

    template <typename DestValue, typename I, typename W>
    mln_ch_value(I, DestValue)
    maxima_components(const Image<I>& input, const Window<W>& win)
    {
      // Dummy value.
      DestValue nmaxima;
      return maxima_components<DestValue>(input, win, nmaxima);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho.

} // end of namespace mln.

#endif // ! MLN_MORPHO_EXTREMA_COMPONENTS_HH
