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

#ifndef MLN_TOPO_ALGEBRAIC_N_FACE_HH
# define MLN_TOPO_ALGEBRAIC_N_FACE_HH

/// \file mln/topo/algebraic_n_face.hh
/// \brief Algebraic n-face of a complex.

#include <mln/topo/n_face.hh>


namespace mln
{

  namespace topo
  {

    /*-------------------.
    | Algebraic n-Face.  |
    `-------------------*/

    /// \brief Algebraic \p N-face handle in a complex.
    ///
    /// Contrary to an mln::topo::algebraic_face, the dimension of an
    /// mln::topo::algebraic_n_face is fixed.
    template <unsigned N, unsigned D>
    class algebraic_n_face : public n_face<N, D>
    {
      typedef n_face<N, D> super_;

    public:
      /// Build a non-initialized algebraic face handle.
      algebraic_n_face();
      /// Build an algebraic face handle from \a complex and \a face_id.
      algebraic_n_face(complex<D>& complex, unsigned face_id, bool sign);
      /// Build an algebraic face handle from an mln::n_face.
      algebraic_n_face(const n_face<N, D>& f, bool sign);

      /// Accessors.
      /// \{
      /// Return the sign of this face.
      bool sign() const;
      /// Set the sign of this face.
      void set_sign(bool sign);
      /// \}

    private:
      bool sign_;
    };


    /// Create an algebraic \p N-face handle of a \p D-complex.
    template <unsigned N, unsigned D>
    algebraic_n_face<N, D>
    make_algebraic_n_face(const n_face<N, D>& f, bool sign);


    /// Inversion operators.
    /// \{
    template <unsigned N, unsigned D>
    algebraic_n_face<N, D>
    operator-(const n_face<N, D>& f);

    template <unsigned N, unsigned D>
    algebraic_n_face<N, D>
    operator-(const algebraic_n_face<N, D>& f);
    /// \}


    /// Comparison of two instances of mln::topo::algebraic_n_face.
    /// \{

    /// \brief Is \a lhs equal to \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned N, unsigned D>
    bool
    operator==(const algebraic_n_face<N, D>& lhs,
	       const algebraic_n_face<N, D>& rhs);

    /// \brief Is \a lhs different from \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned N, unsigned D>
    bool
    operator!=(const algebraic_n_face<N, D>& lhs,
	       const algebraic_n_face<N, D>& rhs);

    /// \brief Is \a lhs ``less'' than \a rhs?
    ///
    /// This comparison is required by algorithms sorting algebraic
    /// face handles.
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned N, unsigned D>
    bool
    operator< (const algebraic_n_face<N, D>& lhs,
	       const algebraic_n_face<N, D>& rhs);

    /// \}


    /// Print an mln::topo::algebraic_n_face.
    template <unsigned N, unsigned D>
    std::ostream&
    operator<<(std::ostream& ostr, const algebraic_n_face<N, D>& f);



# ifndef MLN_INCLUDE_ONLY

    template <unsigned N, unsigned D>
    inline
    algebraic_n_face<N, D>::algebraic_n_face()
      : super_(), sign_(true)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
      mln_postcondition(!this->is_valid());
    }

    template <unsigned N, unsigned D>
    inline
    algebraic_n_face<N, D>::algebraic_n_face(complex<D>& c, unsigned face_id,
					     bool sign)
      : super_(c, face_id), sign_(sign)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
    }

    template <unsigned N, unsigned D>
    inline
    algebraic_n_face<N, D>::algebraic_n_face(const n_face<N, D>& f, bool sign)
      : super_(f), sign_(sign)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
    }


    template <unsigned N, unsigned D>
    inline
    bool
    algebraic_n_face<N, D>::sign() const
    {
      return sign_;
    }

    template <unsigned N, unsigned D>
    inline
    void
    algebraic_n_face<N, D>::set_sign(bool sign)
    {
      sign_ = sign;
    }


    template <unsigned N, unsigned D>
    algebraic_n_face<N, D>
    make_algebraic_n_face(const n_face<N, D>& f, bool sign)
    {
      return algebraic_n_face<N, D>(f, sign);
    }


    template <unsigned N, unsigned D>
    algebraic_n_face<N, D>
    operator-(const n_face<N, D>& f)
    {
      return algebraic_n_face<N, D>(f, false);
    }

    template <unsigned N, unsigned D>
    algebraic_n_face<N, D>
    operator-(const algebraic_n_face<N, D>& f)
    {
      algebraic_n_face<N, D> f2(f);
      f2.set_sign(!f.sign());
      return f2;
    }


    template <unsigned N, unsigned D>
    inline
    bool
    operator==(const algebraic_n_face<N, D>& lhs,
	       const algebraic_n_face<N, D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return lhs.face_id() == rhs.face_id() && lhs.sign() == rhs.sign();
    }

    template <unsigned N, unsigned D>
    inline
    bool
    operator!=(const algebraic_n_face<N, D>& lhs,
	       const algebraic_n_face<N, D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return !(lhs == rhs);
    }

    template <unsigned N, unsigned D>
    inline
    bool
    operator< (const algebraic_n_face<N, D>& lhs,
	       const algebraic_n_face<N, D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return lhs.face_id() < rhs.face_id();
    }


    template <unsigned N, unsigned D>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const algebraic_n_face<N, D>& f)
    {
      return
	ostr << "(cplx = " << f.cplx().addr() << ", dim = " << f.n()
	     << ", id = " << f.face_id() << ", sign = " << f.sign()<< ')';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_ALGEBRAIC_N_FACE_HH
