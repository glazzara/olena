// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_ATTIC_P_COMPLEX_FACES_PITER_HH
# define MLN_CORE_SITE_SET_ATTIC_P_COMPLEX_FACES_PITER_HH

/// \file
/// \brief Definition of point iterator on the n-faces of a
/// complex-based pset.
///
/// Types defined here are akin to parametrized typedefs: the code is
/// factored in mln::internal::p_complex_piter_base_.

# include <mln/core/site_set/p_complex.hh>
# include <mln/core/internal/p_complex_piter_base.hh>
# include <mln/topo/attic/faces_iter.hh>

/* FIXME: p_complex_faces_fwd_piter_ and p_complex_faces_bkd_piter_
   are really long names: find something shorter.  */


namespace mln
{

  // Forward declarations.
  template <unsigned N, unsigned D, typename P> class p_faces;

  template <unsigned N, unsigned D> class faces_fwd_iter_;
  template <unsigned N, unsigned D> class faces_bkd_iter_;


  /*--------------------------------------.
  | p_complex_faces_fwd_piter_<N, D, P>.  |
  `--------------------------------------*/

  /*!
    \internal
    \brief Forward iterator on the \p N-faces sites of an
    mln::p_complex<D, P>.
  */
  template <unsigned N, unsigned D, typename P>
  class p_complex_faces_fwd_piter_
    : public internal::p_complex_piter_base_< topo::faces_fwd_iter_<N, D>,
					      p_complex<D, P>,
					      P,
					      p_complex_faces_fwd_piter_<N, D, P> >
  {
    typedef p_complex_faces_fwd_piter_<N, D, P> self_;
    typedef internal::p_complex_piter_base_< topo::faces_fwd_iter_<N, D>,
					     p_complex<D, P>,
					     P,
					     self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_complex_faces_fwd_piter_();
    p_complex_faces_fwd_piter_(const p_complex<D, P>& pc);
    /// \}
  };


  /*--------------------------------------.
  | p_complex_faces_bkd_piter_<N, D, P>.  |
  `--------------------------------------*/

  /*!
    \internal
    \brief Backward iterator on the \p N-faces sites of an
    mln::p_complex<D, P>.
  */
  template <unsigned N, unsigned D, typename P>
  class p_complex_faces_bkd_piter_
  /* FIXME: Rename internal::p_complex_piter_base_ to something else,
     as it is also used for p_faces piters! */
    : public internal::p_complex_piter_base_< topo::faces_bkd_iter_<N, D>,
					      p_complex<D, P>,
					      P,
					      p_complex_faces_bkd_piter_<N, D, P> >
  {
    typedef p_complex_faces_bkd_piter_<N, D, P> self_;
    typedef internal::p_complex_piter_base_< topo::faces_bkd_iter_<N, D>,
					     p_complex<D, P>,
					     P,
					     self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_complex_faces_bkd_piter_();
    p_complex_faces_bkd_piter_(const p_complex<D, P>& pc);
    /// \}
  };



# ifndef MLN_INCLUDE_ONLY

  /*--------------------------------------.
  | p_complex_faces_fwd_piter_<N, D, P>.  |
  `--------------------------------------*/

  template <unsigned N, unsigned D, typename P>
  inline
  p_complex_faces_fwd_piter_<N, D, P>::p_complex_faces_fwd_piter_()
  {
  }

  template <unsigned N, unsigned D, typename P>
  inline
  p_complex_faces_fwd_piter_<N, D, P>::p_complex_faces_fwd_piter_(const p_complex<D, P>& pc)
    : super_(pc)
  {
  }


  /*--------------------------------------.
  | p_complex_faces_bkd_piter_<N, D, P>.  |
  `--------------------------------------*/

  template <unsigned N, unsigned D, typename P>
  inline
  p_complex_faces_bkd_piter_<N, D, P>::p_complex_faces_bkd_piter_()
  {
  }

  template <unsigned N, unsigned D, typename P>
  inline
  p_complex_faces_bkd_piter_<N, D, P>::p_complex_faces_bkd_piter_(const p_complex<D, P>& pc)
    : super_(pc)
  {
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // ! MLN_CORE_SITE_SET_ATTIC_P_COMPLEX_FACES_PITER_HH
