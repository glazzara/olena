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

#ifndef MLN_TOPO_ADJ_HIGHER_FACE_ITER_HH
# define MLN_TOPO_ADJ_HIGHER_FACE_ITER_HH

/// \file
///
/// \brief Definition of forward and backward iterators on the
/// adjacent (n+1)-faces of a (reference) n-face in a complex.

# include <mln/topo/internal/complex_relative_iterator_base.hh>
# include <mln/topo/algebraic_face.hh>


namespace mln
{

  namespace topo
  {

    // Forward declaration.
    template <unsigned D> class complex;

    namespace internal
    {

      template <typename C, typename F, typename E>
      class forward_complex_relative_iterator_base;

      template <typename C, typename F, typename E>
      class backward_complex_relative_iterator_base;

    } // end of mln::topo::internal

    template <unsigned D>
    class algebraic_face;

    /*------------------------------------.
    | topo::adj_higher_face_fwd_iter<D>.  |
    `------------------------------------*/

    /*!
      \internal
      \brief Forward iterator on all the adjacent (n+1)-faces of the
      n-face of an mln::complex<D>.

      \tparam D The dimension of the complex this iterator belongs to.
    */
    template <unsigned D>
    class adj_higher_face_fwd_iter
      : public internal::forward_complex_relative_iterator_base< topo::face<D>,
								 algebraic_face<D>,
								 adj_higher_face_fwd_iter<D> >
    {
      typedef adj_higher_face_fwd_iter<D> self_;
      typedef internal::forward_complex_relative_iterator_base< topo::face<D>,
								algebraic_face<D>,
								self_ > super_;
      // Tech note: we use topo::face to help g++-2.95.

    public:
      /// Construction.
      /// \{
      adj_higher_face_fwd_iter();
      template <typename Fref>
      adj_higher_face_fwd_iter(const Fref& f_ref);
      /// \}

      /// Compute the set of faces adjacent to the reference face.
      void update_adj_faces_();
    };


    /*------------------------------------.
    | topo::adj_higher_face_bkd_iter<D>.  |
    `------------------------------------*/

    /*!
      \internal
      \brief Backward iterator on all the adjacent (n+1)-faces of
      the n-face of an mln::complex<D>.

      \tparam D The dimension of the complex this iterator belongs to.
    */
    template <unsigned D>
    class adj_higher_face_bkd_iter
      : public internal::backward_complex_relative_iterator_base< topo::face<D>,
								  algebraic_face<D>,
								  adj_higher_face_bkd_iter<D> >
    {
      typedef adj_higher_face_bkd_iter<D> self_;
      typedef internal::backward_complex_relative_iterator_base< topo::face<D>,
								 algebraic_face<D>,
								 self_ > super_;
      // Tech note: we use topo::face to help g++-2.95.

    public:
      /// Construction.
      /// \{
      adj_higher_face_bkd_iter();
      template <typename Fref>
      adj_higher_face_bkd_iter(const Fref& f_ref);
      /// \}

      /// Compute the set of faces adjacent to the reference face.
      void update_adj_faces_();
    };



# ifndef MLN_INCLUDE_ONLY

    /*------------------------------------.
    | topo::adj_higher_face_fwd_iter<D>.  |
    `------------------------------------*/

    template <unsigned D>
    inline
    adj_higher_face_fwd_iter<D>::adj_higher_face_fwd_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    adj_higher_face_fwd_iter<D>::adj_higher_face_fwd_iter(const Fref& f_ref)
      : super_(f_ref)
    {
    }

    template <unsigned D>
    inline
    void
    adj_higher_face_fwd_iter<D>::update_adj_faces_()
    {
      mln_precondition(this->c_);
      this->adj_faces_ = this->c_->higher_dim_adj_faces();
    }


    /*------------------------------------.
    | topo::adj_higher_face_bkd_iter<D>.  |
    `------------------------------------*/

    template <unsigned D>
    inline
    adj_higher_face_bkd_iter<D>::adj_higher_face_bkd_iter()
    {
    }

    template <unsigned D>
    template <typename Fref>
    inline
    adj_higher_face_bkd_iter<D>::adj_higher_face_bkd_iter(const Fref& f_ref)
      : super_(f_ref)
    {
    }

    template <unsigned D>
    inline
    void
    adj_higher_face_bkd_iter<D>::update_adj_faces_()
    {
      mln_precondition(this->c_);
      this->adj_faces_ = this->c_->higher_dim_adj_faces();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_ADJ_HIGHER_FACE_ITER_HH
