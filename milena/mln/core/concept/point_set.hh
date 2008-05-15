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
 *
 * \brief Definition of the concept of mln::Site_Set.
 *
 * \todo Move out the ops.
 */

# include <mln/core/concept/point_site.hh>
# include <mln/core/concept/point_iterator.hh>
# include <mln/trait/site_sets.hh>
# include <mln/metal/not_equal.hh>


namespace mln
{

  // Fwd decl.
  template <typename E> struct Site_Set;


  /// Site_Set category flag type.
  template <>
  struct Site_Set<void>
  {
    typedef Object<void> super;
  };


  /*! \brief Base class for implementation classes of site sets.
   *
   * \see mln::doc::Site_Set for a complete documentation of this
   * class contents.
   */
  template <typename E>
  struct Site_Set : public Object<E>
  {
    typedef Site_Set<void> category;

    /*
      typedef site;
      typedef psite;

      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;
     */

  protected:
    Site_Set();
  };


  /*! \brief Equality test between site sets \p lhs and \p rhs.
   *
   * \param[in] lhs A site set.
   * \param[in] rhs Another site set.
   *
   * \relates mln::Site_Set
   */
  template <typename Sl, typename Sr>
  bool operator==(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /*! \brief Inclusion test between site sets \p lhs and \p rhs.
   *
   * \param[in] lhs A site set (included?).
   * \param[in] rhs Another site set (includer?).
   *
   * \relates mln::Site_Set
   */
  template <typename Sl, typename Sr>
  bool operator<=(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /*! \brief Strict inclusion test between site sets \p lhs and \p
   *  rhs.
   *
   * \param[in] lhs A site set (strictly included?).
   * \param[in] rhs Another site set (includer?).
   *
   * \relates mln::Site_Set
   */
  template <typename Sl, typename Sr>
  bool operator<(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /*! \brief Print a site set \p set into the output stream \p
   *  ostr.
   *
   * \param[in,out] ostr An output stream.
   * \param[in] set A site set.
   *
   * \return The modified output stream \p ostr.
   *
   * \relates mln::Site_Set
   */
  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const Site_Set<S>& set);



# ifndef MLN_INCLUDE_ONLY

  // fwd decl
  template <typename P> struct box_;

  template <typename E>
  inline
  Site_Set<E>::Site_Set()
  {
    // Check properties.
    mlc_not_equal( mln_trait_site_set_nsites(E),   mln::trait::undef )::check();
    mlc_not_equal( mln_trait_site_set_bbox(E),     mln::trait::undef )::check();
    mlc_not_equal( mln_trait_site_set_contents(E), mln::trait::undef )::check();
    mlc_not_equal( mln_trait_site_set_arity(E),    mln::trait::undef )::check();

    // Check associated types.
    typedef mln_site(E)  site;
    typedef mln_psite(E) psite;
    typedef mln_fwd_piter(E) fwd_piter;
    typedef mln_bkd_piter(E) bkd_piter;

    // Check methods.
    bool (E::*m)(const psite& p) const = & E::has;
    m = 0;
  }


  // operators


  template <typename Sl, typename Sr>
  inline
  bool operator==(const Site_Set<Sl>& lhs_, const Site_Set<Sr>& rhs_)
  {
    // FIXME: Same grid!
    const Sl& lhs = exact(lhs_);
    const Sr& rhs = exact(rhs_);

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
  inline
  bool operator<=(const Site_Set<Sl>& lhs_, const Site_Set<Sr>& rhs_)
  {
    // FIXME: Same grid!
    const Sl& lhs = exact(lhs_);
    const Sr& rhs = exact(rhs_);

    // exhaustive test:
    mln_piter(Sl) pl(lhs);
    for_all(pl)
      if (! rhs.has(pl))
	return false;

    return true;
  }


  template <typename Sl, typename Sr>
  inline
  bool operator<(const Site_Set<Sl>& lhs_, const Site_Set<Sr>& rhs_)
  {
    // FIXME: Same grid!
    const Sl& lhs = exact(lhs_);
    const Sr& rhs = exact(rhs_);
    return lhs <= rhs && lhs != rhs;
  }


  template <typename S>
  inline
  std::ostream& operator<<(std::ostream& ostr, const Site_Set<S>& set_)
  {
    const S& set = exact(set_);
    ostr << '{';
    mln_piter(S) p(set);
    for_all(p)
      ostr << p;
    return ostr << '}';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


# include <mln/core/ops.hh>


#endif // ! MLN_CORE_CONCEPT_POINT_SET_HH
