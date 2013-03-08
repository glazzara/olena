// Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_N_FACES_PITER_HH
# define MLN_CORE_SITE_SET_P_N_FACES_PITER_HH

/// \file
/// \brief Definition of point iterator on the n-faces of a
/// complex-based pset.

# include <mln/core/site_set/p_complex.hh>
# include <mln/core/internal/p_complex_piter_base.hh>
# include <mln/topo/n_face_iter.hh>

/* FIXME: Maybe we could factor these iterators again, by introducing
   an implementation class having methods n() and set_n().  */


namespace mln
{

  // Forward declarations.
  template <unsigned D, typename G> class p_complex;


  /*----------------------------.
  | p_n_faces_fwd_piter<D, G>.  |
  `----------------------------*/

  /*!
    \internal
    \brief Forward iterator on the n-faces sites of an
    mln::p_complex<D, G>.
  */
  template <unsigned D, typename G>
  class p_n_faces_fwd_piter
    : public internal::p_complex_piter_base_< topo::n_face_fwd_iter<D>,
					      p_complex<D, G>,
					      mln_site(G),
					      p_n_faces_fwd_piter<D, G> >
  {
    typedef p_n_faces_fwd_piter<D, G> self_;
    typedef internal::p_complex_piter_base_< topo::n_face_fwd_iter<D>,
					     p_complex<D, G>,
					     mln_site(G),
					     self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_n_faces_fwd_piter();
    p_n_faces_fwd_piter(const p_complex<D, G>& pc, unsigned n);
    /// \}

    /// \brief Accessors.
    ///
    /// Shortcuts to face_'s accessors.
    /// \{
    unsigned n() const;
    void set_n (unsigned n);
    /// \}
  };


  /*----------------------------.
  | p_n_faces_bkd_piter<D, G>.  |
  `----------------------------*/

  /*!
    \internal
    \brief Backward iterator on the n-faces sites of an
    mln::p_complex<D, G>.
  */
  template <unsigned D, typename G>
  class p_n_faces_bkd_piter
  /* FIXME: Rename internal::p_complex_piter_base_ to something else,
     as it is also used for p_faces piters! */
    : public internal::p_complex_piter_base_< topo::n_face_bkd_iter<D>,
					      p_complex<D, G>,
					      mln_site(G),
					      p_n_faces_bkd_piter<D, G> >
  {
    typedef p_n_faces_bkd_piter<D, G> self_;
    typedef internal::p_complex_piter_base_< topo::n_face_bkd_iter<D>,
					     p_complex<D, G>,
					     mln_site(G),
					     self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_n_faces_bkd_piter();
    p_n_faces_bkd_piter(const p_complex<D, G>& pc, unsigned n);
    /// \}

    /// \brief Accessors.
    ///
    /// Shortcuts to face_'s accessors.
    /// \{
    unsigned n() const;
    void set_n (unsigned n);
    /// \}
  };



# ifndef MLN_INCLUDE_ONLY

  /*----------------------------.
  | p_n_faces_fwd_piter<D, G>.  |
  `----------------------------*/

  template <unsigned D, typename G>
  inline
  p_n_faces_fwd_piter<D, G>::p_n_faces_fwd_piter()
  {
  }

  template <unsigned D, typename G>
  inline
  p_n_faces_fwd_piter<D, G>::p_n_faces_fwd_piter(const p_complex<D, G>& pc,
						 unsigned n)
    : super_(pc)
  {
    mln_precondition(n <= D);
    set_n(n);
  }

  template <unsigned D, typename G>
  inline
  unsigned
  p_n_faces_fwd_piter<D, G>::n() const
  {
    return this->iter_.n();
  }

  template <unsigned D, typename G>
  inline
  void
  p_n_faces_fwd_piter<D, G>::set_n (unsigned n)
  {
    this->iter_.set_n(n);
  }


  /*----------------------------.
  | p_n_faces_bkd_piter<D, G>.  |
  `----------------------------*/

  template <unsigned D, typename G>
  inline
  p_n_faces_bkd_piter<D, G>::p_n_faces_bkd_piter()
  {
  }

  template <unsigned D, typename G>
  inline
  p_n_faces_bkd_piter<D, G>::p_n_faces_bkd_piter(const p_complex<D, G>& pc,
						 unsigned n)
    : super_(pc)
  {
    mln_precondition(n <= D);
    set_n(n);
  }

  template <unsigned D, typename G>
  inline
  unsigned
  p_n_faces_bkd_piter<D, G>::n() const
  {
    return this->iter_.n();
  }

  template <unsigned D, typename G>
  inline
  void
  p_n_faces_bkd_piter<D, G>::set_n (unsigned n)
  {
    this->iter_.set_n(n);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // ! MLN_CORE_SITE_SET_P_N_FACES_PITER_HH
