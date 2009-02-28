// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_MORPHO_LEVELING_FILTER_HH_
# define MLN_MORPHO_LEVELING_FILTER_HH_

/// \file mln/morpho/leveling_filter.hh
///
/// Leveling filter calls attribute canvas.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/accumulator.hh>

# include <mln/trait/accumulators.hh>

# include <mln/level/sort_psites.hh>
# include <mln/level/sort_offsets.hh>

# include <mln/canvas/morpho/attribute_filter.hh>

namespace mln {
  namespace morpho {

    template <typename I, typename N, typename A>
    mln_concrete(I)
    leveling_filter(const Image<I>& input,
		    const Neighborhood<N>& nbh,
		    const Accumulator<A>& a,
		    const typename A::result& lambda,
		    bool increasing);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename A>
    mln_concrete(I)
    leveling_filter(const Image<I>& input,
		    const Neighborhood<N>& nbh,
		    const Accumulator<A>& a,
		    const typename A::result& lambda,
		    bool increasing)
    {
      trace::entering("morpho::opening::leveling_filter");

      mln_precondition(exact(input).is_valid());

      mlc_or(mlc_equal(mln_trait_accumulator_when_pix(A),
		       trait::accumulator::when_pix::use_pix),
	     mlc_equal(mln_trait_accumulator_when_pix(A),
		       trait::accumulator::when_pix::use_v))::check();

      mln_concrete(I) output;
      output = canvas::morpho::internal::attribute_filter_dispatch(input, nbh, a, lambda, increasing);

      trace::exiting("morpho::opening::leveling_filter");

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY
  } // end of namespace mln::morpho
} // end of namespace mln


#endif /* !MLN_MORPHO_LEVELING_FILTER_HH_ */
