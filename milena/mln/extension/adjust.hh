// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_EXTENSION_ADJUST_HH
# define MLN_EXTENSION_ADJUST_HH

/// \file
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

    /*! \brief Adjust the domain extension of image \p ima with the
     * size of the window \p win.
     *
     * \ingroup mlnborderext
     */
    template <typename I, typename W>
    void adjust(const Image<I>& ima, const Window<W>& win);

    /*! \brief Adjust the domain extension of image \p ima with the
     * size of the weighted window \p wwin.
     *
     * \ingroup mlnborderext
     */
    template <typename I, typename W>
    void adjust(const Image<I>& ima, const Weighted_Window<W>& wwin);

    /*! \brief Adjust the domain extension of image \p ima with the
     *  size of the neighborhood \p nbh.
     *
     * \ingroup mlnborderext
     */
    template <typename I, typename N>
    void adjust(const Image<I>& ima, const Neighborhood<N>& nbh);

    /*! \brief Adjust the domain extension of image \p ima with the
     *  size \p delta.
     *
     * \ingroup mlnborderext
     */
    template <typename I>
    void adjust(const Image<I>& ima, unsigned delta);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      void adjust(const I& ima, unsigned delta)
      {
	mln_precondition(exact(ima).is_valid());
	// FIXME: Is-it right?
	border::adjust(ima, delta);
      }

    } // end of namespace mln::extension::impl


    // Facades.

    template <typename I, typename W>
    void adjust(const Image<I>& ima, const Window<W>& win)
    {
      mln_trace("extension::adjust");
      impl::adjust(ima, geom::delta(win));
    }

    template <typename I, typename W>
    void adjust(const Image<I>& ima, const Weighted_Window<W>& wwin)
    {
      mln_trace("extension::adjust");
      impl::adjust(ima, geom::delta(wwin));
    }

    template <typename I, typename N>
    void adjust(const Image<I>& ima, const Neighborhood<N>& nbh)
    {
      mln_trace("extension::adjust");
      impl::adjust(ima, geom::delta(nbh));
    }

    template <typename I>
    void adjust(const Image<I>& ima, unsigned delta)
    {
      mln_trace("extension::adjust");
      impl::adjust(ima, delta);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::extension

} // end of namespace mln


#endif // ! MLN_EXTENSION_ADJUST_HH
