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

#include <vector>

#include <mln/core/contract.hh>

/* FIXME: Suggestions:

   Rename faces_set as n_faces_set.

   Split `face', `face_handle' and `any_face_handle' code into several
   files.

   `face_data' is somehow an implementation detail: it should be
   hidden, either by moving it to mln/topo/complex.cc or into its own
   file in mln/topo/internal.

   (And what about `faces_set'? Should we move it to its own file as
   well?)  */

namespace mln
{

  namespace topo
  {

    // Forward declarations (external).
    template <unsigned D> class complex;
    namespace internal
    {
      template <unsigned N, unsigned D> struct lower_dim_faces_set_mixin;
      template <unsigned N, unsigned D> struct higher_dim_faces_set_mixin;
    }

    // Forward declarations (internal).
    template <unsigned N, unsigned D> class n_face;
    namespace internal
    {
      template <unsigned N, unsigned D> class lower_dim_faces_data_mixin;
      template <unsigned N, unsigned D> class higher_dim_faces_data_mixin;
    }


    /*------------.
    | Face data.  |
    `------------*/

    /// Data (adjacent faces) associated to a \p N-face of a \p D-complex.
    template <unsigned N, unsigned D> class face_data;


    // Specialization for the faces of highest dimension (\p D).
    template <unsigned D>
    class face_data<D, D> : public internal::lower_dim_faces_data_mixin<D, D>
    {
    };

    // Specialization for the faces of intermediate dimension (greater
    // than 0, lower than \p D).
    template <unsigned N, unsigned D>
    class face_data : public internal::lower_dim_faces_data_mixin<N, D>,
		      public internal::higher_dim_faces_data_mixin<N, D>
    {
    };

    // Specialization for the faces of lowest dimension (0).
    template <unsigned D>
    class face_data<0u, D> : public internal::higher_dim_faces_data_mixin<0u, D>
    {
    };

    // Specialization for the case of a 0-complex.
    template <>
    class face_data<0u, 0u>
    {
    };


    namespace internal
    {

      /// Factored implementation of faces.
      /// \{
      template <unsigned N, unsigned D>
      class lower_dim_faces_data_mixin
      {
      public:
	void connect_lower_dim_face (const n_face<N - 1, D>& f);
      private:
	friend class mln::topo::internal::lower_dim_faces_set_mixin<N, D>;
	std::vector< n_face<N - 1, D> > lower_dim_faces_;
      };

      template <unsigned N, unsigned D>
      class higher_dim_faces_data_mixin
      {
      public:
	void connect_higher_dim_face(const n_face<N + 1, D>& f);
      private:
	friend class mln::topo::internal::higher_dim_faces_set_mixin<N, D>;
	std::vector< n_face<N + 1, D> > higher_dim_faces_;
      };
      /// \}

    } // end of namespace mln::topo::internal



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

      /// Copy and assignment.
      /// \{
      n_face(const n_face<N, D>& rhs);
      n_face<N, D>& operator=(const n_face<N, D>& rhs);
      /// \}

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
      face_data<N, D>& face_data() const;
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


    /*----------------------.
    | Set of face handles.  |
    `----------------------*/

    /// \brief Set of face handles of dimension \p N.
    template <unsigned N, unsigned D>
    class faces_set
    {
    public:
      void add(const n_face<N, D>& f);

      /// \brief Accessors.
      ///
      /// Return the set of handles.
      /// \{
      const std::vector< n_face<N, D> >& faces() const;
      /// \}

    private:
      friend class complex<D>;

      // FIXME: Rename this as `handles_'?
      std::vector< n_face<N, D> > faces_;
    };


    /// Construction helpers for mln::topo::faces_set.
    /// \{
    template <unsigned N, unsigned D>
    faces_set<N, D>
    operator+(const n_face<N, D>& f1, const n_face<N, D>& f2);

    template <unsigned N, unsigned D>
    faces_set<N, D>
    operator+(const faces_set<N, D>& fs, const n_face<N, D>& f);
    /// \}


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

      /// Copy and assignment.
      /// \{
      face(const face<D>& rhs);
      face<D>& operator=(const face<D>& rhs);
      /// \}

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
      face_data<N, D>& face_data() const;
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



# ifndef MLN_INCLUDE_ONLY

    /*--------.
    | Faces.  |
    `--------*/

    namespace internal
    {
      template <unsigned N, unsigned D>
      inline
      void
      lower_dim_faces_data_mixin<N, D>::connect_lower_dim_face(const n_face<N - 1, D>& f)
      {
	lower_dim_faces_.push_back(f);
      }

      template <unsigned N, unsigned D>
      inline
      void
      higher_dim_faces_data_mixin<N, D>::connect_higher_dim_face(const n_face<N + 1, D>& f)
      {
	higher_dim_faces_.push_back(f);
      }

    } // end of namespace mln::topo::internal


    /*--------------.
    | Face handle.  |
    `--------------*/

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
    n_face<N, D>::n_face(const n_face<N, D>& rhs)
      : cplx_(rhs.cplx_), face_id_(rhs.face_id_)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();
    }

    template <unsigned N, unsigned D>
    inline
    n_face<N, D>&
    n_face<N, D>::operator=(const n_face<N, D>& rhs)
    {
      if (&rhs != this)
	{
	  cplx_ = rhs.cplx_;
	  face_id_ = rhs.face_id_;
	}
      return *this;
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
    n_face<N, D>::face_data() const
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
      mln_precondition(&lhs.face.cplx() == &rhs.face.cplx());
      return lhs.face().id() == rhs.face().id();
    }

    template <unsigned N, unsigned D>
    inline
    bool
    operator< (const n_face<N, D>& lhs, const n_face<N, D>& rhs)
    {
      // Ensure LHS and RHS belong to the same complex.
      mln_precondition(&lhs.face.cplx() == &rhs.face.cplx());
      return lhs.face().id() < rhs.face().id();
    }


    /*---------------.
    | Set of faces.  |
    `---------------*/

    template <unsigned N, unsigned D>
    inline
    void
    faces_set<N, D>::add(const n_face<N, D>& f)
    {
      // Check consistency.
      if (!faces_.empty())
	mln_precondition(&faces_.front().cplx() == &f.cplx());
      faces_.push_back(f);
    }

    template <unsigned N, unsigned D>
    inline
    const std::vector< n_face<N, D> >&
    faces_set<N, D>::faces() const
    {
      return faces_;
    }


    template <unsigned N, unsigned D>
    inline
    faces_set<N, D>
    operator+(const n_face<N, D>& f1, const n_face<N, D>& f2)
    {
      faces_set<N, D> fs;
      fs.add(f1);
      fs.add(f2);
      return fs;
    }

    template <unsigned N, unsigned D>
    inline
    faces_set<N, D>
    operator+(const faces_set<N, D>& fs, const n_face<N, D>& f)
    {
      faces_set<N, D> fs2(fs);
      fs2.add(f);
      return fs2;
    }


    /*----------------------.
    | ``Any-face'' handle.  |
    `----------------------*/

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
    face<D>::face(complex<D>& c, unsigned n,
					unsigned face_id)
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
    face<D>::face(const face<D>& rhs)
      : cplx_(rhs.cplx_), n_(rhs.n_), face_id_(rhs.face_id_)
    {
    }

    template <unsigned D>
    inline
    face<D>&
    face<D>::operator=(const face<D>& rhs)
    {
      if (&rhs != this)
	{
	  cplx_ = rhs.cplx_;
	  n_ = rhs.n_;
	  face_id_ = rhs.face_id_;
	}
      return *this;
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
    face<D>::face_data() const
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

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_FACE_HH
