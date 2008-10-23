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

#ifndef MLN_TOPO_ALGEBRAIC_FACE_HH
# define MLN_TOPO_ALGEBRAIC_FACE_HH

/// \file mln/topo/algebraic_face.hh
/// \brief Algebraic face of a complex.

#include <mln/topo/face.hh>


namespace mln
{

  namespace topo
  {

    // Forward declarations.
    template <unsigned D> class complex;
    template <unsigned N, unsigned D> class n_face;
    template <unsigned N, unsigned D> class face_data;
    

    /*-------.
    | Face.  |
    `-------*/

    /// \brief Algebraic face handle in a complex; the face dimension
    /// is dynamic.
    ///
    /// Contrary to an mln::topo::algebraic_n_face, the dimension of an
    /// mln::topo::algebraic_face is not fixed.
    template <unsigned D>
    struct algebraic_face : public face<D>
    {
      typedef face<D> super_;

    public:
      // The type of the complex this handle points to.
      typedef complex<D> complex_type;

      /// Build a non-initialized algebraic face handle.
      algebraic_face();
      /// Build an algebraic face handle from \a complex and \a face_id.
      algebraic_face(complex<D>& complex, unsigned n, unsigned face_id,
		     bool sign);
      /// Build an algebraic face handle from an mln::face.
      algebraic_face(const face<D>& f, bool sign);

      /// Build a face handle from an mln::topo::algebraic_n_face.
      template <unsigned N>
      algebraic_face(const algebraic_n_face<N, D>& f);

      /// Accessors.
      /// \{
      /// Return the sign of this face.
      bool sign() const;
      /// Set the sign of this face.
      void set_sign(bool sign);
      /// \}

    private:
      /// The sign of this algebraic face.
      bool sign_;
    };


    /// Create an algebraic face handle of a \p D-complex.
    template <unsigned D>
    algebraic_face<D>
    make_algebraic_face(const face<D>& f, bool sign);


    /// Inversion operators.
    /// \{
    template <unsigned D>
    algebraic_face<D>
    operator-(const face<D>& f);

    template <unsigned D>
    algebraic_face<D>
    operator-(const algebraic_face<D>& f);
    /// \}


    /// Comparison of two instances of mln::topo::algebraic_face.
    /// \{

    /// \brief Is \a lhs equal to \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned D>
    bool operator==(const algebraic_face<D>& lhs,
		    const algebraic_face<D>& rhs);

    /// \brief Is \a lhs different from \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned D>
    bool operator!=(const algebraic_face<D>& lhs,
		    const algebraic_face<D>& rhs);

    /// \brief Is \a lhs ``less'' than \a rhs?
    ///
    /// This comparison is required by algorithms sorting algebraic
    /// face handles.
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    /// \pre Arguments \a lhs and \a rhs must have the same dimension.
    template <unsigned D>
    bool operator< (const algebraic_face<D>& lhs,
		    const algebraic_face<D>& rhs);

    /// \}


    /// Print an mln::topo::algebraic_face.
    template <unsigned D>
    std::ostream&
    operator<<(std::ostream& ostr, const algebraic_face<D>& f);



# ifndef MLN_INCLUDE_ONLY

    template <unsigned D>
    inline
    algebraic_face<D>::algebraic_face()
      : super_(), sign_(true)
    {
    }

    template <unsigned D>
    inline
    algebraic_face<D>::algebraic_face(complex<D>& c, unsigned n,
				      unsigned face_id, bool sign)
      : super_(c, n, face_id), sign_(sign)
    {
      // Ensure N is compatible with D.
      mln_precondition(n <= D);
    }

    template <unsigned D>
    inline
    algebraic_face<D>::algebraic_face(const face<D>& f, bool sign)
      : super_(f), sign_(sign)
    {
      // Ensure N is compatible with D.
      mln_precondition(f.n() <= D);
    }

    template <unsigned D>
    template <unsigned N>
    inline
    algebraic_face<D>::algebraic_face(const algebraic_n_face<N, D>& f)
      : super_(f), sign_(f.sign())
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
    }


    template <unsigned D>
    inline
    bool
    algebraic_face<D>::sign() const
    {
      return sign_;
    }

    template <unsigned D>
    inline
    void
    algebraic_face<D>::set_sign(bool sign)
    {
      sign_ = sign;
    }


    template <unsigned D>
    algebraic_face<D>
    make_algebraic_face(const face<D>& f, bool sign)
    {
      return algebraic_face<D>(f, sign);
    }


    template <unsigned D>
    algebraic_face<D>
    operator-(const face<D>& f)
    {
      return algebraic_face<D>(f, false);
    }

    template <unsigned D>
    algebraic_face<D>
    operator-(const algebraic_face<D>& f)
    {
      algebraic_face<D> f2(f);
      f2.set_sign(!f.sign());
      return f2;
    }


    template <unsigned D>
    inline
    bool
    operator==(const algebraic_face<D>& lhs, const algebraic_face<D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return
	lhs.n() == rhs.n() &&
	lhs.face_id() == rhs.face_id() &&
	lhs.sign() == rhs.sign();
    }

    template <unsigned D>
    inline
    bool
    operator!=(const algebraic_face<D>& lhs, const algebraic_face<D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return !(lhs == rhs);
    }

    template <unsigned D>
    inline
    bool
    operator< (const algebraic_face<D>& lhs, const algebraic_face<D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      // Ensure LHS and RHS have the same dimension.
      mln_precondition(lhs.n() == rhs.n());
      return lhs.face_id() < rhs.face_id();
    }


    template <unsigned D>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const algebraic_face<D>& f)
    {
      return
	ostr << "(cplx = " << f.cplx().addr() << ", dim = " << f.n()
	     << ", id = " << f.face_id() << ", sign = " << f.sign()<< ')';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_FACE_HH
