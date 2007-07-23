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

#ifndef MLN_CORE_CONCEPT_GENPOINT_HH
# define MLN_CORE_CONCEPT_GENPOINT_HH

/*! \file mln/core/concept/generalized_point.hh
 * \brief Definition of the concept of mln::Generalized_Point.
 */
# include <mlc/equal.hh>
# include <mlc/same_point.hh>
# include <mlc/same_coord.hh>

# include <mln/core/concept/object.hh>
# include <mln/core/internal/force_exact.hh>


namespace mln
{

  // FIXME: \class Generalized_Point generalized_point.hh "mln/core/concept/doc/generalized_point.hh"

  /*! \brief Base class for implementation classes that are points or that
   *  have the behavior of points.
   *
   * "Generalized_Point" is "Generalized Point" for short.
   *
   * \warning This class does \em not derive from mln::Object; it is
   * for use as a parallel hierarchy.
   * 
   * \see mln::doc::Generalized_Point for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Generalized_Point
  {

    /*
      enum { dim };

      typedef point;
      typedef dpoint;
      typedef coord;
      typedef topo; // FIXME

      either Point
      or operator point() const;

      const point* pointer_() const;
      coord operator[](unsigned i) const;
    */

  protected:
    Generalized_Point();

  };



  /*! \brief Equality comparison between a couple of generalized point \p lhs
   *  and \p rhs.
   *
   * \param[in] lhs A first generalized point.
   * \param[in] rhs A second generalized point.
   *
   * \pre Both \p lhs and \p rhs have to be defined on the same
   * topology; otherwise this test does not compile.
   *
   * \return True if both generalized points have the same
   * coordinates, otherwise false.
   *
   * \relates mln::Generalized_Point
   */
  template <typename Pl, typename Pr>
  bool operator==(const Generalized_Point<Pl>& lhs, const Generalized_Point<Pr>& rhs);


  /*! \brief Ordering "less than" comparison between a couple of generalized
   *  points \p lhs and \p rhs.
   *
   * \param[in] lhs A first generalized point.
   * \param[in] rhs A second generalized point.
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
   * \relates mln::Generalized_Point
   */
  template <typename Pl, typename Pr>
  bool operator<(const Generalized_Point<Pl>& lhs, const Generalized_Point<Pr>& rhs);


  /*! \brief Difference between a couple of generalized point \p lhs and \p
   *  rhs.
   *
   * \param[in] lhs A first generalized point.
   * \param[in] rhs A second generalized point.
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
   * \return A delta-point (temporary object).
   *
   * \see mln::Dpoint
   * \relates mln::Generalized_Point
   */
  template <typename Pl, typename Pr>
  mln_dpoint(Pl)
  operator-(const Generalized_Point<Pl>& lhs, const Generalized_Point<Pr>& rhs);


  /*! \brief Add a delta-point \p rhs to a generalized point \p lhs.
   *
   * \param[in] lhs A generalized point.
   * \param[in] rhs A delta-point.
   *
   * The type of \p rhs has to be exactly the delta-point type
   * associated with the type of \p lhs.
   *
   * \return A point (temporary object).
   *
   * \see mln::Dpoint
   * \relates mln::Generalized_Point
   *
   * \todo Introduce the notion of "generalized dpoint" and
   * add the more general extra operator+(Generalized_Point, GenDpoint).
   */
  template <typename P>
  mln_point(P)
  operator+(const Generalized_Point<P>& lhs, const mln_dpoint(P)& rhs);


  /*! \brief Substract a delta-point \p rhs to a generalized point \p lhs.
   *
   * \param[in] lhs A generalized point.
   * \param[in] rhs A delta-point.
   *
   * The type of \p rhs has to be exactly the delta-point type
   * associated with the type of \p lhs.
   *
   * \return A point (temporary object).
   *
   * \see mln::Dpoint
   * \relates mln::Generalized_Point
   */
  template <typename P>
  mln_point(P)
  operator-(const Generalized_Point<P>& lhs, const mln_dpoint(P)& rhs);


  /*! \brief Print a generalized point \p p into the output stream \p
   *  ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] p A generalized point.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Generalized_Point
   */
  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Generalized_Point<P>& p);



# ifndef MLN_INCLUDE_ONLY


  template <typename E>
  Generalized_Point<E>::Generalized_Point()
  {
    int dim = E::dim;
    mln_invariant(dim > 0);
    dim = 0;
    typedef  mln_point(E)  point;
    typedef mln_dpoint(E) dpoint;
    typedef mln_coord(E)  coord;
    const point* (E::*m1)() const = & E::pointer_;
    m1 = 0;
    coord (E::*m2)(unsigned i) const = & E::operator[];
    m2 = 0;
  }


  template <typename Pl, typename Pr>
  bool operator==(const Generalized_Point<Pl>& lhs, const Generalized_Point<Pr>& rhs)
  {
    // FIXME: mlc::same_grid<Pl, Pr>::check();
    const Pl& lhs_ = internal::force_exact<Pl>(lhs);
    const Pr& rhs_ = internal::force_exact<Pr>(rhs);
    mlc::same_point<Pl, Pr>::check();
    for (unsigned i = 0; i < Pl::dim; ++i)
      if (lhs_[i] != rhs_[i])
	return false;
    return true;
  }

  template <typename Pl, typename Pr>
  bool operator<(const Generalized_Point<Pl>& lhs, const Generalized_Point<Pr>& rhs)
  {
    // FIXME: mlc::same_grid<Pl, Pr>::check();
    const Pl& lhs_ = internal::force_exact<Pl>(lhs);
    const Pr& rhs_ = internal::force_exact<Pr>(rhs);
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
  operator-(const Generalized_Point<Pl>& lhs, const Generalized_Point<Pr>& rhs)
  {
    mlc::equal<mln_dpoint(Pl), mln_dpoint(Pr)>::check();
    // FIXME: mlc::same_grid<Pl, Pr>::check();
    mlc::same_coord<Pl, Pr>::check();
    const Pl& lhs_ = internal::force_exact<Pl>(lhs);
    const Pr& rhs_ = internal::force_exact<Pr>(rhs);
    mln_dpoint(Pl) tmp;
    for (unsigned i = 0; i < Pl::dim; ++i)
      tmp[i] = lhs_[i] - rhs_[i];
    assert(rhs + tmp == lhs);
    return tmp;
  }

  template <typename P>
  mln_point(P)
  operator+(const Generalized_Point<P>& lhs, const mln_dpoint(P)& rhs)
  {
    const P& lhs_ = internal::force_exact<P>(lhs);
    mln_point(P) tmp;
    for (unsigned i = 0; i < P::dim; ++i)
      tmp[i] = lhs_[i] + rhs[i];
    return tmp;
  }

  template <typename P>
  mln_point(P)
  operator-(const Generalized_Point<P>& lhs, const mln_dpoint(P)& rhs)
  {
    const P& lhs_ = internal::force_exact<P>(lhs);
    mln_point(P) tmp;
    for (unsigned i = 0; i < P::dim; ++i)
      tmp[i] = lhs_[i] - rhs[i];
    return tmp;
  }

  template <typename P>
  std::ostream& operator<<(std::ostream& ostr, const Generalized_Point<P>& p)
  {
    const P& p_ = internal::force_exact<P>(p);

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
