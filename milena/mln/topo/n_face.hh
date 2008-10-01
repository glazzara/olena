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

#ifndef MLN_TOPO_N_FACE_HH
# define MLN_TOPO_N_FACE_HH

/// \file mln/topo/n_face.hh
/// \brief n-face of a complex.

#include <limits>

#include <mln/core/contract.hh>


namespace mln
{

  namespace topo
  {

    // Forward declaration.
    template <unsigned D> class complex;
    template <unsigned N, unsigned D> class face_data;


    /*---------.
    | n-Face.  |
    `---------*/

    /// \brief \p N-face handle in a complex.
    ///
    /// Contrary to an mln::topo::face, the dimension of an
    /// mln::topo::n_face is fixed.
    template <unsigned N, unsigned D>
    struct n_face
    {
      // The type of the complex this handle points to.
      typedef complex<D> complex_type;

      /// Build a non-initialized face handle.
      n_face();
      /// Build a face handle from \a complex and \a face_id.
      n_face(complex<D>& complex, unsigned face_id);

      /// Is this handle valid?
      bool is_valid() const;
      /// Invalidate this handle.
      void invalidate();

      /// Accessors.
      /// \{
      /// Return the complex the face belongs to.
      complex<D>& cplx() const;
      /// Return the id of the face.
      // FIXME: Rename as `id'?
      unsigned face_id() const;

      /// Set the complex the face belongs to.
      void set_cplx(complex<D>& cplx);
      /// Return the dimension of the face.
      // FIXME: Rename as `dim'?
      unsigned n() const;
      /// Set the id of the face.
      void set_face_id(unsigned face_id);

      /// Return the mln::topo::face_data pointed by this handle.
      face_data<N, D>& data() const;
      /// \}

    private:
      /// \brief The complex the face belongs to.
      ///
      /// A const mln::topo::n_face can be used to modify a complex.
      mutable complex<D>* cplx_;
      /// \brief The id of the face.
      // FIXME: Rename as `id_'?
      unsigned face_id_;
    };


    /// Create a handle for \p N-face of a \p D-complex.
    template <unsigned N, unsigned D>
    n_face<N, D>
    make_n_face(const complex<D>& c, unsigned face_id);


    /// Comparison of two instances of mln::topo::n_face.
    /// \{
    /// \brief Is \a lhs equal to \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned N, unsigned D>
    bool
    operator==(const n_face<N, D>& lhs, const n_face<N, D>& rhs);

    /// \brief Is \a lhs ``less'' than \a rhs?
    ///
    /// This comparison is required by algorithms sorting face handles.
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned N, unsigned D>
    bool
    operator< (const n_face<N, D>& lhs, const n_face<N, D>& rhs);
    /// \}


    /// Print an mln::topo::n_face.
    template <unsigned N, unsigned D>
    std::ostream&
    operator<<(std::ostream& ostr, const n_face<N, D>& f);



# ifndef MLN_INCLUDE_ONLY

    template <unsigned N, unsigned D>
    inline
    n_face<N, D>::n_face()
      : cplx_(0), face_id_(std::numeric_limits<unsigned>::max())
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
    }

    template <unsigned N, unsigned D>
    inline
    n_face<N, D>::n_face(complex<D>& c, unsigned face_id)
      : cplx_(&c), face_id_(face_id)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
    }

    template <unsigned N, unsigned D>
    inline
    bool
    n_face<N, D>::is_valid() const
    {
      return cplx_ != 0 && face_id_ < cplx_->template nfaces<N>();
    }

    template <unsigned N, unsigned D>
    inline
    void
    n_face<N, D>::invalidate()
    {
      set_face_id(std::numeric_limits<unsigned>::max());
    }

    template <unsigned N, unsigned D>
    inline
    complex<D>&
    n_face<N, D>::cplx() const
    {
      mln_precondition(cplx_);
      return *cplx_;
    }

    template <unsigned N, unsigned D>
    inline
    unsigned
    n_face<N, D>::n() const
    {
      return N;
    }

    template <unsigned N, unsigned D>
    inline
    unsigned
    n_face<N, D>::face_id() const
    {
      return face_id_;
    }

    template <unsigned N, unsigned D>
    inline
    void
    n_face<N, D>::set_cplx(complex<D>& cplx)
    {
      cplx_ = &cplx;
    }

    template <unsigned N, unsigned D>
    inline
    void
    n_face<N, D>::set_face_id(unsigned face_id)
    {
      face_id_ = face_id;
    }

    template <unsigned N, unsigned D>
    inline
    face_data<N, D>&
    n_face<N, D>::data() const
    {
      mln_precondition(is_valid());
      return cplx_->template face_data_<N>(face_id_);
    }


    template <unsigned N, unsigned D>
    inline
    n_face<N, D>
    make_n_face(const complex<D>& c, unsigned face_id)
    {
      return n_face<N, D>(&c, face_id);
    }


    template <unsigned N, unsigned D>
    inline
    bool
    operator==(const n_face<N, D>& lhs, const n_face<N, D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return lhs.face_id() == rhs.face_id();
    }

    template <unsigned N, unsigned D>
    inline
    bool
    operator< (const n_face<N, D>& lhs, const n_face<N, D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return lhs.face_id() < rhs.face_id();
    }


    template <unsigned N, unsigned D>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const n_face<N, D>& f)
    {
      return ostr << "(cplx = " << &f.cplx() << ", dim = " << f.n()
		  << ", id = " << f.face_id() << ')';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_N_FACE_HH
