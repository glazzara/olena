// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_W_WINDOW_DIRECTIONAL_HH
# define MLN_MAKE_W_WINDOW_DIRECTIONAL_HH

/// \file
///
/// Routine to create a directional mln::w_window.

# include <mln/core/w_window.hh>
# include <mln/core/concept/gdpoint.hh>
# include <mln/literal/zero.hh>


namespace mln
{

  namespace make
  {

    /// Create a directional centered weighted window.
    ///
    /// \param[in] dp A delta-point to set the orientation. 
    /// \param[in] weights An array of weights.
    /// \return A weighted window.
    ///
    /// The window length \c L has to be odd.
    template <typename D, typename W, unsigned L>
    mln::w_window<D,W>
    w_window_directional(const Gdpoint<D>& dp, W (&weights)[L]);


# ifndef MLN_INCLUDE_ONLY

    template <typename D, typename W, unsigned L>
    inline
    mln::w_window<D,W>
    w_window_directional(const Gdpoint<D>& dp_, W (&weights)[L])
    {
      mlc_bool(L % 2 == 1)::check();
      int half = L / 2;

      const D& dp = exact(dp_);
      D zero = literal::zero;
      mln_precondition(dp != zero);

      mln::w_window<D,W> w_win;
      for (int i = - half; i <= half; ++i)
	if (weights[half + i] != 0)
	  w_win.insert(weights[half + i], zero + dp * i);
      return w_win;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_W_WINDOW_DIRECTIONAL_HH
