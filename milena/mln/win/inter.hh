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

#ifndef MLN_WIN_INTER_HH
# define MLN_WIN_INTER_HH

/// \file
///
/// \brief Define a function which inters a window with a delta-point.

# include <mln/core/window.hh>



namespace mln
{

  /// Inter a window \p win with a delta-point \p dp.
  template <typename W1, typename W2>
  mln_regular(W1)
  operator && (const Window<W1>& win1, const Window<W2>& win2);


# ifndef MLN_INCLUDE_ONLY

  namespace win
  {

    namespace impl
    {

      template <typename W1, typename W2>
      inline
      mln_regular(W1)
      inter(trait::window::definition::unique,
	    const W1& win1, const W2& win2)
      {
	mlc_is(mln_trait_window_size(W1),
	       trait::window::size::fixed)::check();
	mlc_is(mln_trait_window_size(W2),
	       trait::window::size::fixed)::check();

	mln_regular(W1) tmp;
	unsigned n = win1.size();
	for (unsigned i = 0; i < n; ++i)
	  if (win2.has(win1.dp(i)))
	    tmp.insert(win1.dp(i));
	return tmp;
      }

      template <typename W1, typename W2>
      inline
      mln_regular(W1)
      inter(trait::window::definition::multiple,
	    const W1& win1, const W2& win2)
      {
	mln_precondition(win1.nwindows() == win2.nwindows());
	// FIXME: What about "win1.function() not equal win2.function()"!

	mln_regular(W1) tmp(win1.function());
	const unsigned nw = win1.nwindows();
	for (unsigned w = 0; w < nw; ++w)
	  tmp.set_window(w, win1.window(w) && win2.window(w));
	return tmp;
      }

    } // end of namespace mln::win::impl

  } // end of namespace mln::win


  // Facade.
  template <typename W1, typename W2>
  inline
  mln_regular(W1)
  operator && (const Window<W1>& win1, const Window<W2>& win2)
  {
    mln_trace("win::inter");

    // Compatibility checks.
    mlc_equal(mln_regular(W1), mln_regular(W2))::check();
    mlc_converts_to(mln_dpsite(W1), mln_dpsite(W2))::check();

    // Support checks.
    mlc_is(mln_trait_window_support(W1),
	   trait::window::support::regular)::check();
    mlc_is(mln_trait_window_support(W2),
	   trait::window::support::regular)::check();

    // Definition checks.
    mlc_equal(mln_trait_window_definition(W1),
	      mln_trait_window_definition(W2))::check();

    mlc_is_not(mln_trait_window_definition(W1),
	       trait::window::definition::varying)::check();

    mln_regular(W1) tmp = win::impl::inter(mln_trait_window_definition(W1)(),
					   exact(win1), exact(win2));

    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_WIN_INTER_HH
