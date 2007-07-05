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

#ifndef MLN_CORE_CONCEPT_POINT_HH
# define MLN_CORE_CONCEPT_POINT_HH

/*! \file mln/core/concept/point.hh
 *
 * \brief Definition of the concept of mln::Point.
 */

# include <mln/core/concept/psite.hh>


namespace mln
{

  /*! \brief Base class for implementation of point classes.
   *
   * A point is an element of a space.
   *
   * For instance, mln::point2d is the type of elements defined on the
   * discrete square grid of the 2D plane.
   */
  template <typename P>
  struct Point : public Psite<P>
  {
    /*! \brief The associated point type is itself.
     */
    typedef P point;

    /*! \brief The pointer is this point address.
     */
    const P* pointer() const;

  protected:
    Point();
  };


  /*! \brief Add a delta-point \p rhs to a point \p lhs.
   *
   * \param[in,out] lhs The targeted point.
   * \param[in] rhs A delta-point.
   *
   * \pre The type of \p rhs has to be the Dpoint type associated with
   * the type of \p lhs; otherwise this test does not compile.
   *
   * \return A reference to the point \p lhs once translated by \p
   * rhs.
   *
   * \relates mln::Point
   */
  template <typename P>
  P& operator+=(Point<P>& lhs, const mln_dpoint(P)& rhs);


  /*! \brief Remove a delta-point \p rhs to a point \p lhs.
   *
   * \param[in,out] lhs The targeted point.
   * \param[in] rhs A delta-point.
   *
   * \pre The type of \p rhs has to be the Dpoint type associated with
   * the type of \p lhs; otherwise this test does not compile.
   *
   * \return A reference to the point \p lhs once translated by - \p
   * rhs.
   *
   * \relates mln::Point
   */
  template <typename P>
  P& operator-=(Point<P>& lhs, const mln_dpoint(P)& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  Point<P>::Point()
  {
  }

  template <typename P>
  const P* Point<P>::pointer() const
  {
    return exact(this);
  }

  template <typename P>
  P& operator+=(Point<P>& lhs, const mln_dpoint(P)& rhs)
  {
    for (unsigned i = 0; i < P::dim; ++i)
      exact(lhs)[i] += rhs[i];
    return lhs;
  }

  template <typename P>
  P& operator-=(Point<P>& lhs, const mln_dpoint(P)& rhs)
  {
    for (unsigned i = 0; i < P::dim; ++i)
      exact(lhs)[i] -= rhs[i];
    return lhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_CONCEPT_POINT_HH
