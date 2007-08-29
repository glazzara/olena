// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_MORPHO_CONTRAST_HH
# define MLN_MORPHO_CONTRAST_HH

/*! \file mln/morpho/contrast.hh
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

    /*! Morphological contrast operator (based on top-hats).
     *
     * This operator is Id + wth_B - bth_B. 
     */
    template <typename I, typename W, typename O>
    void contrast(const Image<I>& input, const Window<W>& win,
		  Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename W, typename O>
    void contrast(const Image<I>& input_, const Window<W>& win_, Image<O>& output_)
    {
      const I& input = exact(input_);
      const W& win = exact(win_);
      O& output = exact(output_);

      mln_precondition(output.domain() == input.domain());
      mln_precondition(! win.is_empty());

      top_hat_white(input, win, output); // output = wth
      morpho::plus_inplace(output, input); // now output = wth + input
      O temp(input.domain());
      top_hat_black(input, win, temp); // temp = bth
      morpho::minus_inplace(output, temp); // now output = wth + input - bth
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_CONTRAST_HH
