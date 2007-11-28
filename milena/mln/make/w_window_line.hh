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

#ifndef MLN_MAKE_W_WINDOW_LINE_HH
# define MLN_MAKE_W_WINDOW_LINE_HH

/*! \file mln/make/w_window_line.hh
 *
 * \brief Routine to create an horizontal mln::w_window.
 */

# include <mln/core/w_window.hh>
# include <mln/literal/zero.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create an horizontal centered and symmetrical
     *  mln::w_window.
     *
     * The free parameter \c D is a type of delta-point. 
     *
     * \pre The window length \c L has to be odd.
     *
     * \return A window.
     */
    template <typename D, typename W, unsigned L>
    mln::w_window<D,W> w_window_line(W (&w)[L]);


# ifndef MLN_INCLUDE_ONLY

    template <typename D, typename W, unsigned L>
    inline
    mln::w_window<D,W> w_window_line(W (&w)[L])
    {
      mln_precondition(L % 2 == 1);
      mln::w_window<D,W> w_win;
      D dp = literal::zero;
      for (unsigned i = 0; i < L; ++i)
	{
	  dp[D::dim - 1] = i - L / 2;
	  w_win.insert(w[i], dp);
	}
      return w_win;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_W_WINDOW_LINE_HH
