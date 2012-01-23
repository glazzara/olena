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

#ifndef MLN_MORPHO_CONTRAST_HH
# define MLN_MORPHO_CONTRAST_HH

/*! \file
 *
 * \brief Morphological contrast operator (based on top-hats).
 *
 * \todo Save memory.
 */

# include <mln/morpho/top_hat.hh>


namespace mln
{

  namespace morpho
  {

    /*! \brief Morphological contrast operator (based on top-hats).
     *
     * This operator is Id + wth_B - bth_B.
     *
     * \ingroup mlnmorpho
     */
    template <typename I, typename W>
    mln_concrete(I) contrast(const Image<I>& input, const Window<W>& win);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename W>
    inline
    mln_concrete(I) contrast(const Image<I>& input, const Window<W>& win)
    {
      trace::entering("morpho::contrast");
      mln_precondition(exact(input).is_valid());
      mln_precondition(! exact(win).is_empty());

      mln_concrete(I) output =
	arith::plus( input,
		     top_hat_white(input, win) - top_hat_black(input, win),
		     fun::v2v::saturate<mln_value(I)>() );

      trace::exiting("morpho::contrast");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_CONTRAST_HH
