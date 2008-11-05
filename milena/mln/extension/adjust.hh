// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_EXTENSION_ADJUST_HH
# define MLN_CORE_EXTENSION_ADJUST_HH

/// \file mln/extension/adjust.hh
///
/// Adjust the domain extension to a minimum thickness border.
///
/// \todo Fix doc.

# include <mln/border/adjust.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/geom/delta.hh>


namespace mln
{

  namespace extension
  {

    /// Adjust the domain extension of image \p ima with the
    /// size of the window \p win.
    template <typename I, typename W>
    void adjust(const Image<I>& ima, const Window<W>& win);

    /// Adjust the domain extension of image \p ima with the
    /// size of the weighted window \p wwin.
    template <typename I, typename W>
    void adjust(const Image<I>& ima, const Weighted_Window<W>& wwin);

    /// Adjust the domain extension of image \p ima with the
    /// size of the neighborhood \p nbh.
    template <typename I, typename N>
    void adjust(const Image<I>& ima, const Neighborhood<N>& nbh);

    /// Adjust the domain extension of image \p ima with the
    /// size \p delta.
    template <typename I>
    void adjust(const Image<I>& ima, unsigned delta);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      void adjust(const I& ima, unsigned delta)
      {
	mln_precondition(exact(ima).has_data());
	// FIXME: Is-it right?
	border::adjust(ima, delta);
      }

    } // end of namespace mln::extension::impl


    // Facades.

    template <typename I, typename W>
    void adjust(const Image<I>& ima, const Window<W>& win)
    {
      trace::entering("extension::adjust");
      impl::adjust(ima, geom::delta(win));
      trace::exiting("extension::adjust");
    }

    template <typename I, typename W>
    void adjust(const Image<I>& ima, const Weighted_Window<W>& wwin)
    {
      trace::entering("extension::adjust");
      impl::adjust(ima, geom::delta(wwin));
      trace::exiting("extension::adjust");
    }

    template <typename I, typename N>
    void adjust(const Image<I>& ima, const Neighborhood<N>& nbh)
    {
      trace::entering("extension::adjust");
      impl::adjust(ima, geom::delta(nbh));
      trace::exiting("extension::adjust");
    }

    template <typename I>
    void adjust(const Image<I>& ima, unsigned delta)
    {
      trace::entering("extension::adjust");
      impl::adjust(ima, delta);
      trace::exiting("extension::adjust");
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::extension

} // end of namespace mln


#endif // ! MLN_CORE_EXTENSION_ADJUST_HH
