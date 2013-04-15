// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_SITE_SET_P_IF_PITER_HH
# define MLN_CORE_SITE_SET_P_IF_PITER_HH

/*! \file
 *
 * \brief Definition of iterators on p_if<S,F>.
 */

# include <mln/core/internal/piter_adaptor.hh>
# include <mln/core/site_set/p_if.hh>


namespace mln
{


  /*!
    \brief Iterator on site sets conditionned by a function.

    Parameter \c S is a site set type; parameter F is a function
    from point to Boolean.

    \see mln::p_if
  */
  template <typename Pi, typename S, typename F>
  struct p_if_piter_
    : public internal::piter_adaptor_< Pi,                       // Adaptee.
				       p_if<S,F>,             // Site_Set.
				       p_if_piter_<Pi,S,F> >  // Exact.
  {
    /// Constructor without argument.
    p_if_piter_();

    /// Constructor from a site set.
    p_if_piter_(const p_if<S,F>& s);

    /// \cond INTERNAL_API
    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// Change the set site targeted by pi_.
    void pi_change_target_(const p_if<S,F>& s);
    /// \endcond

  private:
    typedef p_if_piter_<Pi,S,F> self_;
    typedef internal::piter_adaptor_<Pi, p_if<S,F>, self_> super_;

  protected:
    using super_::s_;
    using super_::pi_;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename Pi, typename S, typename F>
  inline
  p_if_piter_<Pi,S,F>::p_if_piter_()
  {
  }

  template <typename Pi, typename S, typename F>
  inline
  p_if_piter_<Pi,S,F>::p_if_piter_(const p_if<S,F>& s)
  {
    this->change_target(s);
  }

  template <typename Pi, typename S, typename F>
  inline
  void
  p_if_piter_<Pi,S,F>::start_()
  {
    pi_.start();
    while (pi_.is_valid() && ! s_->pred(pi_))
      pi_.next();
  }

  template <typename Pi, typename S, typename F>
  inline
  void
  p_if_piter_<Pi,S,F>::next_()
  {
    do
      pi_.next();
    while (pi_.is_valid() && ! s_->pred(pi_));
  }

  template <typename Pi, typename S, typename F>
  inline
  void
  p_if_piter_<Pi,S,F>::pi_change_target_(const p_if<S,F>& s)
  {
    pi_.change_target(s.overset());
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_IF_PITER_HH
