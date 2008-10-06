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

#ifndef MLN_TOPO_N_FACE_ITER_HH
# define MLN_TOPO_N_FACE_ITER_HH

/// \file mln/topo/n_face_iter.hh
/// \brief Definition of forward and backward iterators on all the
/// \a n-faces of a complex, \a n being a dynamic value.

# include <mln/topo/internal/complex_set_iterator_base.hh>
# include <mln/topo/face.hh>

// FIXME: Factor a bit more? (Using complex_set_iterator_base.)

// FIXME: Rename the old commplex_faces_{fwd,bkd}_iter as
// static_n_face_{fwd,bkd}_iter.


namespace mln
{

  namespace topo
  {

    // Forward declaration.
    template <unsigned D> class complex;


    /*---------------------------.
    | topo::n_face_fwd_iter<D>.  |
    `---------------------------*/

    /// \brief Forward iterator on all the faces of an mln::complex<D>.
    ///
    /// \arg \p D The dimension of the complex this iterator belongs to.
    template <unsigned D>
    class n_face_fwd_iter
      : public internal::complex_set_iterator_base< face<D>,
						    n_face_fwd_iter<D> >
    {
    private:
      typedef n_face_fwd_iter<D> self_;
      typedef internal::complex_set_iterator_base< face<D>, self_ > super_;

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

      /// \brief Accessors.
      ///
      /// Shortcuts to face_'s accessors.
      /// \{
      unsigned n() const;
      void set_n (unsigned n);
      /// \}

    private:
      using super_::f_;
    };


    /*---------------------------.
    | topo::n_face_bkd_iter<D>.  |
    `---------------------------*/

    /// \brief Backward iterator on all the faces of an mln::complex<D>.
    ///
    /// \arg \p D The dimension of the complex this iterator belongs to.
    template <unsigned D>
    class n_face_bkd_iter
      : public internal::complex_set_iterator_base< face<D>,
						    n_face_bkd_iter<D> >
    {
    private:
      typedef n_face_bkd_iter<D> self_;
      typedef internal::complex_set_iterator_base< face<D>, self_ > super_;

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

      /// \brief Accessors.
      ///
      /// Shortcuts to face_'s accessors.
      /// \{
      unsigned n() const;
      void set_n (unsigned n);
      /// \}

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
      set_cplx(c);
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
	  if (f_.face_id() + 1 < f_.cplx().nfaces(n()))
	    f_.inc_face_id();
	  else
	    invalidate();
	}
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
      f_.set_n(n());
      f_.set_face_id(f_.cplx().nfaces(n()) - 1);
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
	    invalidate();
	}
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
