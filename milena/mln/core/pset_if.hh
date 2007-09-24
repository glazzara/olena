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

#ifndef MLN_CORE_PSET_IF_HH
# define MLN_CORE_PSET_IF_HH

/*! \file mln/core/pset_if.hh
 *
 * \brief Definition of the restriction of a point set w.r.t. a predicate.
 */

# include <mln/core/internal/point_set_base.hh>
# include <mln/core/concept/function.hh>


namespace mln
{

  // Fwd decls.
  template <typename S, typename F> struct pset_if;
  template <typename S, typename F> struct pset_if_fwd_piter_;
  template <typename S, typename F> struct pset_if_bkd_piter_;



  /*! \brief Restrict a point set \p pset to points that verify \p f.
   *
   * \param[in] pset A point set.
   * \param[in] f A function from point to Boolean.
   * \return A subset of points.
   */
  template <typename S, typename F>
  pset_if<S, F>
  operator | (const Point_Set<S>& pset, const Function_p2b<F>& f);



  /*! \brief Generic subset class.
   *
   * Parameter \c S is a point set type; parameter F is a function
   * from point to Boolean.
   */
  template <typename S, typename F>
  class pset_if : public internal::point_set_base_< mln_psite(S), pset_if<S,F> >
  {
    typedef pset_if<S,F> self_;
    typedef internal::point_set_base_<mln_psite(S), self_> super_;
  public:

    typedef mln_psite(super_) psite;

    /// Forward Point_Iterator associated type.
    typedef pset_if_fwd_piter_<S,F> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef internal::fixme bkd_piter;


    /// Constructor with a point set \p pset and a predicate \p f.
    pset_if(const S& pset, const F& f);

    /// Constructor without argument.
    pset_if();


    /// Test if \p p belongs to the subset. 
    bool has(const psite& p) const;

    /// Give a bounding box of the subset. 
    const box_<mln_point(S)>& bbox() const;

    /// Give the number of points of the subset.
    std::size_t npoints() const;

    /// Give the primary overset.
    const S& overset() const;

    /// Test predicate on point site \p p.
    bool pred(const psite& p) const;

    /// Give the predicate function.
    const F& predicate() const;

  protected:

    S pset_;
    F f_;
  };



# ifndef MLN_INCLUDE_ONLY


  template <typename S, typename F>
  pset_if<S, F>
  operator | (const Point_Set<S>& pset, const Function_p2b<F>& f)
  {
    pset_if<S, F> tmp(exact(pset), exact(f));
    return tmp;
  }


  // pset_if<S,F>

  template <typename S, typename F>
  bool
  pset_if<S,F>::has(const psite& p) const
  {
    return pset_.has(p) && f_(p);
  }

  template <typename S, typename F>
  const box_<mln_point(S)>&
  pset_if<S,F>::bbox() const
  {
    return pset_.bbox();
  }

  template <typename S, typename F>
  const S&
  pset_if<S,F>::overset() const
  {
    return pset_;
  }

  template <typename S, typename F>
  bool
  pset_if<S,F>::pred(const psite& p) const
  {
    return f_(p);
  }
  
  template <typename S, typename F>
  pset_if<S,F>::pset_if(const S& pset, const F& f)
    : pset_(pset),
      f_(f)
  {
  }

  template <typename S, typename F>
  pset_if<S,F>::pset_if()
  {
  }

  template <typename S, typename F>
  const F&
  pset_if<S,F>::predicate() const
  {
    return f_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/pset_if_piter.hh>



namespace mln
{

# ifndef MLN_INCLUDE_ONLY

  template <typename S, typename F>
  std::size_t
  pset_if<S,F>::npoints() const
  {
    std::size_t n = 0;
    fwd_piter p(*this);
    for_all(p)
      ++n;
    return n;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_PSET_IF_HH
