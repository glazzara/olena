// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CONVERT_TO_UPPER_WINDOW_HH
# define MLN_CONVERT_TO_UPPER_WINDOW_HH

/*! \file mln/convert/to_upper_window.hh
 *
 * \brief Conversions to upper mln::window.
 */

# include <mln/core/concept/delta_point_site.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/window.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a window \p nbh into an upper window.
    template <typename W>
    window<mln_dpoint(W)> to_upper_window(const Window<W>& win);

    /// Convert a neighborhood \p nbh into an upper window.
    template <typename N>
    window<mln_dpoint(N)> to_upper_window(const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

    /* FIXME: According to milena/core/concepts/README, windows are
       not necessarily based on a set of dpoints.  So the current
       algorithm won't work on non dpoint-set-based windows.  In the
       general case (of windows not being a set of dpoints), the
       window resulting from this conversion (as well as the iterators
       based on such windows!) should depend on the initial
       neighborhood (i.e., delegate the actual iteration to the
       aggregated neighborhood).  When this is fixed, document this in
       depth in milena/core/concepts/README.  */
    template <typename W>
    inline
    window<mln_dpoint(W)> to_upper_window(const Window<W>& win_)
    {
      const W& input_win = exact(win_);
      typedef mln_dpoint(W) D;
      typedef mln_point(D) P;
      window<D> win;
      mln_qiter(W) q(input_win, P::origin);
      for_all(q)
	if (q > P::origin)
	  win.insert(q - P::origin);
      return win;
    }

    template <typename N>
    inline
    window<mln_dpoint(N)> to_upper_window(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      typedef mln_dpoint(N) D;
      typedef mln_point(D) P;
      window<D> win;
      mln_niter(N) n(nbh, P::origin);
      for_all(n)
	if (n > P::origin)
	  win.insert(n - P::origin);
      return win;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_WINDOW_HH
