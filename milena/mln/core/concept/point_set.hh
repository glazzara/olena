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

#ifndef MLN_CORE_CONCEPT_POINT_SET_HH
# define MLN_CORE_CONCEPT_POINT_SET_HH

/*! \file mln/core/concept/point_set.hh
 * \brief Definition of the concept of mln::Point_Set.
 */

# include <mln/core/concept/point.hh>
# include <mln/core/concept/piter.hh>


namespace mln
{

  /*! \brief Base class for implementation classes of point sets.
   *
   * \see mln::doc::Point_Set for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Point_Set : public Object<E>
  {
    /*
      typedef point;
      typedef psite;
      typedef piter;
      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;
      const box_<point>& bbox() const;
      std::size_t npoints() const;
     */

  protected:
    Point_Set();
  };


  /*! \brief Equality test between point sets \p lhs and \p rhs.
   *
   * \param[in] lhs A point set.
   * \param[in] rhs Another point set.
   *
   * \relates mln::Point_Set
   */
  template <typename Sl, typename Sr>
  bool operator==(const Point_Set<Sl>& lhs, const Point_Set<Sr>& rhs);



  /*! \brief Inclusion test between point sets \p lhs and \p rhs.
   *
   * \param[in] lhs A point set (included?).
   * \param[in] rhs Another point set (includer?).
   *
   * \relates mln::Point_Set
   */
  template <typename Sl, typename Sr>
  bool operator<=(const Point_Set<Sl>& lhs, const Point_Set<Sr>& rhs);



  /*! \brief Strict inclusion test between point sets \p lhs and \p
   *  rhs.
   *
   * \param[in] lhs A point set (strictly included?).
   * \param[in] rhs Another point set (includer?).
   *
   * \relates mln::Point_Set
   */
  template <typename Sl, typename Sr>
  bool operator<(const Point_Set<Sl>& lhs, const Point_Set<Sr>& rhs);



  /*! \brief Print a point set \p pset into the output stream \p
   *  ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] pset A point set.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Point_Set
   */
  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const Point_Set<S>& pset);



# ifndef MLN_INCLUDE_ONLY

  // fwd decl
  template <typename P> struct box_;

  template <typename E>
  Point_Set<E>::Point_Set()
  {
    typedef mln_point(E) point;
    typedef mln_psite(E) psite;

    typedef mln_piter(E)         piter;
    typedef mln_fwd_piter(E) fwd_piter;
    typedef mln_bkd_piter(E) bkd_piter;
    
    bool (E::*m1)(const psite& p) const = & E::has;
    m1 = 0;
    const box_<point>& (E::*m2)() const = & E::bbox;
    m2 = 0;
    std::size_t (E::*m3)() const = & E::npoints;
    m3 = 0;
  }


  // operators


  template <typename Sl, typename Sr>
  bool operator==(const Point_Set<Sl>& lhs_, const Point_Set<Sr>& rhs_)
  {
    // FIXME: Same grid!
    const Sl& lhs = exact(lhs_);
    const Sr& rhs = exact(rhs_);

    // easy test:
    if (lhs.npoints() != rhs.npoints())
      return false;

    // exhaustive test:
    mln_fwd_piter(Sl) pl(lhs);
    mln_fwd_piter(Sr) pr(rhs);
    for (pl.start(),      pr.start();
	 pl.is_valid() && pr.is_valid();
	 pl.next(),       pr.next())
      if (pl != pr)
	return false; // difference found

    // both sets are equal only if both browsings are completed
    // at the same time:
    return ! pl.is_valid() && ! pr.is_valid();
  }


  template <typename Sl, typename Sr>
  bool operator<=(const Point_Set<Sl>& lhs_, const Point_Set<Sr>& rhs_)
  {
    // FIXME: Same grid!
    const Sl& lhs = exact(lhs_);
    const Sr& rhs = exact(rhs_);

    // easy test:
    if (lhs.npoints() > rhs.npoints())
      return false;

    // exhaustive test:
    mln_piter(Sl) pl(lhs);
    for_all(pl)
      if (! rhs.has(pl))
	return false;

    return true;
  }


  template <typename Sl, typename Sr>
  bool operator<(const Point_Set<Sl>& lhs_, const Point_Set<Sr>& rhs_)
  {
    // FIXME: Same grid!
    const Sl& lhs = exact(lhs_);
    const Sr& rhs = exact(rhs_);
    return lhs <= rhs && lhs.npoints() != rhs.npoints();
  }


  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const Point_Set<S>& pset_)
  {
    const S& pset = exact(pset_);
    ostr << '{';
    mln_piter(S) p(pset);
    for_all(p)
      ostr << p;
    return ostr << '}';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_CONCEPT_POINT_SET_HH
