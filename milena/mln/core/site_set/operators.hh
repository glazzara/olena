// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_OPERATORS_HH
# define MLN_CORE_SITE_SET_OPERATORS_HH

/// \file
///
/// Definition of operators on mln::Site_Set.
///
/// \todo Fix code for multi-sets.


# include <algorithm>
# include <mln/core/concept/site_set.hh>



namespace mln
{

  // Forward declarations.
  template <typename E> struct Box;
  namespace set { template <typename S> unsigned card(const Site_Set<S>& s); }


  template <typename Sl, typename Sr>
  Sl&
  operator+=(Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);


  /// Equality test between site sets \p lhs and \p rhs.
  ///
  /// \param[in] lhs A site set.
  /// \param[in] rhs Another site set.
  ///
  /// \relates mln::Site_Set
  ///
  template <typename Sl, typename Sr>
  bool
  operator==(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /// Inclusion test between site sets \p lhs and \p rhs.
  ///
  /// \param[in] lhs A site set (included?).
  /// \param[in] rhs Another site set (includer?).
  ///
  /// \relates mln::Site_Set
  ///
  template <typename Sl, typename Sr>
  bool
  operator<=(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



  /// Strict inclusion test between site sets \p lhs and \p
  ///  rhs.
  ///
  /// \param[in] lhs A site set (strictly included?).
  /// \param[in] rhs Another site set (includer?).
  ///
  /// \relates mln::Site_Set
  ///
  template <typename Sl, typename Sr>
  bool
  operator<(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



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
  std::ostream&
  operator<<(std::ostream& ostr, const Site_Set<S>& set);



# ifndef MLN_INCLUDE_ONLY


  namespace internal
  {

    template <typename Sl, typename Sr>
    inline
    std::set< mln_site(Sl), util::ord<mln_site(Sl)> >
    sym_diff_std_set(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
    {
      typedef mln_site(Sl) P;
      mlc_converts_to(mln_psite(Sr), P)::check();
      std::set< P, util::ord<P> > sl, sr, sd;
      from_to_(lhs, sl);
      from_to_(rhs, sr);
      std::set_symmetric_difference(sl.begin(), sl.end(),
				    sr.begin(), sr.end(),
				    std::inserter(sd, sd.begin()),
				    util::ord<P>());
      return sd;
    }

    template <typename S>
    inline
    std::set< mln_site(S), util::ord<mln_site(S)> >
    to_std_set(const Site_Set<S>& s)
    {
      std::set< mln_site(S), util::ord<mln_site(S)> > std_s;
      from_to_(s, std_s);
      return std_s;
    }

    template <typename Sl, typename Sr>
    inline
    bool
    leq_std_set(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
    {
      typedef mln_site(Sl) P;
      mlc_converts_to(mln_psite(Sr), P)::check();
      std::set< P, util::ord<P> > sl, sr;
      from_to_(lhs, sl);
      from_to_(rhs, sr);
      return std::includes(sr.begin(), sr.end(),
			   sl.begin(), sl.end(),
			   util::ord<P>());
    }


    // card.

    template <typename S>
    inline
    unsigned set_card_dispatch_(mln::trait::site_set::nsites::any,
				const S& s)
    {
      unsigned n = 0;
      mln_piter(S) p(s);
      for_all(p)
	++n;
      return n;
    }

    template <typename S>
    inline
    unsigned set_card_dispatch_(mln::trait::site_set::nsites::known,
				const S& s)
    {
      return s.nsites();
    }

    template <typename S>
    inline
    unsigned set_card(const Site_Set<S>& s)
    {
      return set_card_dispatch_(mln_trait_site_set_nsites(S)(),
				exact(s));
    }

  } // end of namespace mln::internal


  namespace impl
  {

    // Implementations for "operator ==" between site sets.

    template <typename Bl, typename Br>
    inline
    bool
    operator_equal_boxes(const Box<Bl>& lhs_, const Box<Br>& rhs_)
    {
      const Bl& lhs = exact(lhs_);
      const Br& rhs = exact(rhs_);
      if (lhs.is_empty() != rhs.is_empty())
	return false;
      if (lhs.is_empty() && rhs.is_empty())
	return true;
      return lhs.pmin() == rhs.pmin() && lhs.pmax() == rhs.pmax();
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_uniques(const Site_Set<Sl>& lhs,
			   const Site_Set<Sr>& rhs)
    {
      if (internal::set_card(lhs) != internal::set_card(rhs))
	return false;
      return mln::internal::sym_diff_std_set(lhs, rhs).empty();
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_unique_multiple(const Site_Set<Sl>& lhs,
				   const Site_Set<Sr>& rhs)
    {
      if (internal::set_card(lhs) != internal::set_card(rhs))
	return false;
      return mln::internal::to_std_set(lhs) == mln::internal::to_std_set(rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_multiples(const Site_Set<Sl>& lhs,
			     const Site_Set<Sr>& rhs)
    {
      // FIXME: Approximate code...
      if (internal::set_card(lhs) != internal::set_card(rhs))
	return false;
      return mln::internal::to_std_set(lhs) == mln::internal::to_std_set(rhs);
    }


    // Implementations for "operator <" between site sets.

    template <typename Bl, typename Br>
    inline
    bool
    operator_less_boxes(const Box<Bl>& lhs_, const Box<Br>& rhs_)
    {
      const Bl& lhs = exact(lhs_);
      const Br& rhs = exact(rhs_);
      if (rhs.is_empty())
	return false;  // We cannot have "lhs < empty_set".
      // From this line, rhs is not empty.
      if (lhs.is_empty())
	return true;  // We have "empty set < a non empty set".
      // From here, both lhs and rhs are not empty.
      if (internal::set_card(lhs) >= internal::set_card(rhs))
	return false;
      return lhs.crop_wrt(rhs) == lhs;
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_uniques(const Site_Set<Sl>& lhs,
			  const Site_Set<Sr>& rhs)
    {
      if (internal::set_card(lhs) >= internal::set_card(rhs))
	return false;
      return mln::internal::leq_std_set(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_unique_multiple(const Site_Set<Sl>& lhs,
				  const Site_Set<Sr>& rhs)
    {
      if (internal::set_card(lhs) >= internal::set_card(rhs))
	return false;
      return mln::internal::leq_std_set(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_multiples(const Site_Set<Sl>& lhs,
			     const Site_Set<Sr>& rhs)
    {
      // FIXME: Approximate code...
      if (internal::set_card(lhs) >= internal::set_card(rhs))
	return false;
      return mln::internal::leq_std_set(lhs, rhs);
    }

  } // end of namespace mln::impl




  namespace internal
  {

    // Dispatch for "operator ==" between site sets.

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_dispatch(trait::site_set::arity::unique,
			    const Box<Sl>& lhs,
			    trait::site_set::arity::unique,
			    const Box<Sr>& rhs)
    {
      return impl::operator_equal_boxes(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_dispatch(trait::site_set::arity::unique,
			    const Site_Set<Sl>& lhs,
			    trait::site_set::arity::unique,
			    const Site_Set<Sr>& rhs)
    {
      return impl::operator_equal_uniques(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_dispatch(trait::site_set::arity::unique,
			    const Site_Set<Sl>& lhs,
			    trait::site_set::arity::multiple,
			    const Site_Set<Sr>& rhs)
    {
      return impl::operator_equal_unique_multiple(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_dispatch(trait::site_set::arity::multiple,
			    const Site_Set<Sl>& lhs,
			    trait::site_set::arity::unique,
			    const Site_Set<Sr>& rhs)
    {
      return impl::operator_equal_unique_multiple(rhs, lhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_dispatch(trait::site_set::arity::multiple,
			    const Site_Set<Sl>& lhs,
			    trait::site_set::arity::multiple,
			    const Site_Set<Sr>& rhs)
    {
      return impl::operator_equal_multiples(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_equal_dispatch(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
    {
      return operator_equal_dispatch(mln_trait_site_set_arity(Sl)(), exact(lhs),
				     mln_trait_site_set_arity(Sr)(), exact(rhs));
    }


    // Dispatch for "operator <" between site sets.

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_dispatch(trait::site_set::arity::unique,
			   const Box<Sl>& lhs,
			   trait::site_set::arity::unique,
			   const Box<Sr>& rhs)
    {
      return impl::operator_less_boxes(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_dispatch(trait::site_set::arity::unique,
			   const Site_Set<Sl>& lhs,
			   trait::site_set::arity::unique,
			   const Site_Set<Sr>& rhs)
    {
      return impl::operator_less_uniques(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_dispatch(trait::site_set::arity::unique,
			   const Site_Set<Sl>& lhs,
			   trait::site_set::arity::multiple,
			   const Site_Set<Sr>& rhs)
    {
      return impl::operator_less_unique_multiple(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_dispatch(trait::site_set::arity::multiple,
			   const Site_Set<Sl>& lhs,
			   trait::site_set::arity::unique,
			   const Site_Set<Sr>& rhs)
    {
      return impl::operator_less_unique_multiple(rhs, lhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_dispatch(trait::site_set::arity::multiple,
			   const Site_Set<Sl>& lhs,
			   trait::site_set::arity::multiple,
			   const Site_Set<Sr>& rhs)
    {
      return impl::operator_less_multiples(lhs, rhs);
    }

    template <typename Sl, typename Sr>
    inline
    bool
    operator_less_dispatch(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
    {
      return operator_less_dispatch(mln_trait_site_set_arity(Sl)(), exact(lhs),
				    mln_trait_site_set_arity(Sr)(), exact(rhs));
    }

  } // end of namespace mln::internal


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
  operator==(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
  {
    mlc_equal(mln_site(Sl), mln_site(Sr))::check();
    return internal::operator_equal_dispatch(lhs, rhs);
  }


  // Operator <.

  template <typename Sl, typename Sr>
  inline
  bool
  operator<(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
  {
    mlc_equal(mln_site(Sl), mln_site(Sr))::check();
    return internal::operator_less_dispatch(lhs, rhs);
  }


  // Operator <=.

  template <typename Sl, typename Sr>
  inline
  bool
  operator<=(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
  {
    mlc_equal(mln_site(Sl), mln_site(Sr))::check();
    if (internal::set_card(lhs) > internal::set_card(rhs))
      return false;
    return lhs < rhs || lhs == rhs;
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
