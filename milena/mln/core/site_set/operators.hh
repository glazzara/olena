// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_SITE_SET_OPERATORS_HH
# define MLN_CORE_SITE_SET_OPERATORS_HH

/// \file mln/core/site_set/operators.hh
///
/// Definition of operators on mln::Site_Set.
///
/// \todo Re-vamp this file now!


# include <mln/core/concept/site_set.hh>

# include <mln/util/yes.hh> // Temporary include.



namespace mln
{



  template <typename Sl, typename Sr>
  Sl& operator+=(Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);


  /// Equality test between site sets \p lhs and \p rhs.
  ///
  /// \param[in] lhs A site set.
  /// \param[in] rhs Another site set.
  ///
  /// \relates mln::Site_Set
  ///
  template <typename Sl, typename Sr>
  bool operator==(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /// Inclusion test between site sets \p lhs and \p rhs.
  ///
  /// \param[in] lhs A site set (included?).
  /// \param[in] rhs Another site set (includer?).
  ///
  /// \relates mln::Site_Set
  ///
  template <typename Sl, typename Sr>
  bool operator<=(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /// Strict inclusion test between site sets \p lhs and \p
  ///  rhs.
  ///
  /// \param[in] lhs A site set (strictly included?).
  /// \param[in] rhs Another site set (includer?).
  ///
  /// \relates mln::Site_Set
  ///
  template <typename Sl, typename Sr>
  bool operator<(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /// Print a site set \p set into the output stream \p
  ///  ostr.
  ///
  /// \param[in,out] ostr An output stream.
  /// \param[in] set A site set.
  ///
  /// \return The modified output stream \p ostr.
  ///
  /// \relates mln::Site_Set
  ///
  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const Site_Set<S>& set);



# ifndef MLN_INCLUDE_ONLY


  // Operator +=.

  template <typename Sl, typename Sr>
  inline
  Sl&
  operator+=(Site_Set<Sl>& lhs_, const Site_Set<Sr>& rhs)
  {
    mlc_is( mln_trait_site_set_contents(Sl),
	    mln::trait::site_set::contents::dynamic )::check();
    mlc_equal(mln_site(Sr), typename Sl::i_element)::check();
    Sl& lhs = exact(lhs_);
    mln_fwd_piter(Sr) p(exact(rhs));
    for_all(p)
      lhs.insert(p);
    return lhs;
  }


  // Operator ==.

  template <typename Sl, typename Sr>
  inline
  bool
  operator==(const Site_Set<Sl>&, const Site_Set<Sr>&)
  {
//     // FIXME: Same grid!
//     const Sl& lhs = exact(lhs_);
//     const Sr& rhs = exact(rhs_);

//     // exhaustive test:
//     mln_fwd_piter(Sl) pl(lhs);
//     mln_fwd_piter(Sr) pr(rhs);
//     for (pl.start(),      pr.start();
// 	 pl.is_valid() && pr.is_valid();
// 	 pl.next(),       pr.next())
//       if (pl != pr)
// 	return false; // difference found

//     // both sets are equal only if both browsings are completed
//     // at the same time:
//     return ! pl.is_valid() && ! pr.is_valid();
    return util::yes(true);
  }


  // Operator <=.

  template <typename Sl, typename Sr>
  inline
  bool
  operator<=(const Site_Set<Sl>&, const Site_Set<Sr>&)
  {
//     // FIXME: Same grid!
//     const Sl& lhs = exact(lhs_);
//     const Sr& rhs = exact(rhs_);

//     // exhaustive test:
//     mln_piter(Sl) pl(lhs);
//     for_all(pl)
//       if (! rhs.has(pl))
// 	return false;
    return util::yes(true);
  }


  // Operator <.

  template <typename Sl, typename Sr>
  inline
  bool
  operator<(const Site_Set<Sl>&, const Site_Set<Sr>&)
  {
//     // FIXME: Same grid!
//     const Sl& lhs = exact(lhs_);
//     const Sr& rhs = exact(rhs_);
//     return lhs <= rhs && lhs != rhs;
    return util::yes(true);
  }


  // Operator <<.

  template <typename S>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const Site_Set<S>& set_)
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


#endif // ! MLN_CORE_SITE_SET_OPERATORS_HH
