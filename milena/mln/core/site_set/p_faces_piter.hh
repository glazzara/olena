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

#ifndef MLN_CORE_SITE_SET_P_FACES_PITER_HH
# define MLN_CORE_SITE_SET_P_FACES_PITER_HH

/// \file mln/core/site_set/p_faces_piter.hh
/// \brief Definition of point iterator on complex-based pset.
///
/// Types defined here are akin to parametrized typedefs: the code is
/// factored in mln::internal::p_complex_piter_base_.

# include <mln/core/site_set/p_faces.hh>
# include <mln/core/internal/p_complex_piter_base.hh>
# include <mln/core/faces_iter.hh>


namespace mln
{

  // Forward declarations.
  template <unsigned N, unsigned D, typename P> class p_faces;

  template <unsigned N, unsigned D> class faces_fwd_iter_;
  template <unsigned N, unsigned D> class faces_bkd_iter_;


  /*------------------------------.
  | p_faces_fwd_piter_<N, D, P>.  |
  `------------------------------*/

  /// \brief Forward iterator on the \p N-faces sites of an
  /// mln::p_faces<N, D, P>.
  template <unsigned N, unsigned D, typename P>
  class p_faces_fwd_piter_
    : public internal::p_complex_piter_base_< faces_fwd_iter_<N, D>,
					      p_faces<N, D, P>,
					      P,
					      p_faces_fwd_piter_<N, D, P> >
  {
    typedef p_faces_fwd_piter_<N, D, P> self_;
    typedef internal::p_complex_piter_base_< faces_fwd_iter_<N, D>,
					     p_faces<N, D, P>,
					     P,
					     self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_faces_fwd_piter_();
    p_faces_fwd_piter_(const p_faces<N, D, P>& pc);
    /// \}
  };


  /*------------------------------.
  | p_faces_bkd_piter_<N, D, P>.  |
  `------------------------------*/

  /// \brief Backward iterator on the \p N-faces sites of an
  /// mln::p_faces<N, D, P>.
  template <unsigned N, unsigned D, typename P>
  class p_faces_bkd_piter_
  /* FIXME: Rename internal::p_complex_piter_base_ to something else,
     as it is also used for p_faces piters! */
    : public internal::p_complex_piter_base_< faces_bkd_iter_<N, D>,
					      p_faces<N, D, P>,
					      P,
					      p_faces_bkd_piter_<N, D, P> >
  {
    typedef p_faces_bkd_piter_<N, D, P> self_;
    typedef internal::p_complex_piter_base_< faces_bkd_iter_<N, D>,
					     p_faces<N, D, P>,
					     P,
					     self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_faces_bkd_piter_();
    p_faces_bkd_piter_(const p_faces<N, D, P>& pc);
    /// \}
  };



# ifndef MLN_INCLUDE_ONLY

  /*------------------------------.
  | p_faces_fwd_piter_<N, D, P>.  |
  `------------------------------*/

  template <unsigned N, unsigned D, typename P>
  inline
  p_faces_fwd_piter_<N, D, P>::p_faces_fwd_piter_()
  {
  }

  template <unsigned N, unsigned D, typename P>
  inline
  p_faces_fwd_piter_<N, D, P>::p_faces_fwd_piter_(const p_faces<N, D, P>& pc)
    : super_(pc)
  {
  }


  /*------------------------------.
  | p_faces_bkd_piter_<N, D, P>.  |
  `------------------------------*/

  template <unsigned N, unsigned D, typename P>
  inline
  p_faces_bkd_piter_<N, D, P>::p_faces_bkd_piter_()
  {
  }

  template <unsigned N, unsigned D, typename P>
  inline
  p_faces_bkd_piter_<N, D, P>::p_faces_bkd_piter_(const p_faces<N, D, P>& pc)
    : super_(pc)
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_SITE_SET_P_FACES_PITER_HH
