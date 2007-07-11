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

#ifndef MLN_CORE_CONCEPT_BOX_HH
# define MLN_CORE_CONCEPT_BOX_HH

/*! \file mln/core/concept/box.hh
 * \brief Definition of the concept of mln::Box.
 */

# include <mln/core/concept/point_set.hh>


namespace mln
{

  /*! \brief Base class for implementation classes of boxes.
   *
   * Boxes are particular point sets useful to bound any set of
   * points.
   *
   * \see mln::doc::Box for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Box : public Point_Set<E>
  {
    /*
      const point& pmin() const;
      const point& pmax() const;
      unsigned len(unsigned i) const; // FIXME: Doc!
    */

    /*! \brief Return the bounding box of this point set.
     *
     * Return the bounding box of this point set, so that is itself.
     * This method is declared by the mln::Point_Set concept.
     *
     * \warning This method is final for all box classes.
     */
    const E& bbox() const;

    /*! \brief Return the number of points of this box.
     *
     * Return the number of points of this box.  This method is
     * declared by the mln::Point_Set concept.
     *
     * \warning This method is final for all box classes.
     */
    std::size_t npoints() const;

  protected:
    Box();
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  const E& Box<E>::bbox() const
  {
    return exact(*this);
  }

  template <typename E>
  Box<E>::Box()
  {
    typedef mln_point(E) point;
    point (E::*m1)() const = & E::pmin;
    m1 = 0;
    point (E::*m2)() const = & E::pmax;
    m2 = 0;
  }

  template <typename E>
  std::size_t
  Box<E>::npoints() const
  {
    std::size_t count = 1;
    typedef typename E::point P; // helps g++-3.3.5
    for (unsigned i = 0; i < P::dim; ++i)
      count *= exact(this)->len(i);
    return count;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_BOX_HH
