// Copyright (C) 2009, 2011, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_TRANSFORMED_HH
# define MLN_CORE_SITE_SET_P_TRANSFORMED_HH

/// \file
///
/// Definition of a site set defined by the transformation of another
/// site set.

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/concept/function.hh>
# include <mln/convert/from_to.hh>

namespace mln
{

  // Forward declarations.
  template <typename S, typename F> class p_transformed;
  template <typename Pi, typename S, typename F> struct p_transformed_piter;


  namespace trait
  {

    template <typename S, typename F>
    struct site_set_< p_transformed<S,F> >
    {
      typedef trait::site_set::nsites::unknown nsites;
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::multiple arity;
    };

  } // end of namespace trait


  /// Transform a site set \p s through the function \p f.
  ///
  /// \param[in] s A site set.
  /// \param[in] f A function from site to site.
  ///
  /// \return The transformed site set.
  ///
  /// \relates p_transformed<S,F>
  //
  template <typename S, typename F>
  p_transformed<S, F>
  ptransform(const Site_Set<S>& s, const Function_v2v<F>& f);



  /// \brief Site set transformed through a function.
  ///
  /// \ingroup modsitesetsparse
  ///
  /// Parameter \c S is a site set type; parameter F is a function
  /// from site to site.
  //
  template <typename S, typename F>
  class p_transformed
    : public internal::site_set_base_< mln_psite(S), p_transformed<S,F> >,
      private mlc_converts_to(mln_result(F), mln_psite(S))::check_t
  {
    typedef p_transformed<S,F> self_;
    typedef internal::site_set_base_<mln_result(F), self_> super_;
  public:

    /// Element associated type.
    typedef mln_element(S) element;


    /// Psite associated type.
    typedef mln_psite(S) psite;

    /// Forward Site_Iterator associated type.
    typedef p_transformed_piter<mln_fwd_piter(S), S, F> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_transformed_piter<mln_bkd_piter(S), S, F> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor with a site set \p s and a predicate \p f.
    p_transformed(const S& s, const F& f);

    /// Constructor without argument.
    p_transformed();


    /// Test if this site set is valid.
    bool is_valid() const;


    /// Test if \p p belongs to the subset.
    bool has(const psite& p) const;


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Return the primary set.
    const S& primary_set() const;

    /// Return the transformation function.
    const F& function() const;

  protected:

    S s_;
    F f_;
  };



# ifndef MLN_INCLUDE_ONLY


  template <typename S, typename F>
  inline
  p_transformed<S, F>
  ptransform(const Site_Set<S>& s, const Function_v2v<F>& f)
  {
    mlc_equal(mln_result(F), mln_psite(S))::check();
    p_transformed<S, F> tmp(exact(s), exact(f));
    return tmp;
  }


  // p_transformed<S,F>

  template <typename S, typename F>
  inline
  p_transformed<S,F>::p_transformed()
  {
  }

  template <typename S, typename F>
  inline
  p_transformed<S,F>::p_transformed(const S& s, const F& f)
    : s_(s),
      f_(f)
  {
  }

  template <typename S, typename F>
  inline
  bool
  p_transformed<S,F>::has(const psite& p) const
  {
    mln_argument(F) arg;
    convert::from_to(p, arg);
    return s_.has(f_.inverse(arg));
  }

  template <typename S, typename F>
  inline
  bool
  p_transformed<S,F>::is_valid() const
  {
    return s_.is_valid();
  }

  template <typename S, typename F>
  inline
  std::size_t
  p_transformed<S,F>::memory_size() const
  {
    return s_.memory_size() + sizeof(f_);
  }

  template <typename S, typename F>
  inline
  const S&
  p_transformed<S,F>::primary_set() const
  {
    return s_;
  }

  template <typename S, typename F>
  inline
  const F&
  p_transformed<S,F>::function() const
  {
    return f_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/site_set/p_transformed_piter.hh>



#endif // ! MLN_CORE_SITE_SET_P_TRANSFORMED_HH
