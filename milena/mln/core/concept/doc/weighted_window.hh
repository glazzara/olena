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

/*! \file mln/core/concept/doc/weighted_window.hh
 * \brief This file documents the concept of mln::Weighted_Window.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Weighted_Window.
     *
     * A weighted_window is the definition of a set of points located
     * around a central point, with a weight associated to each point.
     *
     * \see mln::Weighted_Window
     */
    template <typename E>
    struct Weighted_Window : public Object<E>
    {

      /*! \brief Point_Iterator type associated to this weighted_window to browse its
       * points in a forward way.
       */
      typedef void fwd_qiter;

      /*! \brief Point_Iterator type associated to this weighted_window to browse its
       * points in a backward way.
       */
      typedef void bkd_qiter;

      /// Point associated type.
      typedef void point;

      /// Dpoint associated type.
      typedef void dpoint;

      /// Weight associated type.
      typedef void weight;

      /// Window associated type.
      typedef void window;

      /*! \brief Test if the weighted window is empty.
       *
       * A weighted_window of null size is empty.
       */
      bool is_empty() const;

      /*! \brief Test if the weighted_window is centered.
       *
       * A weighted window is centered is the origin belongs to it.
       */
      bool is_centered() const;

      /*! \brief Give the maximum coordinate gap between the window
	center and a window point.
       */
      unsigned delta() const;

      /*! \brief Give the corresponding window.
       */
      const window& win() const;

      /*! \brief Apply a central symmetry to the target weighted window.
       */
      E& sym();
    };

  } // end of namespace mln::doc

} // end of namespace mln
