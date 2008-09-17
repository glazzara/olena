// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_SITE_SET_P_COMPLEX_PITER_HH
# define MLN_CORE_SITE_SET_P_COMPLEX_PITER_HH

/// \file mln/core/site_set/p_complex_piter.hh
/// \brief Definition of point iterator on complex-based pset.

# include <mln/core/internal/p_complex_piter_base.hh>

namespace mln
{
  /* FIXME: Get rid of P?  */


  /*-----------------------------.
  | p_complex_fwd_piter_<D, P>.  |
  `-----------------------------*/

  /// \brief Forward iterator on point sites of a mln::p_complex<D, P>.
  template <unsigned D, typename P>
  class p_complex_fwd_piter_
    : public internal::p_complex_piter_base_< complex_fwd_iter_<D>,
					      P,
					      p_complex_fwd_piter_<D, P> >
  {
    typedef p_complex_fwd_piter_<D, P> self_;
    typedef
    internal::p_complex_piter_base_< complex_fwd_iter_<D>, P, self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_complex_fwd_piter_(const p_complex<D, P>& pc);
    p_complex_fwd_piter_(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}
  };


  /*-----------------------------.
  | p_complex_bkd_piter_<D, P>.  |
  `-----------------------------*/

  /// \brief Backward iterator on point sites of a mln::p_complex<D, P>.
  template <unsigned D, typename P>
  class p_complex_bkd_piter_
    : public internal::p_complex_piter_base_< complex_bkd_iter_<D>,
					      P,
					      p_complex_bkd_piter_<D, P> >
  {
    typedef p_complex_bkd_piter_<D, P> self_;
    typedef
    internal::p_complex_piter_base_< complex_bkd_iter_<D>, P, self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_complex_bkd_piter_(const p_complex<D, P>& pc);
    p_complex_bkd_piter_(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}
  };



# ifndef MLN_INCLUDE_ONLY

  /*-----------------------------.
  | p_complex_fwd_piter_<D, P>.  |
  `-----------------------------*/

  template <unsigned D, typename P>
  inline
  p_complex_fwd_piter_<D, P>::p_complex_fwd_piter_(const p_complex<D, P>& pc)
    : super_(pc)
  {
  }

  template <unsigned D, typename P>
  inline
  p_complex_fwd_piter_<D, P>::p_complex_fwd_piter_(const p_complex_fwd_piter_<D, P>& rhs)
    : super_(rhs)
  {
  }

  template <unsigned D, typename P>
  inline
  p_complex_fwd_piter_<D, P>&
  p_complex_fwd_piter_<D, P>::operator=(const p_complex_fwd_piter_<D, P>& rhs)
  {
    if (&rhs == this)
      return *this;
    super_::operator=(rhs);
    return *this;
  }


  /*-----------------------------.
  | p_complex_bkd_piter_<D, P>.  |
  `-----------------------------*/

  template <unsigned D, typename P>
  inline
  p_complex_bkd_piter_<D, P>::p_complex_bkd_piter_(const p_complex<D, P>& pc)
    : super_(pc)
  {
  }

  template <unsigned D, typename P>
  inline
  p_complex_bkd_piter_<D, P>::p_complex_bkd_piter_(const p_complex_bkd_piter_<D, P>& rhs)
    : super_(rhs)
  {
  }

  template <unsigned D, typename P>
  inline
  p_complex_bkd_piter_<D, P>&
  p_complex_bkd_piter_<D, P>::operator=(const p_complex_bkd_piter_<D, P>& rhs)
  {
    if (&rhs == this)
      return *this;
    super_::operator=(rhs);
    return *this;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_SITE_SET_P_COMPLEX_PITER_HH
