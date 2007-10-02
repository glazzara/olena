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

#ifndef MLN_CORE_CONCEPT_PSITE_HH
# define MLN_CORE_CONCEPT_PSITE_HH

/*! \file mln/core/concept/point_site.hh
 * \brief Definition of the concept of mln::Point_Site.
 */

# include <mln/core/concept/generalized_point.hh>


namespace mln
{

  // FIXME: Hack is "Point" instead of "Points_Site"; change it!


  // Fwd decl.
  template <typename E> struct Point;

  // Point_Site category flag type.
  template <>
  struct Point<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes of the notion of
   *  "point site".
   *
   * A point site ("psite" for short) is an object that allows an
   * efficient access to data associated with a point.  A point site
   * is either a point or designates a point: regular points, deriving
   * from mln::Point, are point sites, yet some point sites are not
   * points.)  A point site has the behavior expected from every
   * point; see mln::Generalized_Point.
   *
   * When a point site is not a point, it is automatically convertible
   * to the point it designates.
   *
   *
   * Let us take the example of a 2D image encoded as an array of runs
   * of values.  With a point, a pair (row index, column index),
   * retrieving the corresponding pixel value would mean to browse the
   * array of runs to find the value location.  That would not be
   * efficient.  Conversely, a point site dedicated to this image
   * structure allows for value access in contant time; precisely the
   * proper point site is a pair (index of run, index within the run).
   */
  template <typename E>
  struct Point_Site : public Object<E>,
		      public Generalized_Point<E>
  {
    typedef Point<void> category; // FIXME: This is a hack!  Change the hierarchy...

    /*
    const point* pointer_() const
    {
      return & (exact(this)->operator point());
    }
    */

  protected:
    Point_Site();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Point_Site<E>::Point_Site()
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_PSITE_HH
