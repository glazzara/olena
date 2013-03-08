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

#ifndef MLN_CORE_SITE_SET_P_RUN_PITER_HH
# define MLN_CORE_SITE_SET_P_RUN_PITER_HH

/*! \file
 *
 * \brief Definition of point iterators on mln::p_run.
 */

# include <mln/core/site_set/p_run.hh>
# include <mln/core/internal/site_set_iterator_base.hh>


namespace mln
{

  /*!
    \internal
    \brief Forward iterator on points of a p_run<P>.
  */
  template <typename P>
  class p_run_fwd_piter_
    :
    public internal::site_set_iterator_base< p_run<P>,
					     p_run_fwd_piter_<P> >
  {
    typedef p_run_fwd_piter_<P> self_;
    typedef internal::site_set_iterator_base< p_run<P>, self_ > super_;
  public:

    /// Constructor without arguments.
    p_run_fwd_piter_();

    /// Coordinate associated type.
    p_run_fwd_piter_(const p_run<P>& r);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

  protected:
    using super_::p_;
    using super_::s_;
  };



  /*!
    \internal
    \brief Backward iterator on points of a p_run<P>.
  */
  template <typename P>
  class p_run_bkd_piter_
    :
    public internal::site_set_iterator_base< p_run<P>,
					     p_run_bkd_piter_<P> >
  {
    typedef p_run_bkd_piter_<P> self_;
    typedef internal::site_set_iterator_base< p_run<P>, self_ > super_;
  public:

    /// Constructor without arguments.
    p_run_bkd_piter_();

    /// Coordinate associated type.
    p_run_bkd_piter_(const p_run<P>& r);

    /// Test if the iterator is valid.
    bool is_valid_() const;

    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

  protected:
    using super_::p_;
    using super_::s_;
  };



# ifndef MLN_INCLUDE_ONLY

  // p_run_fwd_piter_<P>

  template <typename P>
  inline
  p_run_fwd_piter_<P>::p_run_fwd_piter_()
  {
  }

  template <typename P>
  inline
  p_run_fwd_piter_<P>::p_run_fwd_piter_(const p_run<P>& r)
  {
    this->change_target(r);
  }

  template <typename P>
  inline
  bool
  p_run_fwd_piter_<P>::is_valid_() const
  {
    mln_invariant(p_.index() >= 0);
    return p_.index() < int(s_->length());
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::invalidate_()
  {
    p_.change_index(s_->length());
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::start_()
  {
    p_.change_index(0);
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::next_()
  {
    p_.inc_index();
  }


  // p_run_bkd_piter_<P>

  template <typename P>
  inline
  p_run_bkd_piter_<P>::p_run_bkd_piter_()
  {
  }

  template <typename P>
  inline
  p_run_bkd_piter_<P>::p_run_bkd_piter_(const p_run<P>& r)
  {
    this->change_target(r);
  }

  template <typename P>
  inline
  bool
  p_run_bkd_piter_<P>::is_valid_() const
  {
    mln_invariant(p_.index() < int(s_->length()));
    return p_.index() >= 0;
  }

  template <typename P>
  inline
  void
  p_run_bkd_piter_<P>::invalidate_()
  {
    p_.change_index(-1);
  }

  template <typename P>
  inline
  void
  p_run_bkd_piter_<P>::start_()
  {
    p_.change_index(s_->length() - 1);
  }

  template <typename P>
  inline
  void
  p_run_bkd_piter_<P>::next_()
  {
    p_.dec_index();
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_RUN_PITER_HH
