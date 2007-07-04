#ifndef MLN_CORE_CONCEPT_POINT_HH
# define MLN_CORE_CONCEPT_POINT_HH

/*! \file mln/core/concept/point.hh
 * \brief This file defines the concept of mln::Point.
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
