// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_MORPHO_ALGEBRAIC_FILTER_HH
# define MLN_MORPHO_ALGEBRAIC_FILTER_HH

/// \file
///
/// Alegebraic filter calls attribute canvas.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/concept/accumulator.hh>

# include <mln/trait/accumulators.hh>

# include <mln/data/sort_psites.hh>
# include <mln/data/sort_offsets.hh>

# include <mln/canvas/morpho/attribute_filter.hh>

namespace mln
{

  namespace morpho
  {

    /*!
     * \ingroup mlndatafilter
     */
    template <typename I, typename N, typename A>
    mln_concrete(I)
    algebraic_filter(const Image<I>& input,
		    const Neighborhood<N>& nbh,
		    const Accumulator<A>& a,
		    const typename A::result& lambda,
		    bool increasing);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename A>
    mln_concrete(I)
    algebraic_filter(const Image<I>& input,
		    const Neighborhood<N>& nbh,
		    const Accumulator<A>& a,
		    const typename A::result& lambda,
		    bool increasing)
    {
      trace::entering("morpho::opening::algebraic_filter");

      mln_precondition(exact(input).is_valid());

      mlc_or(mlc_equal(mln_trait_accumulator_when_pix(A),
		       trait::accumulator::when_pix::use_none),
	     mlc_equal(mln_trait_accumulator_when_pix(A),
		       trait::accumulator::when_pix::use_p))::check();

      mln_concrete(I) output;
      output = canvas::morpho::internal::attribute_filter_dispatch(input,
								   nbh,
								   a,
								   lambda,
								   increasing);

      trace::exiting("morpho::opening::algebraic_filter");

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_ALGEBRAIC_FILTER_HH
