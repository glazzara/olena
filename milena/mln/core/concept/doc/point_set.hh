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

/*! \file mln/core/concept/doc/point_set.hh
 * \brief This file documents the concept of mln::Point_Set.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Point_Set.
     *
     * \see mln::Point_Set
     */
    template <typename E>
    struct Point_Set : public Object<E>
    {
      /*! \brief Point associated type.
       */
      typedef void point;

      /*! \brief Psite associated type.
       */
      typedef void psite;

      /*! \brief Forward Piter associated type.
       */
      typedef void fwd_piter;

      /*! \brief Backward Piter associated type.
       */
      typedef void bkd_piter;

      /*! \brief Test if \p p belongs to this point set. 
       *
       * \param[in] p A point site.
       * \return True if \p p is an element of the point set.
       */
      bool has(const psite& p) const;

      /*! \brief Give the bounding box of this point set.
       */
      const box_<point>& bbox() const;

      /*! \brief Give the number of points of this point set.
       */
      std::size_t npoints() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
