// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MORPHO_VECTOR_MEDIAN_HH
# define MLN_MORPHO_VECTOR_MEDIAN_HH

/// \file mln/morpho/vector_median.hh
///
/// Morphological vector median filter.
///
/// \todo The overloads are hidden and I don't know why!

# include <mln/morpho/includes.hh>
# include <mln/morpho/general.hh>
# include <mln/accu/land.hh>
# include <mln/accu/land_basic.hh>
# include <mln/accu/stat/min.hh>
# include <mln/accu/stat/min_h.hh>
# include <mln/norm/l2.hh>

namespace mln
{

  namespace morpho
  {

    /// Morphological vector_median.
    template <typename I, typename W>
    mln_concrete(I)
    vector_median(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // On set with centered window (overloads).

      template <typename I, typename W>
      mln_concrete(I)
      vector_median(const Image<I>& input_, const Window<W>& win_)
      {
	trace::entering("morpho::impl::general_on_set_centered__vector_median");

	typedef mln_concrete(I) O;
	const I& input = exact(input_);
	const W& win = exact(win_);

	O output;
	output = duplicate(input);

	mln_piter(I) p(input.domain());
	mln_qiter(W) q(win, p);
	mln_qiter(W) r(win, p);
	for_all(p)
	{
	  mln_psite(W) v;
	  double min_dist = std::numeric_limits<double>::infinity();
	  for_all(q) if (input.domain().has(q))
	  {
	    double dist = norm::l2(input(p) - input(q));
	    for_all(r) if (input.domain().has(r) && q!=r)
	      dist += norm::l2(input(r) - input(q));
	    if (dist < min_dist)
	    {
	      min_dist = dist;
	      v = q;
	    }
	  }
	  output(p) = input(v);
	}

	trace::exiting("morpho::impl::general_on_set_centered__vector_median");
	return output;
      }


    } // end of namespace morpho::impl


    template <typename I, typename W>
    inline
    mln_concrete(I)
    vector_median(const Image<I>& input, const Window<W>& win)
    {
      trace::entering("morpho::vector_median");
      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output = impl::vector_median(input, win);

      trace::exiting("morpho::vector_median");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_VECTOR_MEDIAN_HH
