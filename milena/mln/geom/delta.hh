// Copyright (C) 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_GEOM_DELTA_HH
# define MLN_GEOM_DELTA_HH

/// \file mln/geom/delta.hh
///
/// \todo Add doc.


# include <mln/core/concept/window.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/core/concept/neighborhood.hh>


namespace mln
{

  namespace geom
  {

    /// Compute the delta of a window \p win.
    template <typename W>
    unsigned delta(const Window<W>& win);

    /// Compute the delta of a weighted window \p wwin.
    template <typename W>
    unsigned delta(const Weighted_Window<W>& wwin);

    /// Compute the delta of a neighborhood \p nbh.
    template <typename N>
    unsigned delta(const Neighborhood<N>& nbh);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename W>
      unsigned delta_dispatch(trait::window::support::any,
			      trait::window::definition::any,
			      const W& win)
      {
	(void) win;
	return 0;
      }

      template <typename W>
      unsigned delta_dispatch(trait::window::support::regular,
			      trait::window::definition::varying,
			      const W& win)
      {
	(void) win;
	return 0;
      }

      template <typename W>
      unsigned delta_dispatch(trait::window::support::regular,
			      trait::window::definition::any, // So not varying.
			      const W& win)
      {
	return win.delta();
      }

    } // end of namespace mln::geom::internal


    // Facades.

    template <typename W>
    unsigned delta(const Window<W>& win)
    {
      // mln_precondition(exact(win).is_valid());
      return internal::delta_dispatch(mln_trait_window_support(W)(),
				      mln_trait_window_definition(W)(),
				      exact(win));
    }

    template <typename W>
    unsigned delta(const Weighted_Window<W>& wwin)
    {
      // mln_precondition(exact(wwin).is_valid());
      return delta(exact(wwin).win());
    }

    template <typename N>
    unsigned delta(const Neighborhood<N>& nbh)
    {
      // mln_precondition(exact(nbh).is_valid());
      return delta(exact(nbh).win());
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

} // end of namespace mln


#endif // ! MLN_GEOM_DELTA_HH
