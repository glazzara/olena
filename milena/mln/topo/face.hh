// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
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

#ifndef MLN_TOPO_FACE_HH
# define MLN_TOPO_FACE_HH

/// \file
///
/// \brief Face of a complex.

# include <iostream>
# include <vector>

# include <mln/value/internal/limits.hh>
# include <mln/core/contract.hh>
# include <mln/metal/bool.hh>


namespace mln
{

  namespace topo
  {

    // Forward declarations.
    template <unsigned D> class complex;
    template <unsigned N, unsigned D> class n_face;
    template <unsigned N, unsigned D> class face_data;
    template <unsigned N> struct algebraic_face;


    /*-------.
    | Face.  |
    `-------*/

    /// Face handle in a complex; the face dimension is dynamic.
    ///
    /// Contrary to an mln::topo::n_face, the dimension of an
    /// mln::topo::face is not fixed.
    template <unsigned D>
    class face
    {
    public:
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
      complex<D> cplx() const;
      /// Return the dimension of the face.
      // FIXME: Rename as `dim'?
      unsigned n() const;
      /// Return the id of the face.
      // FIXME: Rename as `id'?
      unsigned face_id() const;

      /// Set the complex the face belongs to.
      void set_cplx(const complex<D>& cplx);

      /// Set the dimension of the face.
      void set_n(unsigned n);
      /// Increment the dimension of the face.
      void inc_n();
      /// Decrement the dimension of the face.
      void dec_n();

      /// Set the id of the face.
      void set_face_id(unsigned face_id);
      /// Increment the id of the face.
      void inc_face_id();
      /// Decrement the id of the face.
      void dec_face_id();

      /// Return the mln::topo::face_data pointed by this handle.
      template <unsigned N>
      face_data<N, D>& data() const;

      // FIXME: To be overhauled.
      /// Return an array of face handles pointing to adjacent (n-1)-faces.
      std::vector< algebraic_face<D> > lower_dim_adj_faces() const;
      /// Return an array of face handles pointing to adjacent (n+1)-faces.
      std::vector< algebraic_face<D> > higher_dim_adj_faces() const;
      /// \}

    private:
      /// The complex the face belongs to.
      ///
      /// A const face can be used to modify a complex.
      mutable complex<D> cplx_;
      /// The dimension of the face.
      // FIXME: Rename as `dim_'?
      unsigned n_;
      /// The id of the face.
      // FIXME: Rename as `id_'?
      unsigned face_id_;
    };


    /// Comparison of two instances of mln::topo::face.
    /// \{

    /// Is \a lhs equal to \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned D>
    bool operator==(const face<D>& lhs, const face<D>& rhs);

    /// Is \a lhs different from \a rhs?
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    template <unsigned D>
    bool operator!=(const face<D>& lhs, const face<D>& rhs);

    /// Is \a lhs ``less'' than \a rhs?
    ///
    /// This comparison is required by algorithms sorting face handles.
    ///
    /// \pre Arguments \a lhs and \a rhs must belong to the same
    /// mln::topo::complex.
    /// \pre Arguments \a lhs and \a rhs must have the same dimension.
    template <unsigned D>
    bool operator< (const face<D>& lhs, const face<D>& rhs);

    /// \}


    /// Print an mln::topo::face.
    template <unsigned D>
    std::ostream&
    operator<<(std::ostream& ostr, const face<D>& f);



# ifndef MLN_INCLUDE_ONLY

    template <unsigned D>
    inline
    face<D>::face()
      : cplx_(),
	n_(value::internal::limits<unsigned>::max()),
	face_id_(value::internal::limits<unsigned>::max())
    {
    }

    template <unsigned D>
    inline
    face<D>::face(complex<D>& c, unsigned n, unsigned face_id)
      : cplx_(c), n_(n), face_id_(face_id)
    {
      // Ensure N is compatible with D.
      mln_precondition(n <= D);
    }

    template <unsigned D>
    template <unsigned N>
    inline
    face<D>::face(const n_face<N, D>& f)
      : cplx_(f.cplx()), n_(N), face_id_(f.face_id())
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
    }

    template <unsigned D>
    inline
    bool
    face<D>::is_valid() const
    {
      return n_ <= D && face_id_ < cplx_.nfaces_of_dim(n_);
    }

