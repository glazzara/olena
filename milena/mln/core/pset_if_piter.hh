// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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
 * \brief Definition of iterators on pset_if<S,F>.
 *
 * \todo See todo in site_iterator_base.hh
 */

# include <mln/core/internal/site_set_iterator_base.hh>
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
    : public internal::site_set_iterator_base< pset_if<S,F>,
					       pset_if_fwd_piter_<S,F> >
  {
  public:

    /// Constructor without argument.
    pset_if_fwd_piter_();

    /// Constructor from a site set.
    pset_if_fwd_piter_(const pset_if<S,F>& s);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    mln_fwd_piter(S)& hook_pi_() { return pi_; }

  private:

    mln_fwd_piter(S) pi_;
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
  pset_if_fwd_piter_<S,F>::pset_if_fwd_piter_()
  {
  }

  template <typename S, typename F>
  inline
  pset_if_fwd_piter_<S,F>::pset_if_fwd_piter_(const pset_if<S,F>& s)
  {
    pi_.change_target(s.overset());
    this->change_target(s);
  }

  template <typename S, typename F>
  inline
  bool
  pset_if_fwd_piter_<S,F>::is_valid_() const
  {
    return pi_.is_valid();
  }

  template <typename S, typename F>
  inline
  void
  pset_if_fwd_piter_<S,F>::invalidate_()
  {
    pi_.invalidate();
  }

  template <typename S, typename F>
  inline
  void
  pset_if_fwd_piter_<S,F>::start_()
  {
    pi_.start();
    while (pi_.is_valid() && ! this->s_->pred(pi_))
      pi_.next();
    if (is_valid_())
      this->p_ = pi_;
  }

  template <typename S, typename F>
  inline
  void
  pset_if_fwd_piter_<S,F>::next_()
  {
    do
      pi_.next();
    while (pi_.is_valid() && ! this->s_->pred(pi_));
    if (is_valid_())
      this->p_ = pi_;
  }
  

  // FIXME: pset_if_bkd_piter_<S,F>


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_PSET_IF_PITER_HH
