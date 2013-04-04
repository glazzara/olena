// Copyright (C) 2007, 2008, 2009, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_CORE_CONCEPT_SITE_SET_HH
# define MLN_CORE_CONCEPT_SITE_SET_HH

/// \file
///
/// Definition of the concept of mln::Site_Set.
///
/// \todo Add a "composite" property.

# include <set>

# include <mln/core/concept/site_iterator.hh>
# include <mln/trait/site_sets.hh>
# include <mln/metal/not_equal.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/is_unqualif.hh>
# include <mln/util/ord.hh>


namespace mln
{

  // Forward declaration.
  template <typename E> struct Site_Set;


  /// \cond INTERNAL_API
  /// \brief Site_Set category flag type.
  template <>
  struct Site_Set<void>
  {
    typedef Object<void> super;
  };
  /// \endcond


  /*!
    \brief Base class for implementation classes of site sets.

    \see mln::doc::Site_Set for a complete documentation of this
    class contents.

    \ingroup modconcepts
  */
  template <typename E>
  struct Site_Set : public Object<E>
  {
    typedef Site_Set<void> category;

    /*
      typedef element;

      typedef site;
      typedef psite;

      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;
      bool is_valid() const;

      std::size_t memory_size() const;
     */

  protected:
    Site_Set();
  };


  /*!
    \brief Conversion: site_set -> std::set
    \ingroup fromto
  */
  template <typename S, typename P, typename C_>
  void
  from_to_(const Site_Set<S>& from, std::set<P,C_>& to);

} // end of namespace mln

namespace std
{

  /*!
    \brief Conversion: std::set -> site_set
    \ingroup fromto
  */
  template <typename P, typename C_, typename S>
  void
  from_to_(const set<P,C_>& from, mln::Site_Set<S>& to);

} // end of namespace std


namespace mln
{

  namespace trait
  {

    template <typename S>
    struct set_unary_< op::ord, Site_Set, S >
    {
      // No definition here.

      // That prevents from calling the inclusion operator ("op<")
      // which is clearly not the expected ordering...

      // Consequently site sets have to define the way they are
      // ordered.
    };

  } // end of namespace mln::trait

} // end of namespace mln

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace internal
  {

    // nsites: known or unknown.

    template <typename trait_nsites, typename E>
    struct site_set_nsites_check
    {
      static void run() { /* No requirement. */ }
    };

    template <typename E>
    struct site_set_nsites_check< mln::trait::site_set::nsites::known, E >
    {
      static void run()
      {
	unsigned (E::*m)() const = & E::nsites;
	(void) m;
      }
    };

    // bbox: unknown, lazy, or straight.

    template <typename trait_bbox, typename E>
    struct site_set_bbox_check
    {
      static void run()
      {
	typedef typename E::q_box q_box;
	q_box (E::*m)() const = & E::bbox;
	(void) m;
      }
    };

    template <typename E>
    struct site_set_bbox_check< mln::trait::site_set::bbox::unknown, E >
    {
      static void run() { /* No requirement. */ }
    };

    // contents: fixed, growing, or free.

    template <typename trait_contents, typename E>
    struct site_set_contents_check;

    template <typename E>
    struct site_set_contents_check< mln::trait::site_set::contents::fixed, E >
    {
      static void run() { /* No requirement. */ }
    };

    template <typename E>
    struct site_set_contents_check< mln::trait::site_set::contents::growing, E >
    {
      static void run()
      {
	typedef typename E::i_element i_element;
	mlc_equal(mlc_unqualif(i_element), i_element)::check();
	void (E::*m1)(const i_element&) = & E::insert;
	(void) m1;
	void (E::*m2)() = & E::clear;
	(void) m2;
      }
    };

    template <typename E>
    struct site_set_contents_check< mln::trait::site_set::contents::free, E >
    {
      static void run()
      {
	typedef typename E::i_element i_element;
	mlc_equal(mlc_unqualif(i_element), i_element)::check();
	void (E::*m1)(const i_element&) = & E::insert;
	(void) m1;
	typedef typename E::r_element r_element;
	mlc_equal(mlc_unqualif(r_element), r_element)::check();
	void (E::*m2)(const r_element&) = & E::remove;
	(void) m2;
	void (E::*m3)() = & E::clear;
	(void) m3;
      }
    };

  } // end of namespace mln::internal


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
    // FIXME: also test something like
    //        mlc_is_a( mln_trait_site_set_nsites(E),   mln::trait::site_set::nsites::any   )::check();

    // Check associated types.
    typedef mln_element(E) element;
    mlc_is_unqualif(element)::check();
    typedef mln_site(E)  site;
    typedef mln_psite(E) psite;
    typedef mln_piter(E) piter;
    typedef mln_fwd_piter(E) fwd_piter;
    typedef mln_bkd_piter(E) bkd_piter;

    // Check methods.
    bool (E::*m1)(const psite& p) const = & E::has;
    (void) m1;
    bool (E::*m2)() const = & E::is_valid;
    (void) m2;
    std::size_t (E::*m3)() const = & E::memory_size;
    (void) m3;

    // Check methods depending upon properties.
    internal::site_set_nsites_check  < mln_trait_site_set_nsites(E),   E >::run();
    internal::site_set_bbox_check    < mln_trait_site_set_bbox(E),     E >::run();
    internal::site_set_contents_check< mln_trait_site_set_contents(E), E >::run();
  }


  // Conversion

  template <typename S, typename P, typename C_>
  inline
  void
  from_to_(const Site_Set<S>& from_, std::set<P,C_>& to)
  {
    mlc_converts_to(mln_psite(S), P)::check();
    const S& from = exact(from_);
    to.clear();
    mln_piter(S) p(from);
    for_all(p)
      to.insert(p);
  }

} // end of namespace mln

namespace std
{

  template <typename P, typename C_, typename S>
  inline
  void
  from_to_(const set<P,C_>& from, mln::Site_Set<S>& to_)
  {
    mlc_converts_to(P, mln_i_element(S))::check();
    S& to = exact(to_);
    to.clear();
    for (typename set<P>::const_iterator i = from.begin();
	 i != from.end();
	 ++i)
      to.insert(*i);
  }

} // end of namespace std


# endif // ! MLN_INCLUDE_ONLY


# include <mln/core/site_set/operators.hh>
# include <mln/core/routine/ops.hh>


#endif // ! MLN_CORE_CONCEPT_SITE_SET_HH