    template <unsigned D>
    inline
    void
    face<D>::invalidate()
    {
      set_n(value::internal::limits<unsigned>::max());
      set_face_id(value::internal::limits<unsigned>::max());
    }

    template <unsigned D>
    inline
    complex<D>
    face<D>::cplx() const
    {
      return cplx_;
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
    face<D>::set_cplx(const complex<D>& cplx)
    {
      cplx_ = cplx;
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
    face<D>::inc_n()
    {
      ++n_;
    }

    template <unsigned D>
    inline
    void
    face<D>::dec_n()
    {
      --n_;
    }

    template <unsigned D>
    inline
    void
    face<D>::set_face_id(unsigned face_id)
    {
      face_id_ = face_id;
    }

    template <unsigned D>
    inline
    void
    face<D>::inc_face_id()
    {
      ++face_id_;
    }

    template <unsigned D>
    inline
    void
    face<D>::dec_face_id()
    {
      --face_id_;
    }

    template <unsigned D>
    template <unsigned N>
    inline
    face_data<N, D>&
    face<D>::data() const
    {
      mln_precondition(n_ == N);
      mln_precondition(is_valid());
      return cplx_.template face_data_<N>(face_id_);
    }


    /*-----------------------------------------------.
    | Helpers for face<D>::lower_dim_adj_faces() and |
    | face<D>::higher_dim_adj_faces().               |
    `-----------------------------------------------*/

    /* FIXME: This is way too complicated; should disappear when the
       implementation of complexes is simplified (see
       https://trac.lrde.org/olena/ticket/168).  */

    // Implementation is in mln/topo/algebraic_face.hh.

    namespace internal
    {

      template <unsigned N, unsigned D>
      struct lower_dim_adj_faces_if_dim_matches_
      {
	std::vector< algebraic_face<D> > operator()(const face<D>& face);
      };

      template <unsigned D>
      struct lower_dim_adj_faces_if_dim_matches_<1, D>
      {
	std::vector< algebraic_face<D> > operator()(const face<D>& face);
      };

      template <unsigned N, unsigned D>
      struct higher_dim_adj_faces_if_dim_matches_
      {
	std::vector< algebraic_face<D> > operator()(const face<D>& face);
      };

      template <unsigned D>
      struct higher_dim_adj_faces_if_dim_matches_<0, D>
      {
	std::vector< algebraic_face<D> > operator()(const face<D>& face);
      };

    } // end of namespace mln::topo::internal


    template <unsigned D>
    inline
    std::vector< algebraic_face<D> >
    face<D>::lower_dim_adj_faces() const
    {
      // FIXME: Warning: might prevent any attempt to build a complex<0>.
      metal::bool_<( D != 0 )>::check();

      return n_ > 0 ?
	internal::lower_dim_adj_faces_if_dim_matches_<D, D>()(*this) :
	std::vector< algebraic_face<D> >();
    }

    template <unsigned D>
    inline
    std::vector< algebraic_face<D> >
    face<D>::higher_dim_adj_faces() const
    {
      // FIXME: Warning: might prevent any attempt to build a complex<0>.
      metal::bool_<( D != 0 )>::check();

      return n_ < D ?
	internal::higher_dim_adj_faces_if_dim_matches_<D - 1, D>()(*this) :
	std::vector< algebraic_face<D> >();
    }


    template <unsigned D>
    inline
    bool
    operator==(const face<D>& lhs, const face<D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return lhs.n() == rhs.n() && lhs.face_id() == rhs.face_id();
    }

    template <unsigned D>
    inline
    bool
    operator!=(const face<D>& lhs, const face<D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return !(lhs == rhs);
    }

    template <unsigned D>
    inline
    bool
    operator< (const face<D>& lhs, const face<D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(lhs.cplx() == rhs.cplx());
      return lhs.n() < rhs.n() ||
	(lhs.n() == rhs.n() && lhs.face_id() < rhs.face_id());
    }


    template <unsigned D>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const face<D>& f)
    {
      return ostr << "(cplx = " << f.cplx().addr() << ", dim = " << f.n()
		  << ", id = " << f.face_id() << ')';
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_FACE_HH
