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

#ifndef MLN_MORPHO_THICKENING_HH
# define MLN_MORPHO_THICKENING_HH

/*! \file mln/morpho/thickening.hh
 *
 * \brief Morphological thickening.
 */

# include <mln/morpho/hit_or_miss.hh>


namespace mln
{

  namespace morpho
  {


    /*! Morphological thickening.
     *
     * This operator is THICK_B = Id + HMT_B, where B = (Bfg, Bbg).
     */
    template <typename I, typename Wfg, typename Wbg>
    mln_concrete(I)
      thickening(const Image<I>& input,
		 const Window<Wfg>& win_fg, const Window<Wbg>& win_bg);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename Wfg, typename Wbg>
    inline
    mln_concrete(I)
      thickening(const Image<I>& input,
		 const Window<Wfg>& win_fg, const Window<Wbg>& win_bg)
    {
      trace::entering("morpho::thickening");
      mln_precondition(exact(input).has_data());
      mln_precondition(exact(win_bg).is_centered());
      mln_precondition(! exact(win_fg).is_empty());
      mln_precondition(set::inter(exact(win_fg), exact(win_bg)).is_empty());

      mln_concrete(I) output = morpho::plus( input,
					     hit_or_miss(input, win_fg, win_bg) );

      trace::exiting("morpho::thickening");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_THICKENING_HH
