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

#ifndef MLN_MAKE_W_WINDOW_HH
# define MLN_MAKE_W_WINDOW_HH

/*! \file mln/make/w_window.hh
 *
 * \brief Routine to create a mln::w_window.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/concept/function.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace make
  {

    /*! \brief Create a mln::w_window from a window and a weight
     *  function.
     *
     * \param[in] win The window.
     * \param[in] wei A weight function.
     *
     * \return A  weighted window.
     */
    template <typename W, typename F>
    mln::w_window<mln_dpsite(W), mln_result(F)>
    w_window(const Window<W>& win, const Function_p2v<F>& wei);


# ifndef MLN_INCLUDE_ONLY

    template <typename W, typename F>
    inline
    mln::w_window<mln_dpsite(W), mln_result(F)>
    w_window(const Window<W>& win_, const Function_p2v<F>& wei_)
    {
      const W& win = exact(win_);
      const F& wei = exact(wei_);
      mln_precondition(! win.is_empty());
      typedef mln_dpsite(W) D;
      typedef mln_point(D)  P;
      mln::w_window<D, mln_result(F)> w_win;
      mln_qiter(W) q(win, P::origin);
      for_all(q)
	w_win.insert(wei(q), q - P::origin);
      return w_win;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_W_WINDOW_HH
