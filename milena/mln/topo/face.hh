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

#ifndef MLN_TOPO_FACE_HH
# define MLN_TOPO_FACE_HH

/// \file mln/topo/face.hh
/// \brief Face of a complex.

#include <limits>

#include <mln/core/contract.hh>

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

    /// \brief Face handle in a complex; the face dimension is dynamic.
    ///
    /// Contrary to an mln::topo::n_face, the dimension of an
    /// mln::topo::face is not fixed.
    template <unsigned D>
    struct face
    {
      // The type of the complex this handle points to.
      typedef complex<D> complex_type;

      /// Build a non-initialized face handle.
      face();
      /// Build a face handle from \a complex and \a face_id.
      face(complex<D>& complex, unsigned n, unsigned face_id);

      /// Build a face handle from an mln::topo::n_face.
      template <unsigned N>
      face(const n_face<N, D>& f);

      /// Is this handle valid?
      bool is_valid() const;
      /// Invalidate this handle.
      void invalidate();

      /// Accessors.
      /// \{
      /// Return the complex the face belongs to.
      complex<D>& cplx() const;
      /// Return the dimension of the face.
      // FIXME: Rename as `dim'?
      unsigned n() const;
      /// Return the id of the face.
      // FIXME: Rename as `id'?
      unsigned face_id() const;

      /// Set the complex the face belongs to.
      void set_cplx(complex<D>& cplx);
      /// Set the dimension of the face.
      void set_n(unsigned n);
      /// Set the id of the face.
      void set_face_id(unsigned face_id);

      /// Return the mln::topo::face_data pointed by this handle.
      template <unsigned N>
      face_data<N, D>& data() const;
      /// \}

    private:
      /// \brief The complex the face belongs to.
      ///
      /// A const face can be used to modify a complex.
      mutable complex<D>* cplx_;
      /// The dimension of the face.
      // FIXME: Rename as `dim_'?
      unsigned n_;
      /// \brief The id of the face.
      // FIXME: Rename as `id_'?
      unsigned face_id_;
    };


    /// Comparison of two instances of mln::topo::face.
    /// \{
    /// \brief Is \a lhs equal to \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned D>
    bool
    operator==(const face<D>& lhs, const face<D>& rhs);

    /// \brief Is \a lhs ``less'' than \a rhs?
    ///
    /// This comparison is required by algorithms sorting face handles.
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    /// \pre Arguments \a lhs and \a rhs must have the same dimension.
    template <unsigned D>
    bool
    operator< (const face<D>& lhs, const face<D>& rhs);
    /// \}


    /// Print an mln::topo::face.
    template <unsigned D>
    std::ostream&
    operator<<(std::ostream& ostr, const face<D>& f);



# ifndef MLN_INCLUDE_ONLY

    template <unsigned D>
    inline
    face<D>::face()
      : cplx_(0),
	n_(std::numeric_limits<unsigned>::max()),
	face_id_(std::numeric_limits<unsigned>::max())
    {
    }

    template <unsigned D>
    inline
    face<D>::face(complex<D>& c, unsigned n, unsigned face_id)
      : cplx_(&c), n_(n), face_id_(face_id)
    {
      // Ensure N is compatible with D.
      mln_precondition(n <= D);
    }

    template <unsigned D>
    template <unsigned N>
    inline
    face<D>::face(const n_face<N, D>& f)
      : cplx_(&f.cplx()), n_(N), face_id_(f.face_id())
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();

    }

    template <unsigned D>
    inline
    bool
    face<D>::is_valid() const
    {
      return cplx_ != 0 && n_ <= D && face_id_ < cplx_->nfaces(n_);
    }

    template <unsigned D>
    inline
    void
    face<D>::invalidate()
    {
      set_n(std::numeric_limits<unsigned>::max());
      set_face_id(std::numeric_limits<unsigned>::max());
    }

    template <unsigned D>
    inline
    complex<D>&
    face<D>::cplx() const
    {
      mln_precondition(cplx_);
      return *cplx_;
    }

    template <unsigned D>
    inline
    unsigned
    face<D>::n() const
    {
      return n_;
    }

    template <unsigned D>
    inline
    unsigned
    face<D>::face_id() const
    {
      return face_id_;
    }

    template <unsigned D>
    inline
    void
    face<D>::set_cplx(complex<D>& cplx)
    {
      cplx_ = &cplx;
    }

    template <unsigned D>
    inline
    void
    face<D>::set_n(unsigned n)
    {
      n_ = n;
    }

    template <unsigned D>
    inline
    void
    face<D>::set_face_id(unsigned face_id)
    {
      face_id_ = face_id;
    }

    template <unsigned D>
    template <unsigned N>
    inline
    face_data<N, D>&
    face<D>::data() const
    {
      mln_precondition(n_ == N);
      mln_precondition(is_valid());
      return cplx_->template face_data_<N>(face_id_);
    }


    template <unsigned D>
    inline
    bool
    operator==(const face<D>& lhs, const face<D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(&lhs.face.cplx() == &rhs.face.cplx());
      return
	lhs.face().n() == rhs.face().n() &&
	lhs.face().id() == rhs.face().id();
    }

    template <unsigned D>
    inline
    bool
    operator< (const face<D>& lhs, const face<D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(&lhs.face.cplx() == &rhs.face.cplx());
      // Ensure LHS and RHS have the same dimension.
      mln_precondition(lhs.face().n() == rhs.face().n());
      return lhs.face().id() < rhs.face().id();
    }


    template <unsigned D>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const face<D>& f)
    {
      return ostr << "(dim = " << f.n() << ", id = " << f.face_id() << ')';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_FACE_HH
