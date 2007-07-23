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

/*! \file mln/core/concept/doc/window.hh
 * \brief This file documents the concept of mln::Window.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Window.
     *
     * A window is the definition of a set of points located around a
     * central point.
     *
     * \see mln::Window
     */
    template <typename E>
    struct Window : public Object<E>
    {
      /*! \brief Point_Iterator type associated to this window to browse its
       * points.
       */
      typedef void qiter;

      /*! \brief Point_Iterator type associated to this window to browse its
       * points in a forward way.
       */
      typedef void fwd_qiter;

      /*! \brief Point_Iterator type associated to this window to browse its
       * points in a backward way.
       */
      typedef void bkd_qiter;

      /*! \brief Test if the window is empty.
       *
       * A window of null size is empty.
       */
      bool is_empty() const;

      /*! \brief Test if the window is centered.
       *
       * A window is centered is the origin belongs to the window.
       */
      bool is_centered() const;

      /*! \brief Test if the window is symmetric.
       */
      bool is_symmetric() const;

      /*! \brief Give the maximum coordinate gap between the window
	center and a window point.
       */
      unsigned delta() const;

      /*! \brief Give the symmetrical window.
       */
      E sym_() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
