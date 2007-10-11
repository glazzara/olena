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

#ifndef MLN_CONVERT_TO_W_WINDOW_HH
# define MLN_CONVERT_TO_W_WINDOW_HH

/*! \file mln/convert/to_w_window.hh
 *
 * \brief Conversions to mln::w_window.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/window.hh>


namespace mln
{

  namespace convert
  {

    /// Convert an image \p input into a weighted window.
    template <typename I>
    w_window<mln_dpoint(I), mln_value(I)> to_w_window(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    w_window<mln_dpoint(I), mln_value(I)> to_w_window(const Image<I>& input_)
    {
      const I& input = exact(input_);
      mln_precondition(input.has_data());
      // FIXME: Check that input is scalar?
      typedef mln_dpoint(I) D;
      typedef mln_point(D) P;
      w_window<D, mln_value(I)> w_win;
      mln_piter(I) p(input.domain());
      for_all(p)
	if (input(p) != 0)
	  w_win.insert(input(p), p - P::origin);
      return w_win;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_W_WINDOW_HH
