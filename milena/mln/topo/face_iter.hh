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

#ifndef MLN_TOPO_FACE_ITER_HH
# define MLN_TOPO_FACE_ITER_HH

/// \file
///
/// \brief Definition of forward and backward iterators on all the
/// faces of a complex.

# include <mln/topo/internal/complex_set_iterator_base.hh>
# include <mln/topo/face.hh>
# include <mln/topo/face_iter.hh>

// FIXME: Factor a bit more? (Using complex_set_iterator_base.)


namespace mln
{

  namespace topo
  {

    // Forward declarations.
    template <unsigned D> class complex;

    namespace internal
    {

      template <typename F, typename E>
      class complex_set_iterator_base;

    } // end of namespace mln::topo::internal


    /*-------------------------.
    | topo::face_fwd_iter<D>.  |
    `-------------------------*/

    /*!
      \internal
      \brief Forward iterator on all the faces of an
      mln::complex<D>.

      \tparam D The dimension of the complex this iterator belongs to.
    */
    template <unsigned D>
    class face_fwd_iter
      : public internal::complex_set_iterator_base< topo::face<D>, face_fwd_iter<D> >
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef face_fwd_iter<D> self_;
      typedef internal::complex_set_iterator_base< topo::face<D>, self_ > super_;

    public:
      using super_::is_valid;
      using super_::invalidate;

    public:
      /// Construction and assignment.
      /// \{
      face_fwd_iter();
      // FIXME: See comment in internal::complex_set_iterator_base's
      // default ctor.
      face_fwd_iter(complex<D>& c);
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


    /*-------------------------.
    | topo::face_bkd_iter<D>.  |
    `-------------------------*/

    /*!
      \internal
      \brief Backward iterator on all the faces of an mln::complex<D>.

      \tparam D The dimension of the complex this iterator belongs to.
    */
    template <unsigned D>
    class face_bkd_iter
    : public internal::complex_set_iterator_base< topo::face<D>, face_bkd_iter<D> >
    {
      // Tech note: we use topo::face to help g++-2.95.
    private:
      typedef face_bkd_iter<D> self_;
      typedef internal::complex_set_iterator_base< topo::face<D>, self_ > super_;

    public:
      using super_::is_valid;
      using super_::invalidate;

    public:
      /// Construction and assignment.
      /// \{
      face_bkd_iter();
      // FIXME: See comment in internal::complex_set_iterator_base's
      // default ctor.
      face_bkd_iter(complex<D>& c);
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

    /*-------------------------.
    | topo::face_fwd_iter<D>.  |
    `-------------------------*/

    template <unsigned D>
    inline
    face_fwd_iter<D>::face_fwd_iter()
      : super_()
    {
    }

    template <unsigned D>
    inline
    face_fwd_iter<D>::face_fwd_iter(complex<D>& c)
      : super_(c)
    {
      this->set_cplx(c);
      mln_postcondition(!is_valid());
    }

    template <unsigned D>
    inline
    void
    face_fwd_iter<D>::start()
    {
      f_.set_n(0u);
      f_.set_face_id(0u);
    }

    template <unsigned D>
    inline
    void
    face_fwd_iter<D>::next_()
    {
      if (is_valid())
	{
	  if (f_.face_id() + 1 < f_.cplx().nfaces_of_dim(f_.n()))
	    f_.inc_face_id();
	  else
	    // Start to iterate on the faces of the next dimension if
	    // possible.
	    if (f_.n() <= D)
	      {
		f_.inc_n();
		f_.set_face_id(0u);
	      }
	    else
	      invalidate();
	}
    }


    /*-------------------------.
    | topo::face_bkd_iter<D>.  |
    `-------------------------*/

    template <unsigned D>
    inline
    face_bkd_iter<D>::face_bkd_iter()
      : super_()
    {
    }

    template <unsigned D>
    inline
    face_bkd_iter<D>::face_bkd_iter(complex<D>& c)
      : super_(c)
    {
      this->set_cplx(c);
      mln_postcondition(!is_valid());
    }

    template <unsigned D>
    inline
    void
    face_bkd_iter<D>::start()
    {
      f_.set_n(D);
      f_.set_face_id(f_.cplx().template nfaces_of_static_dim<D>() - 1);
    }

    template <unsigned D>
    inline
    void
    face_bkd_iter<D>::next_()
    {
      if (is_valid())
	{
	  if (f_.face_id() > 0)
	    f_.dec_face_id();
	  else
	    // Start to iterate on the faces of the previous dimension
	    // if it exists.
	    if (f_.n() > 0)
	      {
		f_.dec_n();
		f_.set_face_id(f_.cplx().nfaces_of_dim(f_.n()) - 1);
	      }
	    else
	      invalidate();
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_FACE_ITER_HH
