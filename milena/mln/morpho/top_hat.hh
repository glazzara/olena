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

#ifndef MLN_MORPHO_TOP_HAT_HH
# define MLN_MORPHO_TOP_HAT_HH

/*! \file mln/morpho/top_hat.hh
 *
 * \brief Morphological top-hats.
 *
 * \todo Save memory.
 */

# include <mln/morpho/opening.hh>
# include <mln/morpho/closing.hh>


namespace mln
{

  namespace morpho
  {

    /*! Morphological white top-hat (for object / light objects).
     *
     * This operator is Id - ope_B. 
     */
    template <typename I, typename W, typename O>
    void top_hat_white(const Image<I>& input, const Window<W>& win,
		       Image<O>& output);


    /*! Morphological black top-hat (for background / dark objects).
     *
     * This operator is clo_B - Id.
     */
    template <typename I, typename W, typename O>
    void top_hat_black(const Image<I>& input, const Window<W>& win,
		       Image<O>& output);


    /*! Morphological self-complementary top-hat.
     *
     * This operator is \n
     *   = top_hat_white + top_hat_black \n
     *   = (input - opening) + (closing - input) \n
     *   = closing - opening. \n
     */
    template <typename I, typename W, typename O>
    void top_hat_self_complementary(const Image<I>& input, const Window<W>& win,
				    Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename W, typename O>
    void top_hat_white(const Image<I>& input_, const Window<W>& win_, Image<O>& output_)
    {
      const I& input = exact(input_);
      const W& win = exact(win_);
      O& output = exact(output_);

      mln_precondition(output.domain() == input.domain());
      mln_precondition(! win.is_empty());

      O temp(input.domain());
      opening(input, win, temp); // temp = opening
      arith::minus(input, temp, output); // output = input - opening

      mln_postcondition(test::positive(output));
    }

    template <typename I, typename W, typename O>
    void top_hat_black(const Image<I>& input_, const Window<W>& win_, Image<O>& output_)
    {
      const I& input = exact(input_);
      const W& win = exact(win_);
      O& output = exact(output_);

      mln_precondition(output.domain() == input.domain());
      mln_precondition(! win.is_empty());

      closing(input, win, output); // output = closing
      arith::minus_inplace(output, input); // now output = closing - input

      mln_postcondition(test::positive(output));
    }

    template <typename I, typename W, typename O>
    void top_hat_self_complementary(const Image<I>& input_, const Window<W>& win_, Image<O>& output_)
    {
      const I& input = exact(input_);
      const W& win = exact(win_);
      O& output = exact(output_);

      mln_precondition(output.domain() == input.domain());
      mln_precondition(! win.is_empty());

      closing(input, win, output); // output = closing
      O temp(input.domain());
      opening(input, win, temp); // temp = opening
      arith::minus_inplace(output, temp); // now output = closing - opening

      mln_postcondition(test::positive(output));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_TOP_HAT_HH
