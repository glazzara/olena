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

#ifndef MLN_CORE_PSET_IF_PITER_HH
# define MLN_CORE_PSET_IF_PITER_HH

/*! \file mln/core/pset_if_piter.hh
 *
 * \brief Definition of iterators on points of pset_ifes.
 */

# include <mln/core/concept/site_iterator.hh>
# include <mln/core/internal/piter_adaptor.hh>
# include <mln/core/pset_if.hh>


namespace mln
{

  /*! \brief A generic forward iterator on points of subsets.
   *
   * Parameter \c S is a point set type; parameter F is a function
   * from point to Boolean.
   *
   * \see mln::pset_if
   */
  template <typename S, typename F>
  class pset_if_fwd_piter_
    : public internal::piter_adaptor_< mln_fwd_piter(S),
				       pset_if_fwd_piter_<S,F> >
  {
    typedef mln_fwd_piter(S) adaptee_;
    typedef pset_if_fwd_piter_<S,F> self_;
    typedef internal::piter_adaptor_<adaptee_, self_> super_;

  public:

    /// Constructor from a subset of points.
    pset_if_fwd_piter_(const pset_if<S,F>& subset);

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

  private:

    const pset_if<S,F>& subset_;
  };


  // FIXME:
  template <typename S, typename F>
  class pset_if_bkd_piter_
    :
    public mln::internal::fixme
  {};



# ifndef MLN_INCLUDE_ONLY


  // pset_if_fwd_piter_<S,F>

  template <typename S, typename F>
  inline
  pset_if_fwd_piter_<S,F>::pset_if_fwd_piter_(const pset_if<S,F>& subset)
    : super_(adaptee_(subset.overset())),
      subset_(subset)
  {
  }

  template <typename S, typename F>
  inline
  void
  pset_if_fwd_piter_<S,F>::start()
  {
    this->piter_.start();
    while (this->piter_.is_valid() && ! subset_.pred(this->piter_))
      this->piter_.next();
  }

  template <typename S, typename F>
  inline
  void
  pset_if_fwd_piter_<S,F>::next_()
  {
    do
      this->piter_.next();
    while (this->piter_.is_valid() && ! subset_.pred(this->piter_));
  }
  

  // FIXME: pset_if_bkd_piter_<S,F>


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_PSET_IF_PITER_HH
