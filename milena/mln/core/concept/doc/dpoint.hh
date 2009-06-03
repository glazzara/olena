// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

/*! \file mln/core/concept/doc/dpoint.hh
 * \brief This file documents the concept of mln::Dpoint.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Dpoint.
     * \see mln::Dpoint
     */
    template <typename E>
    struct Dpoint : public Object<E>
    {
      /*! \var dim
       * \brief Dimension of the space.
       * \invariant dim > 0 
       */
      enum { dim };

      /*! \brief Site associated type.
       * \invariant This type has to derive from mln::Point.
       */
      typedef void point;

      /*! \brief Dpsite associated type.
       * \invariant This type has to derive from mln::Dpoint.
       */
      typedef void dpoint;

      /*! Coordinate associated type.
       */
      typedef void coord;

      /*! \brief Read-only access to the \p i-th coordinate value.
       *
       * \param[in] i The coordinate index.
       *
       * \pre \p i < \c dim
       *
       * \return The value of the \p i-th coordinate.
       */
      coord operator[](unsigned i) const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
