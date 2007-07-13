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

#ifndef MLN_CORE_PSUBSET_HH
# define MLN_CORE_PSUBSET_HH

/*! \file mln/core/psubset.hh
 *
 * \brief This file defines a generic class for subset of point set.
 */

# include <mln/core/concept/point_set.hh>
# include <mln/core/concept/function.hh>


namespace mln
{

  // Fwd decls.
  template <typename S, typename F> struct psubset;
  template <typename S, typename F> struct psubset_fwd_piter_;
  template <typename S, typename F> struct psubset_bkd_piter_;



  /*! \brief Restrict a point set \p pset to points that verify \p f.
   *
   * \param[in] pset A point set.
   * \param[in] f A function from point to Boolean.
   * \return A subset of points.
   */
  template <typename S, typename F>
  psubset<S, F>
  operator | (const Point_Set<S>& pset, const Function_p2b<F>& f);



  /*! \brief Generic subset class.
   *
   * Parameter \c S is a point set type; parameter F is a function
   * from point to Boolean.
   */
  template <typename S, typename F>
  struct psubset : public Point_Set< psubset<S,F> >
  {
    /// Psite associated type.
    typedef mln_psite(S) psite;

    /// Point associated type.
    typedef mln_point(S) point;

    
    /// Forward Piter associated type.
    typedef psubset_fwd_piter_<S,F> fwd_piter;

    /// Backward Piter associated type.
    typedef psubset_fwd_piter_<S,F> bkd_piter; // FIXME: bkd!!!

    /// Piter associated type.
    typedef fwd_piter piter;


    /// Constructor without argument.
    psubset(const S& pset, const F& f);


    /// Test if \p p belongs to the subset. 
    bool has(const psite& p) const;

    /// Give a bounding box of the subset. 
    const box_<point>& bbox() const;

    /// Give the number of points of the subset.
    std::size_t npoints() const;

    /// Give the primary overset.
    const S& overset() const;

    /// Test predicate on point site \p p.
    bool pred(const psite& p) const;

  protected:

    const S& pset_;
    F f_;
  };



# ifndef MLN_INCLUDE_ONLY


  template <typename S, typename F>
  psubset<S, F>
  operator | (const Point_Set<S>& pset, const Function_p2b<F>& f)
  {
    psubset<S, F> tmp(exact(pset), exact(f));
    return tmp;
  }


  // psubset<S,F>

  template <typename S, typename F>
  bool
  psubset<S,F>::has(const psite& p) const
  {
    return pset_.has(p) && f_(p);
  }

  template <typename S, typename F>
  const box_<mln_point(S)>&
  psubset<S,F>::bbox() const
  {
    return pset_.bbox();
  }

  template <typename S, typename F>
  const S&
  psubset<S,F>::overset() const
  {
    return pset_;
  }

  template <typename S, typename F>
  bool
  psubset<S,F>::pred(const psite& p) const
  {
    return f_(p);
  }
  
  template <typename S, typename F>
  psubset<S,F>::psubset(const S& pset, const F& f)
    : pset_(pset),
      f_(f)
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/psubset_piter.hh>



namespace mln
{

# ifndef MLN_INCLUDE_ONLY

  template <typename S, typename F>
  std::size_t
  psubset<S,F>::npoints() const
  {
    std::size_t n = 0;
    piter p(*this);
    for_all(p)
      ++n;
    return n;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_PSUBSET_HH
