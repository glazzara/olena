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

#ifndef MLN_TOPO_STATIC_N_FACE_ITER_HH
# define MLN_TOPO_STATIC_N_FACE_ITER_HH

/// \file mln/topo/static_n_face_iter.hh
/// \brief Definition of forward and backward iterators on all the
/// \p N-faces of a complex, \p N being a static value.

# include <mln/topo/internal/complex_iter_base.hh>
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
    template <unsigned N, unsigned D>
    class static_n_face_fwd_iter
      : public internal::complex_iter_base< face<D>,
					    static_n_face_fwd_iter<N, D> >
    {
    private:
      typedef static_n_face_fwd_iter<N, D> self_;
      typedef internal::complex_iter_base< face<D>, self_ > super_;

    public:
      using super_::is_valid;
      using super_::invalidate;

    public:
      /// Construction and assignment.
      /// \{
      static_n_face_fwd_iter();
      // FIXME: See comment in internal::complex_iter_base's default ctor
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
      using super_::face_;
    };


    /*-------------------------------------.
    | topo::static_n_face_bkd_iter<N, D>.  |
    `-------------------------------------*/

    /// \brief Backward iterator on all the \p N-faces of a
    /// mln::complex<D>.
    ///
    /// \arg \p N The dimension of the face associated to this iterator.
    /// \arg \p D The dimension of the complex this iterator belongs to.
    template <unsigned N, unsigned D>
    class static_n_face_bkd_iter
      : public internal::complex_iter_base< face<D>,
					    static_n_face_bkd_iter<N, D> >
    {
    private:
      typedef static_n_face_bkd_iter<N, D> self_;
      typedef internal::complex_iter_base< face<D>, self_ > super_;

    public:
      using super_::is_valid;
      using super_::invalidate;

    public:
      /// Construction and assignment.
      /// \{
      static_n_face_bkd_iter();
      // FIXME: See comment in internal::complex_iter_base's default ctor
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
      using super_::face_;
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
      face_.set_n(N);
      mln_postcondition(!is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    static_n_face_fwd_iter<N, D>::static_n_face_fwd_iter(complex<D>& c)
      : super_(c)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
      face_.set_n(N);
      mln_postcondition(!is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    void
    static_n_face_fwd_iter<N, D>::start()
    {
      face_.set_face_id(0u);
    }

    template <unsigned N, unsigned D>
    inline
    void
    static_n_face_fwd_iter<N, D>::next_()
    {
      if (is_valid())
	{
	  unsigned face_id = face_.face_id();
	  // The number of faces of dimension N in cplx_.
	  unsigned nn_faces = face_.cplx().template nfaces<N>();
	  if (face_id + 1 < nn_faces)
	    /* FIXME: Provide accessor face::face_id()
	       returning a mutable reference?  This way, we could just
	       write

	       ++face_.face_id();

	       instead of the following.

	       Or add {inc,add}_face_id() services.  */
	    face_.set_face_id(face_id + 1);
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
      face_.set_n(N);
      mln_postcondition(!is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    static_n_face_bkd_iter<N, D>::static_n_face_bkd_iter(complex<D>& c)
      : super_(c)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
      face_.set_n(N);
      mln_postcondition(!is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    void
    static_n_face_bkd_iter<N, D>::start()
    {
      face_.set_face_id(face_.cplx().template nfaces<N>() - 1);
    }

    template <unsigned N, unsigned D>
    inline
    void
    static_n_face_bkd_iter<N, D>::next_()
    {
      if (is_valid())
	{
	  unsigned face_id = face_.face_id();
	  if (face_id > 0)
	    /* FIXME: Provide accessor face::face_id()
	       returning a mutable reference?  This way, we could just
	       write

	       ++face_.face_id();

	       instead of the following.

	       Or add {inc,add}_face_id() services.  */
	    face_.set_face_id(face_id - 1);
	  else
	    invalidate();
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_STATIC_N_FACE_ITER_HH
