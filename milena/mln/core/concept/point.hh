// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_POINT_HH
# define MLN_CORE_CONCEPT_POINT_HH

/*! \file
 *
 * \brief Definition of the concept of mln::Point.
 */

# include <mln/core/concept/point_site.hh>
# include <mln/core/concept/dpoint.hh>
# include <mln/value/concept/scalar.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Point;


  /// \cond INTERNAL_API
  /// Point category flag type.
  template <>
  struct Point<void>
  {
    typedef Point_Site<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for implementation of point classes.

    A point is an element of a space.

    For instance, mln::point2d is the type of elements defined on the
    discrete square grid of the 2D plane.

    \ingroup modconcepts
   */
  template <typename P>
  struct Point : public Point_Site<P>
  {
    /*! \brief The associated point type is itself.
     */
    typedef P point;

    /*! \brief It is a Point so it returns itself.
     */
    const P& to_point() const;

  protected:
    Point();
  };


  /*! \brief Shift a point \p by a delta-point \p dp.
   *
   * \param[in,out] p The targeted point.
   * \param[in] dp A delta-point.
   * \return A reference to the point \p p once translated by \p dp.
   *
   * \pre The type of \p dp has to be compatible with the type of \p
   * p.
   *
   * \relates mln::Point
   */
  template <typename P, typename D>
  P& operator+=(Point<P>& p, const Dpoint<D>& dp);


  /*! \brief Shift a point \p by the negate of a delta-point \p dp.
   *
   * \param[in,out] p The targeted point.
   * \param[in] dp A delta-point.
   * \return A reference to the point \p p once translated by - \p dp.
   *
   * \pre The type of \p dp has to be compatible with the type of \p
   * p.
   *
   * \relates mln::Point
   */
  template <typename P, typename D>
  P& operator-=(Point<P>& p, const Dpoint<D>& dp);

  /*! \brief Divise a point \p by a scalar \p s.
   *
   * \param[in,out] p The targeted point.
   * \param[in] dp A scalar.
   * \return A reference to the point \p p once divised by \p s.
   *
   * \relates mln::Point
   */
  template <typename P, typename D>
  P& operator/(Point<P>& p, const value::Scalar<D>& dp);

  // FIXME : add operators and traits?

# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  Point<P>::Point()
  {
  }

  template <typename P>
  inline
  const P&
  Point<P>::to_point() const
  {
    return exact(*this);
  }

  template <typename P, typename D>
  inline
  P& operator+=(Point<P>& p_, const Dpoint<D>& dp_)
  {
    P& p = exact(p_);
    const D& dp = exact(dp_);
    // FIXME: Use to_vec()!
    for (unsigned i = 0; i < P::dim; ++i)
      p[i] += dp[i];
    return p;
  }

  template <typename P, typename D>
  inline
  P& operator-=(Point<P>& p_, const Dpoint<D>& dp_)
  {
    P& p = exact(p_);
    const D& dp = exact(dp_);
    // FIXME: Use to_vec()!
    for (unsigned i = 0; i < P::dim; ++i)
      p[i] -= dp[i];
    return p;
  }


  template <typename P, typename D>
  inline
  P&
  operator/(Point<P>& p_, const value::Scalar<D>& s_)
  {
    P& p = exact(p_);
    const D& s = exact(s_);
    for (unsigned i = 0; i < P::dim; ++i)
      p[i] /= s;
    return p;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



#endif // ! MLN_CORE_CONCEPT_POINT_HH
