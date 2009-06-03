// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_TOPO_STATIC_N_FACE_ITER_HH
# define MLN_TOPO_STATIC_N_FACE_ITER_HH

/// \file mln/topo/static_n_face_iter.hh
///
/// \brief Definition of forward and backward iterators on all the
/// \p N-faces of a complex, \p N being a static value.

# include <mln/topo/internal/complex_set_iterator_base.hh>
# include <mln/topo/face.hh>


namespace mln
{

  namespace topo
  {

    /*-------------------------------------.
    | topo::static_n_face_fwd_iter<N, D>.  |
    `-------------------------------------*/

    /// \brief Forward iterator on all the \p N-faces of a
    /// mln::complex<D>.
    ///
    /// \arg \p N The dimension of the face associated to this iterator.
    /// \arg \p D The dimension of the complex this iterator belongs to.
    //
    template <unsigned N, unsigned D>
    class static_n_face_fwd_iter
      : public internal::complex_set_iterator_base< topo::face<D>,
						    static_n_face_fwd_iter<N, D> >
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef static_n_face_fwd_iter<N, D> self_;
      typedef internal::complex_set_iterator_base< topo::face<D>, self_ > super_;

    public:
      using super_::is_valid;
      using super_::invalidate;

    public:
      /// Construction and assignment.
      /// \{
      static_n_face_fwd_iter();
      // FIXME: See comment in internal::complex_set_iterator_base's
      // default ctor.
      static_n_face_fwd_iter(complex<D>& c);
      /// \}

      /// Manipulation.
      /// \{
      /// Test if the iterator is valid.
      void start();
      /// Go to the next point.
      void next_();
      /// \}

    private:
      using super_::f_;
    };


    /*-------------------------------------.
    | topo::static_n_face_bkd_iter<N, D>.  |
    `-------------------------------------*/

    /// \brief Backward iterator on all the \p N-faces of a
    /// mln::complex<D>.
    ///
    /// \arg \p N The dimension of the face associated to this iterator.
    /// \arg \p D The dimension of the complex this iterator belongs to.
    //
    template <unsigned N, unsigned D>
    class static_n_face_bkd_iter
      : public internal::complex_set_iterator_base< topo::face<D>,
						    static_n_face_bkd_iter<N, D> >
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef static_n_face_bkd_iter<N, D> self_;
      typedef internal::complex_set_iterator_base< topo::face<D>, self_ > super_;

    public:
      using super_::is_valid;
      using super_::invalidate;

    public:
      /// Construction and assignment.
      /// \{
      static_n_face_bkd_iter();
      // FIXME: See comment in internal::complex_set_iterator_base's
      // default ctor.
      static_n_face_bkd_iter(complex<D>& c);
      /// \}

      /// Manipulation.
      /// \{
      /// Start an iteration.
      void start();
      /// Go to the next point.
      void next_();
      /// \}

    private:
      using super_::f_;
    };



# ifndef MLN_INCLUDE_ONLY

    /*-------------------------------------.
    | topo::static_n_face_fwd_iter<N, D>.  |
    `-------------------------------------*/

    template <unsigned N, unsigned D>
    inline
    static_n_face_fwd_iter<N, D>::static_n_face_fwd_iter()
      : super_()
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
      f_.set_n(N);
      mln_postcondition(!is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    static_n_face_fwd_iter<N, D>::static_n_face_fwd_iter(complex<D>& c)
      : super_(c)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
      f_.set_n(N);
      mln_postcondition(!is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    void
    static_n_face_fwd_iter<N, D>::start()
    {
      f_.set_face_id(0u);
    }

    template <unsigned N, unsigned D>
    inline
    void
    static_n_face_fwd_iter<N, D>::next_()
    {
      if (is_valid())
	{
	  if (f_.face_id() + 1 < f_.cplx().template nfaces_with_dim<N>())
	    f_.inc_face_id();
	  else
	    invalidate();
	}
    }


    /*-------------------------------------.
    | topo::static_n_face_bkd_iter<N, D>.  |
    `-------------------------------------*/

    template <unsigned N, unsigned D>
    inline
    static_n_face_bkd_iter<N, D>::static_n_face_bkd_iter()
      : super_()
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
      f_.set_n(N);
      mln_postcondition(!is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    static_n_face_bkd_iter<N, D>::static_n_face_bkd_iter(complex<D>& c)
      : super_(c)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
      f_.set_n(N);
      mln_postcondition(!is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    void
    static_n_face_bkd_iter<N, D>::start()
    {
      f_.set_face_id(f_.cplx().template nfaces_with_dim<N>() - 1);
    }

    template <unsigned N, unsigned D>
    inline
    void
    static_n_face_bkd_iter<N, D>::next_()
    {
      if (is_valid())
	{
	  if (f_.face_id() > 0)
	    f_.dec_face_id();
	  else
	    invalidate();
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_STATIC_N_FACE_ITER_HH
