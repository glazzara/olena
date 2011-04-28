// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_IF_HH
# define MLN_CORE_SITE_SET_P_IF_HH

/// \file
///
/// \brief Definition of the restriction of a site set w.r.t. a
/// predicate.
///
/// \todo Change s_ attribute type to S*.

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/concept/function.hh>


namespace mln
{

  // Forward declarations.
  template <typename S, typename F> class p_if;
  template <typename Pi, typename S, typename F> struct p_if_piter_;


  namespace trait
  {

    template <typename S, typename F>
    struct site_set_< p_if<S,F> >
    {
      typedef trait::site_set::nsites::unknown nsites;
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef mln_trait_site_set_arity(S)      arity;
    };

  } // end of namespace trait


  /*! \brief Restrict a site set \p s to points that verify \p f.
   *
   * \param[in] s A site set.
   * \param[in] f A function from point to Boolean.
   * \return A subset of points.
   */
  template <typename S, typename F>
  p_if<S, F>
  operator | (const Site_Set<S>& s, const Function_v2b<F>& f);



  /// \brief Site set restricted w.r.t. a predicate.
  ///
  /// \ingroup modsitesetsparse
  /*!
   * Parameter \c S is a site set type; parameter F is a function
   * from point to Boolean.
   */
  template <typename S, typename F>
  class p_if : public internal::site_set_base_< mln_psite(S), p_if<S,F> >
  {
    typedef p_if<S,F> self_;
    typedef internal::site_set_base_<mln_psite(S), self_> super_;
  public:

    /// Element associated type.
    typedef mln_element(S) element;


    /// Psite associated type.
    typedef mln_psite(S) psite;

    /// Forward Site_Iterator associated type.
    typedef p_if_piter_<mln_fwd_piter(S), S, F> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_if_piter_<mln_bkd_piter(S), S, F> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor with a site set \p s and a predicate \p f.
    p_if(const S& s, const F& f);

    /// Constructor without argument.
    p_if();


    /// Test if this site set is valid.
    bool is_valid() const;


    /// Test if \p p belongs to the subset.
    bool has(const psite& p) const;

    /// Give the primary overset.
    const S& overset() const;

    /// Test predicate on point site \p p.
    bool pred(const psite& p) const;

    /// Give the predicate function.
    const F& predicate() const;


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

  protected:

    S s_;
    F f_;
  };



# ifndef MLN_INCLUDE_ONLY


  template <typename S, typename F>
  inline
  p_if<S, F>
  operator | (const Site_Set<S>& s, const Function_v2b<F>& f)
  {
    p_if<S, F> tmp(exact(s), exact(f));
    return tmp;
  }


  // p_if<S,F>

  template <typename S, typename F>
  inline
  bool
  p_if<S,F>::has(const psite& p) const
  {
    return s_.has(p) && f_(p) == true;
  }

  template <typename S, typename F>
  inline
  bool
  p_if<S,F>::is_valid() const
  {
    return s_.is_valid();
  }

  template <typename S, typename F>
  inline
  const S&
  p_if<S,F>::overset() const
  {
    return s_;
  }

  template <typename S, typename F>
  inline
  bool
  p_if<S,F>::pred(const psite& p) const
  {
    return f_(p);
  }

  template <typename S, typename F>
  inline
  p_if<S,F>::p_if(const S& s, const F& f)
    : s_(s),
      f_(f)
  {
  }

  template <typename S, typename F>
  inline
  p_if<S,F>::p_if()
  {
  }

  template <typename S, typename F>
  inline
  const F&
  p_if<S,F>::predicate() const
  {
    return f_;
  }

  template <typename S, typename F>
  inline
  std::size_t
  p_if<S,F>::memory_size() const
  {
    return s_.memory_size() + sizeof(f_);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/site_set/p_if_piter.hh>



#endif // ! MLN_CORE_SITE_SET_P_IF_HH
