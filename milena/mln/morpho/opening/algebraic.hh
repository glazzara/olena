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

#ifndef MLN_MORPHO_OPENING_ALGEBRAIC_HH
# define MLN_MORPHO_OPENING_ALGEBRAIC_HH

/// \file
///
/// Morphological algebraic opening.

# include <mln/morpho/includes.hh>
# include <mln/morpho/algebraic_filter.hh>


namespace mln
{

  namespace morpho
  {

    namespace opening
    {

      /*! \brief Morphological algebraic opening.
       *
       * \ingroup mlnmorphoopening
       */
      template <typename I, typename N, typename A>
      mln_concrete(I)
      algebraic(const Image<I>& input, const Neighborhood<N>& nbh,
		const Accumulator<A>& accu, const mln_result(A)& lambda);


# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename N, typename A>
    inline
    mln_concrete(I)
    algebraic(const Image<I>& input, const Neighborhood<N>& nbh,
	      const Accumulator<A>& accu, const mln_result(A)& lambda)
    {
      mln_trace("morpho::opening::algebraic");

      mln_precondition(exact(input).is_valid());
      mln_precondition(mlc_not_equal(mln_trait_accumulator_when_pix(A),
				     trait::accumulator::when_pix::not_ok)::value);

      mln_concrete(I) output;
      output = algebraic_filter(input, nbh, accu, lambda,
				/* increasing = */ false);

      mln_postcondition(output <= input);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::opening

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_OPENING_ALGEBRAIC_HH
