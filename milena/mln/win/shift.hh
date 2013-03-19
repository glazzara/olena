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

#ifndef MLN_WIN_SHIFT_HH
# define MLN_WIN_SHIFT_HH

/// \file
///
/// Define a function which shifts a window with a delta-point.

# include <mln/core/window.hh>



namespace mln
{

  namespace win
  {

    /// Shift a window \p win with a delta-point \p dp.
    template <typename W>
    mln_regular(W)
    shift(const Window<W>& win, const mln_dpsite(W)& dp);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename W>
      inline
      mln_regular(W)
      shift_(trait::window::definition::unique,
	     const W& win, const mln_dpsite(W)& dp)
      {
	mlc_is(mln_trait_window_size(W),
	       trait::window::size::fixed)::check();
	mln_regular(W) tmp;
	unsigned n = win.size();
	for (unsigned i = 0; i < n; ++i)
	  tmp.insert(win.dp(i) + dp);
	return tmp;
      }

      template <typename W>
      inline
      mln_regular(W)
      shift_(trait::window::definition::multiple,
	     const W& win, const mln_dpsite(W)& dp)
      {
	mln_regular(W) tmp(win.function());
	const unsigned nw = win.nwindows();
	for (unsigned w = 0; w < nw; ++w)
	  tmp.set_window(w, win::shift(win.window(w), dp));
	return tmp;
      }

    } // end of namespace mln::win::impl


    // Facade.
    template <typename W>
    inline
    mln_regular(W)
    shift(const Window<W>& win, const mln_dpsite(W)& dp)
    {
      mln_trace("win::shift");

      mlc_is(mln_trait_window_support(W),
	     trait::window::support::regular)::check();
      mlc_is_not(mln_trait_window_definition(W),
		 trait::window::definition::varying)::check();

      mln_regular(W) tmp = impl::shift_(mln_trait_window_definition(W)(),
					exact(win), dp);

      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::win

} // end of namespace mln


#endif // ! MLN_WIN_SHIFT_HH
