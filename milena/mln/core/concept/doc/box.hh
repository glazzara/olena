// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/*! \file mln/core/concept/doc/box.hh
 *
 * \brief This file documents the concept of mln::Box.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Box.
     * \see mln::Box
     */
    template <typename E>
    struct Box : public Site_Set<E>
    {
      /*! \brief Give the box "minimum" point.
       *
       * Return the "minimum" point w.r.t. the ordering between points.
       * For instance, with mln::box2d, this minimum is the top left
       * point of the box.
       */
      const site& pmin() const;

      /*! \brief Give the box "maximum" point.
       *
       * Return the "maximum" point w.r.t. the ordering between points.
       * For instance, with mln::box2d, this maximum is the bottom
       * right point of the box.
       */
      const site& pmax() const;

      /*! \brief Return the bounding box of this point set.
       *
       * Return the bounding box of this point set, so that is itself.
       * This method is declared by the mln::Site_Set concept.
       *
       * \warning This method is final for all box classes.
       */
      const E& bbox() const;

      /*! \brief Return the number of points of this box.
       *
       * Return the number of points of this box.  This method is
       * declared by the mln::Site_Set concept.
       *
       * \warning This method is final for all box classes.
       */
      unsigned nsites() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
