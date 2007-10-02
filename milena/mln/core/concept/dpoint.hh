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

#ifndef MLN_CORE_CONCEPT_DPOINT_HH
# define MLN_CORE_CONCEPT_DPOINT_HH

/*! \file mln/core/concept/dpoint.hh
 * \brief Definition of the concept of mln::Dpoint.
 */

# include <mln/core/concept/object.hh>
# include <mln/core/grids.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Dpoint;

  // Dpoint category flag type.
  template <>
  struct Dpoint<void>
  {
    typedef Object<void> super;
  };


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
    typedef Dpoint<void> category;

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


  /*! \brief Add the couple of delta-points \p lhs and \p rhs.
   *
   * \param[in] lhs A delta-point.
   * \param[in] rhs Another delta-point.
   *
   * \return A delta-point (temporary object).
   *
   * \relates mln::Dpoint
   */
  template <typename D>
  D operator+(const Dpoint<D>& lhs, const Dpoint<D>& rhs);


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

  template <typename D>
  D operator+(const Dpoint<D>& lhs, const Dpoint<D>& rhs)
  {
    D tmp; 
    for (unsigned i = 0; i < D::dim; ++i)
      tmp[i] = exact(lhs)[i] + exact(rhs)[i];
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
