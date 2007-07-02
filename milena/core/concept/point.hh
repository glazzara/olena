#ifndef MLN_CORE_CONCEPT_POINT_HH
# define MLN_CORE_CONCEPT_POINT_HH

/*! \file mln/core/exact.hh
 * This file defines the concept of mln::Point.
 */

# include <core/concept/psite.hh>


namespace mln
{

  /*! Base class for implementation of point.
   *
   * A point is 
   */
  template <typename P>
  struct Point : public Psite<P>
  {
    // final
    typedef P point;
    const P* pointer() const;

  protected:
    Point();
  };


  template <typename P>
  P& operator+=(Point<P>& lhs, const mln_dpoint(P)& rhs);

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
