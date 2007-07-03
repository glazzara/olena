#ifndef MLN_CORE_CONCEPT_GENPOINT_HH
# define MLN_CORE_CONCEPT_GENPOINT_HH

/*! \file mln/core/concept/genpoint.hh
 * \brief This file defines the concept of mln::GenPoint.
 */
# include <mlc/equal.hh>
# include <mlc/same_point.hh>
# include <mlc/same_coord.hh>

# include <mln/core/concept/object.hh>
# include <mln/core/macros.hh>
# include <mln/core/contract.hh>


namespace mln
{

  // FIXME: \class GenPoint genpoint.hh "mln/core/concept/doc/genpoint.hh"

  /*! \brief Base class for implementation classes that are points or that
   *  have the behavior of points.
   *
   * "GenPoint" is "Generalized Point" for short.
   *
   * \warning This class does \em not derive from mln::Object; it is
   * for use as a parallel hierarchy.
   *
   * \see mln::doc::GenPoint for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct GenPoint // stand-alone class!
  {

    /*
      enum { dim };

      typedef point;
      typedef dpoint;
      typedef coord;
      typedef topo; // FIXME

      either Point
      or operator point() const;

      const point* pointer() const;
      coord operator[](unsigned i) const;
    */

  protected:
    GenPoint();
  };



  /*! \brief Equality comparison between a couple of generalized point \p lhs
   *  and \p rhs.
   *
   * @param[in] lhs A first generalized point.
   * @param[in] rhs A second generalized point.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology; otherwise this test does not compile.
   *
   * \return True if both generalized points have the same
   * coordinates, otherwise false.
   *
   * \relates mln::GenPoint
   */
  template <typename Pl, typename Pr>
  bool operator==(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs);


  /*! \brief Ordering "less than" comparison between a couple of generalized
   *  point \p lhs and \p rhs.
   *
   * @param[in] lhs A first generalized point.
   * @param[in] rhs A second generalized point.
   *
   * This test is based on a lexicographical ordering over coordinates.
   *
   * \warning In the general case this ordering relationship is \em
   * not bound to the way of browsing a domain with a forward point
   * iterator.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology; otherwise this test does not compile.
   *
   * \return True if \p lhs is before \p rhs in the sense of the
   * coordinates lexicographic comparison, otherwise false.
   *
   * \relates mln::GenPoint
   */
  template <typename Pl, typename Pr>
  bool operator<(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs);


  /*! \brief Difference between a couple of generalized point \p lhs and \p
   *  rhs.
   *
   * @param[in] lhs A first generalized point.
   * @param[in] rhs A second generalized point.
   *
   * \warning There is no type promotion in milena so the client
   * has to make sure that both points are defined with the same
   * type of coordinates.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology and with the same type of coordinates; otherwise this
   * test does not compile.
   *
   * \post The result, \p dp, is such as \p lhs == \p rhs + \p dp. 
   *
   * \return A delta-point.
   *
   * \see mln::Dpoint
   * \relates mln::GenPoint
   */
  template <typename Pl, typename Pr>
  mln_dpoint(Pl)
  operator-(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs);


  /*! \brief Add a delta-point \p rhs to a generalized point \p lhs.
   *
   * @param[in] lhs A generalized point.
   * @param[in] rhs A delta-point.
   *
   * The type of \p rhs has to be exactly the delta-point type
   * associated with the type of \p lhs.
   *
   * \return A new point (temporary object).
   *
   * \see mln::Dpoint
   * \relates mln::GenPoint
   *
   * \todo Introduce the notion of "generalized dpoint" and
   * add the more general extra operator+(GenPoint, GenDpoint).
   */
  template <typename P>
  mln_point(P)
  operator+(const GenPoint<P>& lhs, const mln_dpoint(P)& rhs);


  /*! \brief Print a generalized point \p p into the output stream \p
   *  ostr.
   *
   * @param[in,out] ostr An output stream.
   * @param[in] p A generalized point.
   *
   * The type of \p rhs has to be exactly the delta-point type
   * associated with the type of \p lhs.
   *
   * \return The modified output stream.
   *
   * \see mln::Dpoint
   * \relates mln::GenPoint
   *
   * \todo Introduce the notion of "generalized dpoint" and
   * add the more general extra operator+(GenPoint, GenDpoint).
   */
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const GenPoint<P>& p);


# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  GenPoint<E>::GenPoint()
  {
    const int dim = E::dim;
    invariant(dim > 0);
    typedef  mln_point(E)  point;
    typedef mln_dpoint(E) dpoint;
    typedef mln_coord(E)  coord;
    const point* (E::*m1)() const = & E::pointer;
    m1 = 0;
    coord (E::*m2)(unsigned i) const = & E::operator[];
    m2 = 0;
  }

  template <typename Pl, typename Pr>
  bool operator==(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs)
  {
    // FIXME: mlc::same_grid<Pl, Pr>::check();
    const Pl& lhs_ = force_exact<Pl>(lhs);
    const Pr& rhs_ = force_exact<Pr>(rhs);
    mlc::same_point<Pl, Pr>::check();
    for (unsigned i = 0; i < Pl::dim; ++i)
      if (lhs_[i] != rhs_[i])
	return false;
    return true;
  }

  template <typename Pl, typename Pr>
  bool operator<(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs)
  {
    // FIXME: mlc::same_grid<Pl, Pr>::check();
    const Pl& lhs_ = force_exact<Pl>(lhs);
    const Pr& rhs_ = force_exact<Pr>(rhs);
    for (unsigned i = 0; i < Pl::dim; ++i)
      {
	if (lhs_[i] == rhs_[i])
	  continue;
	return lhs_[i] < rhs_[i];
      }
    return false;
  }


  template <typename Pl, typename Pr>
  mln_dpoint(Pl)
  operator-(const GenPoint<Pl>& lhs, const GenPoint<Pr>& rhs)
  {
    mlc::equal<mln_dpoint(Pl), mln_dpoint(Pr)>::check();
    // FIXME: mlc::same_grid<Pl, Pr>::check();
    mlc::same_coord<Pl, Pr>::check();
    const Pl& lhs_ = force_exact<Pl>(lhs);
    const Pr& rhs_ = force_exact<Pr>(rhs);
    mln_dpoint(Pl) tmp;
    for (unsigned i = 0; i < Pl::dim; ++i)
      tmp[i] = lhs_[i] - rhs_[i];
    assert(rhs + tmp == lhs);
    return tmp;
  }


  template <typename P>
  mln_point(P)
  operator+(const GenPoint<P>& lhs, const mln_dpoint(P)& rhs)
  {
    const P& lhs_ = force_exact<P>(lhs);
    mln_point(P) tmp;
    for (unsigned i = 0; i < P::dim; ++i)
      tmp[i] = lhs_[i] + rhs[i];
    return tmp;
  }

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const GenPoint<P>& p)
  {
    const P& p_ = force_exact<P>(p);
    ostr << '(';
    for (unsigned i = 0; i < P::dim; ++i)
      {
	ostr << p_[i]
	     << (i == P::dim - 1 ? ')' : ',');
      }
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_GENPOINT_HH
