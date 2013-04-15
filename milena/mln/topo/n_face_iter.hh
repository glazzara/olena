// Copyright (C) 2008, 2009, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_TOPO_N_FACE_ITER_HH
# define MLN_TOPO_N_FACE_ITER_HH

/// \file
///
/// \brief Definition of forward and backward iterators on all the \a
/// n-faces of a complex, \a n being a dynamic value.

# include <mln/topo/internal/complex_set_iterator_base.hh>
# include <mln/topo/face.hh>

// \todo Factor a bit more? (Using complex_set_iterator_base.)

// FIXME: Rename the old commplex_faces_{fwd,bkd}_iter as
// static_n_face_{fwd,bkd}_iter.


namespace mln
{

  namespace topo
  {

    // Forward declarations.
    template <unsigned D> class complex;

    namespace internal
    {

      // Forward declaration.
      template <typename F, typename E>
      class complex_set_iterator_base;

    } // end of namespace mln::topo::internal


    /*---------------------------.
    | topo::n_face_fwd_iter<D>.  |
    `---------------------------*/

    /*!
      \internal
      \brief Forward iterator on all the faces of an
      mln::complex<D>.

      \tparam D The dimension of the complex this iterator belongs to.
    */
    template <unsigned D>
    class n_face_fwd_iter
      : public internal::complex_set_iterator_base< topo::face<D>,
						    n_face_fwd_iter<D> >
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef n_face_fwd_iter<D> self_;
      typedef internal::complex_set_iterator_base< topo::face<D>, self_ > super_;

    public:
      using super_::is_valid;
      using super_::invalidate;

    public:
      /// Construction and assignment.
      /// \{
      n_face_fwd_iter();
      // FIXME: See comment in internal::complex_set_iterator_base's
      // default ctor.
      n_face_fwd_iter(complex<D>& c, unsigned n);
      /// \}

      /// Manipulation.
      /// \{
      /// Test if the iterator is valid.
      void start();
      /// Go to the next point.
      void next_();
      /// \}

      /// Accessors.
      ///
      /// Shortcuts to face_'s accessors.
      /// \{
      unsigned n() const;
      void set_n (unsigned n);
      /// \}

    private:
      /// Partially invalidate the iterator (don't alter its dimension).
      void invalidate_face_id_();

    private:
      using super_::f_;
    };


    /*---------------------------.
    | topo::n_face_bkd_iter<D>.  |
    `---------------------------*/

    /*!
      \internal
      \brief Backward iterator on all the faces of an mln::complex<D>.

      \tparam D The dimension of the complex this iterator belongs to.
    */
    template <unsigned D>
    class n_face_bkd_iter
      : public internal::complex_set_iterator_base< topo::face<D>,
						    n_face_bkd_iter<D> >
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef n_face_bkd_iter<D> self_;
      typedef internal::complex_set_iterator_base< topo::face<D>, self_ > super_;

    public:
      using super_::is_valid;
      using super_::invalidate;

    public:
      /// Construction and assignment.
      /// \{
      n_face_bkd_iter();
      // FIXME: See comment in internal::complex_set_iterator_base's
      // default ctor.
      n_face_bkd_iter(complex<D>& c, unsigned n);
      /// \}

      /// Manipulation.
      /// \{
      /// Start an iteration.
      void start();
      /// Go to the next point.
      void next_();
      /// \}

      /// Accessors.
      ///
      /// Shortcuts to face_'s accessors.
      /// \{
      unsigned n() const;
      void set_n (unsigned n);
      /// \}

    private:
      /// Partially invalidate the iterator (don't alter its dimension).
      void invalidate_face_id_();

    private:
      using super_::f_;
    };



# ifndef MLN_INCLUDE_ONLY

    /*---------------------------.
    | topo::n_face_fwd_iter<D>.  |
    `---------------------------*/

    template <unsigned D>
    inline
    n_face_fwd_iter<D>::n_face_fwd_iter()
      : super_()
    {
    }

    template <unsigned D>
    inline
    n_face_fwd_iter<D>::n_face_fwd_iter(complex<D>& c, unsigned n)
      : super_(c)
    {
      mln_precondition(n <= D);
      this->set_cplx(c);
      set_n(n);
      mln_postcondition(!is_valid());
    }

    template <unsigned D>
    inline
    void
    n_face_fwd_iter<D>::start()
    {
      f_.set_face_id(0u);
    }

    template <unsigned D>
    inline
    void
    n_face_fwd_iter<D>::next_()
    {
      if (is_valid())
	{
	  if (f_.face_id() + 1 < f_.cplx().nfaces_of_dim(n()))
	    f_.inc_face_id();
	  else
	    /* Don't invalidate the whole face if we have reached the
	       last face of the dimension---this would lose the
	       dimension.  Instead, invalidate the face_id only.  */
	    invalidate_face_id_();
	}
    }

    template <unsigned D>
    inline
    void
    n_face_fwd_iter<D>::invalidate_face_id_()
    {
      f_.set_face_id(f_.cplx().nfaces_of_dim(n()));
    }

    template <unsigned D>
    inline
    unsigned
    n_face_fwd_iter<D>::n() const
    {
      return f_.n();
    }

    template <unsigned D>
    inline
    void
    n_face_fwd_iter<D>::set_n(unsigned n)
    {
      mln_precondition(n <= D);
      f_.set_n(n);
    }


    /*---------------------------.
    | topo::n_face_bkd_iter<D>.  |
    `---------------------------*/

    template <unsigned D>
    inline
    n_face_bkd_iter<D>::n_face_bkd_iter()
      : super_()
    {
    }

    template <unsigned D>
    inline
    n_face_bkd_iter<D>::n_face_bkd_iter(complex<D>& c, unsigned n)
      : super_(c)
    {
      mln_precondition(n <= D);
      set_cplx(c);
      set_n(n);
      mln_postcondition(!is_valid());
    }

    template <unsigned D>
    inline
    void
    n_face_bkd_iter<D>::start()
    {
      f_.set_face_id(f_.cplx().nfaces_of_dim(n()) - 1);
    }

    template <unsigned D>
    inline
    void
    n_face_bkd_iter<D>::next_()
    {
      if (is_valid())
	{
	  if (f_.face_id() > 0)
	    f_.dec_face_id();
	  else
	    /* Don't invalidate the whole face if we have reached the
	       last face of the dimension---this would lose the
	       dimension.  Instead, invalidate the face_id only.  */
	    invalidate_face_id_();
	}
    }

    template <unsigned D>
    inline
    void
    n_face_bkd_iter<D>::invalidate_face_id_()
    {
      f_.set_face_id(f_.cplx().nfaces_of_dim(n()));
    }

    template <unsigned D>
    inline
    unsigned
    n_face_bkd_iter<D>::n() const
    {
      return f_.n();
    }

    template <unsigned D>
    inline
    void
    n_face_bkd_iter<D>::set_n(unsigned n)
    {
      mln_precondition(n <= D);
      f_.set_n(n);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_N_FACE_ITER_HH
