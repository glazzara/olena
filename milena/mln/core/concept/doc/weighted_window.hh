// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
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

      /*! \brief Site_Iterator type associated to this weighted_window to browse its
       * points in a forward way.
       */
      typedef void fwd_qiter;

      /*! \brief Site_Iterator type associated to this weighted_window to browse its
       * points in a backward way.
       */
      typedef void bkd_qiter;

      /// Site associated type.
      typedef void point;

      /// Dpsite associated type.
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
