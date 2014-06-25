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

#ifndef MLN_CORE_CONCEPT_POINT_SITE_HH
# define MLN_CORE_CONCEPT_POINT_SITE_HH

/*! \file
 *
 * \brief Definition of the concept of mln::Point_Site.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/concept/delta_point_site.hh>
# include <mln/core/grids.hh>
# include <mln/trait/all.hh>



namespace mln
{

  // Fwd decls.
  template <typename E> struct Point_Site;


  /// \cond INTERNAL_API
  namespace trait
  {

    template < typename P, typename D >
    struct set_binary_< op::plus,
			mln::Point_Site, P, mln::Delta_Point_Site, D >
    {
      typedef mln_point(P) ret;
    };

    template < typename P, typename D >
    struct set_binary_< op::minus,
			mln::Point_Site, P, mln::Delta_Point_Site, D >
    {
      typedef mln_point(P) ret;
    };

    template < typename L, typename R >
    struct set_binary_< op::minus,
			mln::Point_Site, L, mln::Point_Site, R >
    {
      typedef mln_dpoint(L) ret;
    };

  } // end of namespace mln::trait
  /// \endcond


  /// \cond INTERNAL_API
  /// Point site category flag type.
  template <>
  struct Point_Site<void>
  {
    typedef Object<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for implementation classes of the notion of
    "point site".

    A point site ("psite" for short) is an object that allows an
    efficient access to data associated with a point.  A point site
    is either a point or designates a point.

    When a point site is not really a point, it is automatically
    convertible to the point it designates.


    Let us take the example of a 2D image encoded as an array of runs
    of values.  With a point, a pair (row index, column index),
    retrieving the corresponding pixel value would mean to browse the
    array of runs to find the value location.  That would not be
    efficient.  Conversely, a point site dedicated to this image
    structure allows for value access in contant time; precisely the
    proper point site is a pair (index of run, index within the run).

    \ingroup modconcepts
  */
  template <typename E>
  struct Point_Site : public Object<E>
  {
    typedef Point_Site<void> category;

    /*
    enum { dim };
    typedef mesh;

    typedef point;
    typedef dpoint;
    typedef coord;

    const point& to_point() const;

    coord operator[](unsigned i) const;
    */

  protected:
    Point_Site();
  };


  /*! \brief Equality comparison between a couple of point site \p lhs
   *  and \p rhs.
   *
   * \param[in] lhs A first point site.
   * \param[in] rhs A second point site.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology; otherwise this test does not compile.
   *
   * \return True if both point sites have the same
   * coordinates, otherwise false.
   *
   * \relates mln::Point_Site
   */
  template <typename L, typename R>
  bool operator==(const Point_Site<L>& lhs, const Point_Site<R>& rhs);


  /*! \brief Difference between a couple of point site \p lhs and \p
   *  rhs.
   *
   * \param[in] lhs A first point site.
   * \param[in] rhs A second point site.
   *
   * \warning There is no type promotion in Milena so the client
   * has to make sure that both points are defined with the same
   * type of coordinates.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology and with the same type of coordinates; otherwise this
   * test does not compile.
   *
   * \post The result, \p dp, is such as \p lhs == \p rhs + \p dp.
   *
   * \return A delta point (temporary object).
   *
   * \relates mln::Point_Site
   * \see mln::Delta_Point_Site
   */
  template <typename L, typename R>
  mln_dpoint(L)
  operator-(const Point_Site<L>& lhs, const Point_Site<R>& rhs);


  /// \brief Add a delta-point \p rhs to a point site \p lhs.
  ///
  /// \param[in] p  A point site.
  /// \param[in] dp A delta-point.
  ///
  /// The type of \p dp has to compatible with the type of \p p.
  ///
  /// \return A point (temporary object).
  ///
  /// \relates mln::Point_Site
  /// \see mln::Delta_Point_Site
  ///
  /// \{
  template <typename P, typename D>
  mln_point(P)
  operator+(const Point_Site<P>& p, const Delta_Point_Site<D>& dp);
  /// }


  /// \brief Substract a delta-point \p dp to a point site \p p.
  ///
  /// \param[in] p A point site.
  /// \param[in] dp A delta-point.
  ///
  /// The type of \p dp has to compatible with the type of \p p.
  ///
  /// \return A point (temporary object).
  ///
  /// \see mln::Dpoint
  /// \see mln::Delta_Point_Site
  ///
  /// \{
  template <typename P, typename D>
  mln_point(P)
  operator-(const Point_Site<P>& p, const Delta_Point_Site<D>& dp);
  /// \}


  /*! \brief Print a point site \p p into the output stream \p ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] p A point site.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Point_Site
   */
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Point_Site<P>& p);




# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  inline
  Point_Site<E>::Point_Site()
  {
    int dim = E::dim;
    mln_invariant(dim > 0);
    dim = 0;
    typedef mln_mesh(E)   mesh;
    typedef mln_point(E)  point;
    typedef mln_dpoint(E) dpoint;
    typedef mln_coord(E)  coord;
    const point& (E::*m1)() const = & E::to_point;
    m1 = 0;
    coord (E::*m2)(unsigned i) const = & E::operator[];
    m2 = 0;
  }


  // Operators.

  // FIXME: Remove, or factor in a lower class (Théo removed it from
  // the cleanup-2008 branch).
  template <typename L, typename R>
  inline
  bool operator==(const Point_Site<L>& lhs_, const Point_Site<R>& rhs_)
  {
    mln::metal::bool_<(int(L::dim) == int(R::dim))>::check();
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    for (unsigned i = 0; i < L::dim; ++i)
      if (lhs[i] != rhs[i])
	return false;
    return true;
  }

  // FIXME: Remove, or factor in a lower class (Théo removed it from
  // the cleanup-2008 branch).
  template <typename L, typename R>
  inline
  mln_dpoint(L) // FIXME: promote!
  operator-(const Point_Site<L>& lhs_, const Point_Site<R>& rhs_)
  {
    mln::metal::bool_<(int(L::dim) == int(R::dim))>::check();
    const L& lhs = exact(lhs_);
    const R& rhs = exact(rhs_);
    mln_dpoint(L) tmp;
    for (unsigned i = 0; i < L::dim; ++i)
      tmp[i] = lhs[i] - rhs[i];
    mln_postcondition(rhs_ + tmp == lhs_);
    return tmp;
  }

  template <typename P, typename D>
  inline
  mln_point(P) // FIXME: promote!
  operator+(const Point_Site<P>& p_, const Delta_Point_Site<D>& dp_)
  {
    mln::metal::bool_<(int(P::dim) == int(D::dim))>::check();
    const P& p  = exact(p_);
    const D& dp = exact(dp_);
    mln_point(P) tmp;
    for (unsigned i = 0; i < P::dim; ++i)
      tmp[i] = p[i] + dp[i];
    return tmp;
  }

  template <typename P, typename D>
  inline
  mln_point(P) // FIXME: promote!
  operator-(const Point_Site<P>& p_, const Delta_Point_Site<D>& dp_)
  {
    mln::metal::bool_<(int(P::dim) == int(D::dim))>::check();
    const P& p  = exact(p_);
    const D& dp = exact(dp_);
    mln_point(P) tmp;
    for (unsigned i = 0; i < P::dim; ++i)
      tmp[i] = p[i] - dp[i];
    return tmp;
  }

  // FIXME: We shall not rely on a point object associted to the point
  // site!  (Such an object does not always exist.)
  template <typename P>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Point_Site<P>& p_)
  {
    const P& p = exact(p_);
    ostr << '(';
    for (unsigned i = 0; i < P::dim; ++i)
	ostr << p[i] << (i == P::dim - 1 ? ')' : ',');
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_POINT_SITE_HH
