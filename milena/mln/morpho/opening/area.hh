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

#ifndef MLN_MORPHO_OPENING_AREA_HH
# define MLN_MORPHO_OPENING_AREA_HH

/// \file
///
/// Morphological area opening.

# include <mln/morpho/opening/algebraic.hh>
# include <mln/morpho/attribute/card.hh>


namespace mln
{

  namespace morpho
  {

    namespace opening
    {

      /*! \brief Morphological area opening.
       *
       * \ingroup mlnmorphoopening
       */
      template <typename I, typename N>
      mln_concrete(I)
      area(const Image<I>& input, const Neighborhood<N>& nbh,
	   unsigned lambda);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename N>
      inline
      mln_concrete(I)
      area(const Image<I>& input, const Neighborhood<N>& nbh,
	   unsigned lambda)
      {
	trace::entering("morpho::opening::area");

	mln_precondition(exact(input).is_valid());

	mln_concrete(I) output;
	output = opening::algebraic(input, nbh, attribute::card<I>(), lambda);

	trace::exiting("morpho::opening::area");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::opening

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_OPENING_AREA_HH
