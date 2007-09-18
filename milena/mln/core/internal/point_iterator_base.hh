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

#ifndef MLN_CORE_INTERNAL_POINT_ITERATOR_BASE_HH
# define MLN_CORE_INTERNAL_POINT_ITERATOR_BASE_HH

/*! \file mln/core/internal/point_iterator_base.hh
 *
 * \brief Base class to factor code for point iterator classes.
 */

# include <mln/core/concept/point_iterator.hh>
# include <mln/core/concept/point_site.hh>


namespace mln
{

  namespace internal
  {


    /*! \brief A base class for point iterators.
     *
     * Parameter \c P is a point site type.
     *
     * \internal
     */
    template <typename P, typename E>
    struct point_iterator_base_ : public Point_Iterator<E>
    {
      /// Psite associated type.
      typedef P psite;

      /// Mesh associated type.
      typedef mln_mesh(P)  mesh;

      /// Dim value.
      enum { dim = mesh::dim };

      /// Point associated type.
      typedef mln_point(P) point;

      /// Dpoint associated type.
      typedef mln_dpoint(P) dpoint;

      /// Coord associated type.
      typedef mln_coord(point) coord;

    protected:
      /// Constructor.
      point_iterator_base_();
    };


#ifndef MLN_INCLUDE_ONLY

    template <typename P, typename E>
    point_iterator_base_<P, E>::point_iterator_base_()
    {
      mln::metal::is_a<P, Point_Site>::check();
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_POINT_ITERATOR_BASE_HH
