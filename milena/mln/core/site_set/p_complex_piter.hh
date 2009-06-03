// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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
///
/// Types defined here are akin to parametrized typedefs: the code is
/// factored in mln::internal::p_complex_piter_base_.

# include <mln/core/site_set/p_complex.hh>
# include <mln/core/internal/p_complex_piter_base.hh>
# include <mln/topo/face_iter.hh>

namespace mln
{

  // Forward declarations.
  template <unsigned D, typename G> class p_complex;

  template <unsigned D> class complex_fwd_iter_;
  template <unsigned D> class complex_bkd_iter_;


  /*-----------------------------.
  | p_complex_fwd_piter_<D, G>.  |
  `-----------------------------*/

  /// \brief Forward iterator on (all) the faces of an mln::p_complex<D, G>.
  template <unsigned D, typename G>
  class p_complex_fwd_piter_
    : public internal::p_complex_piter_base_< topo::face_fwd_iter<D>,
					      p_complex<D, G>,
					      mln_site(G),
					      p_complex_fwd_piter_<D, G> >
  {
    typedef p_complex_fwd_piter_<D, G> self_;
    typedef internal::p_complex_piter_base_< topo::face_fwd_iter<D>,
					     p_complex<D, G>,
					     mln_site(G),
					     self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_complex_fwd_piter_();
    p_complex_fwd_piter_(const p_complex<D, G>& pc);
    /// \}
  };


  /*-----------------------------.
  | p_complex_bkd_piter_<D, G>.  |
  `-----------------------------*/

  /// \brief Backward iterator on (all) the faces of an mln::p_complex<D, G>.
  template <unsigned D, typename G>
  class p_complex_bkd_piter_
    : public internal::p_complex_piter_base_< topo::face_bkd_iter<D>,
					      p_complex<D, G>,
					      mln_site(G),
					      p_complex_bkd_piter_<D, G> >
  {
    typedef p_complex_bkd_piter_<D, G> self_;
    typedef internal::p_complex_piter_base_< topo::face_bkd_iter<D>,
					     p_complex<D, G>,
					     mln_site(G),
					     self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_complex_bkd_piter_();
    p_complex_bkd_piter_(const p_complex<D, G>& pc);
    /// \}
  };



# ifndef MLN_INCLUDE_ONLY

  /*-----------------------------.
  | p_complex_fwd_piter_<D, G>.  |
  `-----------------------------*/

  template <unsigned D, typename G>
  inline
  p_complex_fwd_piter_<D, G>::p_complex_fwd_piter_()
  {
  }

  template <unsigned D, typename G>
  inline
  p_complex_fwd_piter_<D, G>::p_complex_fwd_piter_(const p_complex<D, G>& pc)
    : super_(pc)
  {
  }


  /*-----------------------------.
  | p_complex_bkd_piter_<D, G>.  |
  `-----------------------------*/

  template <unsigned D, typename G>
  inline
  p_complex_bkd_piter_<D, G>::p_complex_bkd_piter_()
  {
  }

  template <unsigned D, typename G>
  inline
  p_complex_bkd_piter_<D, G>::p_complex_bkd_piter_(const p_complex<D, G>& pc)
    : super_(pc)
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // ! MLN_CORE_SITE_SET_P_COMPLEX_PITER_HH
