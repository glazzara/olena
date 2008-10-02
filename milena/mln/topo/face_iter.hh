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

#ifndef MLN_TOPO_FACE_ITER_HH
# define MLN_TOPO_FACE_ITER_HH

/// \file mln/topo/face_iter.hh
/// \brief Definition of forward and backward iterators on all the
/// faces of a complex.

# include <mln/topo/internal/complex_set_iterator_base.hh>
# include <mln/topo/face.hh>

// FIXME: Factor a bit more? (Using complex_set_iterator_base.)


namespace mln
{

  namespace topo
  {

    // Forward declaration.
    template <unsigned D> class complex;


    /*-------------------------.
    | topo::face_fwd_iter<D>.  |
    `-------------------------*/

    /// \brief Forward iterator on all the faces of an mln::complex<D>.
    ///
    /// \arg \p D The dimension of the complex this iterator belongs to.
    template <unsigned D>
    class face_fwd_iter
      : public internal::complex_set_iterator_base< face<D>, face_fwd_iter<D> >
    {
    private:
      typedef face_fwd_iter<D> self_;
      typedef internal::complex_set_iterator_base< face<D>, self_ > super_;

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

    /// \brief Backward iterator on all the faces of an mln::complex<D>.
    ///
    /// \arg \p D The dimension of the complex this iterator belongs to.
    template <unsigned D>
    class face_bkd_iter
    : public internal::complex_set_iterator_base< face<D>, face_bkd_iter<D> >
    {
    private:
      typedef face_bkd_iter<D> self_;
      typedef internal::complex_set_iterator_base< face<D>, self_ > super_;

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
      set_cplx(c);
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
	  unsigned n = f_.n();
	  unsigned face_id = f_.face_id();

	  if (face_id + 1 < f_.cplx().nfaces(n))
	    /* FIXME: Provide accessor face::face_id() returning
	       a mutable reference?  This way, we could just write

	       ++f_.face_id();

	       instead of the following.

	       Or add {inc,add}_face_id() services.  */
	    f_.set_face_id(face_id + 1);
	  else
	    // Start to iterate on the faces of the next dimension if
	    // possible.
	    if (n <= D)
	      {
		// FIXME: Same remark as above.
		f_.set_n(n + 1);
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
      set_cplx(c);
      mln_postcondition(!is_valid());
    }

    template <unsigned D>
    inline
    void
    face_bkd_iter<D>::start()
    {
      f_.set_n(D);
      f_.set_face_id(f_.cplx().template nfaces<D>() - 1);
    }

    template <unsigned D>
    inline
    void
    face_bkd_iter<D>::next_()
    {
      if (is_valid())
	{
	  unsigned n = f_.n();
	  unsigned face_id = f_.face_id();

	  if (face_id > 0)
	    /* FIXME: Provide accessor face::face_id() returning
	       a mutable reference?  This way, we could just write

	       ++f_.face_id();

	       instead of the following.

	       Or add {inc,add}_face_id() services.  */
	    f_.set_face_id(face_id - 1);
	  else
	    // Start to iterate on the faces of the previous dimension
	    // if it exists.
	    if (n > 0)
	      {
		// FIXME: Same remark as above.
		f_.set_n(n - 1);
		f_.set_face_id(f_.cplx().nfaces(n - 1) - 1);
	      }
	    else
	      invalidate();
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_FACE_ITER_HH
