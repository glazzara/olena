#ifndef MLN_CORE_CONCEPT_DPOINT_HH
# define MLN_CORE_CONCEPT_DPOINT_HH

/*! \file mln/core/concept/dpoint.hh
 * \brief Definition of the concept of mln::Dpoint.
 */

# include <mln/core/concept/object.hh>


namespace mln
{


  /*! \brief Base class for implementation of delta-point classes.
   *
   * A delta-point is a vector defined by a couple of points.
   *
   * Given two points, A and B, the vector AB is mapped into the
   * delta-point D = AB.  Practically one can write: D = B - A.   
   *
   * \see mln::doc::Dpoint for a complete documentation of this class
   * contents.
   */
  template <typename E>
  struct Dpoint : public Object<E>
  {
    /*
      typedef point;
      typedef coord;
      enum { dim };
      coord operator[](unsigned i) const;
    */

  protected:
    Dpoint();
  };


  /*! \brief Give the negate of the delta-point \p rhs.
   *
   * \param[in] rhs The targeted delta-point.
   *
   * \invariant For all delta-point dp, we have: -(-dp) == dp.
   *
   * \return A delta-point (temporary object).
   *
   * \relates mln::Dpoint
   */
  template <typename D>
  D operator-(const Dpoint<D>& rhs);


  /*! \brief Equality comparison between a couple of delta-point \p lhs
   *  and \p rhs.
   *
   * \param[in] lhs A first delta-point.
   * \param[in] rhs A second delta-point.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology; otherwise this test does not compile.
   *
   * \return True if both delta-points have the same coordinates,
   * otherwise false.
   *
   * \relates mln::Dpoint
   */
  template <typename Dl, typename Dr>
  bool operator==(const Dpoint<Dl>& lhs, const Dpoint<Dr>& rhs);


  /*! \brief Ordering "less than" comparison between a couple of
   *  delta-points \p lhs and \p rhs.
   *
   * \param[in] lhs A first delta-point.
   * \param[in] rhs A second delta-point.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology; otherwise this test does not compile.
   *
   * \return True if both delta-points have the same coordinates,
   * otherwise false.
   *
   * \relates mln::Dpoint
   */
  template <typename Dl, typename Dr>
  bool operator<(const Dpoint<Dl>& lhs, const Dpoint<Dr>& rhs);


  /*! \brief Print a delta-point \p p into the output stream \p ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] dp A delta-point.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Dpoint
   */
  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const Dpoint<D>& dp);



# ifndef MLN_INCLUDE_ONLY

  template <typename E>
  Dpoint<E>::Dpoint()
  {
    typedef mln_point(E) point;
    typedef mln_coord(E) coord;
    enum { dim = E::dim };
    coord (E::*m)(unsigned i) const = & E::operator[];
    m = 0;
  }

  template <typename D>
  D operator-(const Dpoint<D>& rhs)
  {
    D tmp; 
    for (unsigned i = 0; i < D::dim; ++i)
      tmp[i] = - exact(rhs)[i];
    return tmp;
  }

  template <typename Dl, typename Dr>
  bool operator==(const Dpoint<Dl>& lhs, const Dpoint<Dr>& rhs)
  {
    for (unsigned i = 0; i < Dl::dim; ++i)
      if (exact(lhs)[i] != exact(rhs)[i])
	return false;
    return true;
  }

  template <typename Dl, typename Dr>
  bool operator<(const Dpoint<Dl>& lhs, const Dpoint<Dr>& rhs)
  {
    for (unsigned i = 0; i < Dl::dim; ++i)
      {
	if (exact(lhs)[i] == exact(rhs)[i])
	  continue;
	return exact(lhs)[i] < exact(rhs)[i];
      }
    return false;
  }

  template <typename D>
  std::ostream& operator<<(std::ostream& ostr, const Dpoint<D>& dp)
  {
    ostr << '(';
    for (unsigned i = 0; i < D::dim; ++i)
      {
	ostr << exact(dp)[i]
	     << (i == D::dim - 1 ? ')' : ',');
      }
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_DPOINT_HH
