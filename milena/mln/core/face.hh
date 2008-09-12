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

#ifndef MLN_CORE_FACE_HH
# define MLN_CORE_FACE_HH

/// \file mln/core/face.hh
/// \brief Face of a complex.

#include <limits>

#include <vector>

#include <mln/core/contract.hh>

/* FIXME: Suggestions:

   - rename `face' as `face_data', and move it into complex.hh or its
     own file;
   - rename `face_handle' as `face', and move it to its own file;
   - rename `any_face_handle' as `any_face', and move it to its own file.

   Anyway, whatever the decision, splitting `face', `face_handle' and
   `any_face_handle' seems to be sound.

   (And what about `faces_set'? Should we move it to its own file as
   well?)  */

namespace mln
{

  // Forward declarations (external).
  template <unsigned D> class complex;
  namespace internal
  {
    template <unsigned N, unsigned D> struct lower_dim_faces_set_mixin;
    template <unsigned N, unsigned D> struct higher_dim_faces_set_mixin;
  }

  // Forward declarations (internal).
  template <unsigned N, unsigned D> class face_handle;
  namespace internal
  {
    template <unsigned N, unsigned D> class lower_dim_faces_mixin;
    template <unsigned N, unsigned D> class higher_dim_faces_mixin;
  }


  /*-------.
  | Face.  |
  `-------*/

  /// \p N-face of a \p D-complex.
  template <unsigned N, unsigned D> class face;


  // Specialization for the faces of highest dimension (\p D).
  template <unsigned D>
  class face<D, D> : public internal::lower_dim_faces_mixin<D, D>
  {
  };

  // Specialization for the faces of intermediate dimension (greater
  // than 0, lower than \p D).
  template <unsigned N, unsigned D>
  class face : public internal::lower_dim_faces_mixin<N, D>,
	       public internal::higher_dim_faces_mixin<N, D>
  {
  };

  // Specialization for the faces of lowest dimension (0).
  template <unsigned D>
  class face<0u, D> : public internal::higher_dim_faces_mixin<0u, D>
  {
  };

  // Specialization for the case of a 0-complex.
  template <>
  class face<0u, 0u>
  {
  };


  namespace internal
  {

    /// Factored implementation of faces.
    /// \{
    template <unsigned N, unsigned D>
    class lower_dim_faces_mixin
    {
    public:
      void connect_lower_dim_face (const face_handle<N - 1, D>& f);
    private:
      friend class mln::internal::lower_dim_faces_set_mixin<N, D>;
      std::vector< face_handle<N - 1, D> > lower_dim_faces_;
    };  

    template <unsigned N, unsigned D>
    class higher_dim_faces_mixin
    {
    public:
      void connect_higher_dim_face(const face_handle<N + 1, D>& f);
    private:
      friend class mln::internal::higher_dim_faces_set_mixin<N, D>;      
      std::vector< face_handle<N + 1, D> > higher_dim_faces_;
    };
    /// \}

  } // end of namespace mln::internal



  /*--------------.
  | Face handle.  |
  `--------------*/

  // Face handle in a complex.
  template <unsigned N, unsigned D>
  struct face_handle
  {
    // The type of the complex this handle points to.
    typedef complex<D> complex_type;

    /// Build a non-initialized face handle.
    face_handle();
    /// Build a face handle from \a complex and \a face_id.
    face_handle(complex<D>& complex, unsigned face_id);

    /// Copy and assignment.
    /// \{
    face_handle(const face_handle<N, D>& rhs);
    face_handle<N, D>& operator=(const face_handle<N, D>& rhs);
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

    /// Return the mln::face pointed by this handle.
    face<N, D>& to_face() const;
    /// \}

  private:
    /// \brief The complex the face belongs to.
    ///
    /// A const face_handle can be used to modify a complex.
    mutable complex<D>* cplx_;
    /// \brief The id of the face.
    // FIXME: Rename as `id_'?
    unsigned face_id_;
  };


  /// Create a handle for \p N-face of a \p D-complex.
  template <unsigned N, unsigned D>
  face_handle<N, D>
  make_face_handle(const complex<D>& c, unsigned face_id);


  /// Comparison of two instances of mln::face_handle.
  /// \{
  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  template <unsigned N, unsigned D>
  bool
  operator==(const face_handle<N, D>& lhs, const face_handle<N, D>& rhs);

  /// \brief Is \a lhs ``less'' than \a rhs?
  ///
  /// This comparison is required by algorithms sorting face handles.
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  template <unsigned N, unsigned D>
  bool
  operator< (const face_handle<N, D>& lhs, const face_handle<N, D>& rhs);
  /// \}


  /*----------------------.
  | Set of face handles.  |
  `----------------------*/

  /// \brief Set of face handles of dimension \p N.
  template <unsigned N, unsigned D>
  class faces_set
  {
  public:
    void add(const face_handle<N, D>& f);

    /// \brief Accessors.
    ///
    /// Return the set of handles.
    /// \{
    const std::vector< face_handle<N, D> >& faces() const;
    /// \}

  private:
    friend class mln::complex<D>;

    // FIXME: Rename this as `handles_'?
    std::vector< face_handle<N, D> > faces_;
  };


  /// Construction helpers for mln::faces_set.
  /// \{
  template <unsigned N, unsigned D>
  faces_set<N, D>
  operator+(const face_handle<N, D>& f1, const face_handle<N, D>& f2);

  template <unsigned N, unsigned D>
  faces_set<N, D>
  operator+(const faces_set<N, D>& fs, const face_handle<N, D>& f);
  /// \}


  /*----------------------.
  | ``Any-face'' handle.  |
  `----------------------*/

  // Face handle in a complex, where the dimension is dynamic.
  template <unsigned D>
  struct any_face_handle
  {
    // The type of the complex this handle points to.
    typedef complex<D> complex_type;

    /// Build a non-initialized face handle.
    any_face_handle();
    /// Build a face handle from \a complex and \a face_id.
    any_face_handle(complex<D>& complex, unsigned n, unsigned face_id);

    /// Build a face handle from a face_handle.
    template <unsigned N>
    any_face_handle(const face_handle<N, D>& f);

    /// Copy and assignment.
    /// \{
    any_face_handle(const any_face_handle<D>& rhs);
    any_face_handle<D>& operator=(const any_face_handle<D>& rhs);
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

    /// Return the mln::face pointed by this handle.
    template <unsigned N>
    face<N, D>& to_face() const;
    /// \}

  private:
    /// \brief The complex the face belongs to.
    ///
    /// A const any_face_handle can be used to modify a complex.
    mutable complex<D>* cplx_;
    /// The dimension of the face.
    // FIXME: Rename as `dim_'?
    unsigned n_;
    /// \brief The id of the face.
    // FIXME: Rename as `id_'?
    unsigned face_id_;
  };


  /// Comparison of two instances of mln::any_face_handle.
  /// \{
  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  template <unsigned D>
  bool
  operator==(const any_face_handle<D>& lhs, const any_face_handle<D>& rhs);

  /// \brief Is \a lhs ``less'' than \a rhs?
  ///
  /// This comparison is required by algorithms sorting face handles.
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::complex.
  /// \pre Arguments \a lhs and \a rhs must have the same dimension.
  template <unsigned D>
  bool
  operator< (const any_face_handle<D>& lhs, const any_face_handle<D>& rhs);
  /// \}



# ifndef MLN_INCLUDE_ONLY

  /*--------.
  | Faces.  |
  `--------*/

  namespace internal
  {
    template <unsigned N, unsigned D>
    void
    lower_dim_faces_mixin<N, D>::connect_lower_dim_face(const face_handle<N - 1, D>& f)
    {
      lower_dim_faces_.push_back(f);
    }

    template <unsigned N, unsigned D>
    void
    higher_dim_faces_mixin<N, D>::connect_higher_dim_face(const face_handle<N + 1, D>& f)
    {
      higher_dim_faces_.push_back(f);
    }

  } // end of namespace mln::internal


  /*--------------.
  | Face handle.  |
  `--------------*/

  template <unsigned N, unsigned D>
  face_handle<N, D>::face_handle()
    : cplx_(0), face_id_(std::numeric_limits<unsigned>::max())
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D>
  face_handle<N, D>::face_handle(complex<D>& c, unsigned face_id)
    : cplx_(&c), face_id_(face_id)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D>
  face_handle<N, D>::face_handle(const face_handle<N, D>& rhs)
    : cplx_(rhs.cplx_), face_id_(rhs.face_id_)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
  }

  template <unsigned N, unsigned D>
  face_handle<N, D>&
  face_handle<N, D>::operator=(const face_handle<N, D>& rhs)
  {
    if (&rhs != this)
      {
	cplx_ = rhs.cplx_;
	face_id_ = rhs.face_id_;
      }
    return *this;
  }

  template <unsigned N, unsigned D>
  bool
  face_handle<N, D>::is_valid() const
  {
    return cplx_ != 0 && face_id_ < cplx_->template nfaces<N>();
  }

  template <unsigned N, unsigned D>
  void
  face_handle<N, D>::invalidate()
  {
    set_face_id(std::numeric_limits<unsigned>::max());
  }

  template <unsigned N, unsigned D>
  complex<D>&
  face_handle<N, D>::cplx() const
  {
    mln_precondition(cplx_);
    return *cplx_;
  }

  template <unsigned N, unsigned D>
  unsigned
  face_handle<N, D>::n() const
  {
    return N;
  }

  template <unsigned N, unsigned D>
  unsigned
  face_handle<N, D>::face_id() const
  {
    return face_id_;
  }

  template <unsigned N, unsigned D>
  void
  face_handle<N, D>::set_cplx(complex<D>& cplx)
  {
    cplx_ = &cplx;
  }

  template <unsigned N, unsigned D>
  void
  face_handle<N, D>::set_face_id(unsigned face_id)
  {
    face_id_ = face_id;
  }

  template <unsigned N, unsigned D>
  face<N, D>&
  face_handle<N, D>::to_face() const
  {
    mln_precondition(is_valid());
    return cplx_->template face_<N>(face_id_);
  }


  template <unsigned N, unsigned D>
  face_handle<N, D>
  make_face_handle(const complex<D>& c, unsigned face_id)
  {
    return face_handle<N, D>(&c, face_id);
  }

  template <unsigned N, unsigned D>
  bool
  operator==(const face_handle<N, D>& lhs, const face_handle<N, D>& rhs)
  {
    // Ensure LHS and RHS belong to the same complex.
    mln_precondition(&lhs.face.cplx() == &rhs.face.cplx());
    return lhs.face().id() == rhs.face().id();
  }

  template <unsigned N, unsigned D>
  bool
  operator< (const face_handle<N, D>& lhs, const face_handle<N, D>& rhs)
  {
    // Ensure LHS and RHS belong to the same complex.
    mln_precondition(&lhs.face.cplx() == &rhs.face.cplx());
    return lhs.face().id() < rhs.face().id();
  }


  /*---------------.
  | Set of faces.  |
  `---------------*/

  template <unsigned N, unsigned D>
  void
  faces_set<N, D>::add(const face_handle<N, D>& f)
  {
    // Check consistency.
    if (!faces_.empty())
      mln_precondition(&faces_.front().cplx() == &f.cplx());
    faces_.push_back(f);
  }

  template <unsigned N, unsigned D>
  const std::vector< face_handle<N, D> >&
  faces_set<N, D>::faces() const
  {
    return faces_;
  }


  template <unsigned N, unsigned D>
  faces_set<N, D>
  operator+(const face_handle<N, D>& f1, const face_handle<N, D>& f2)
  {
    faces_set<N, D> fs;
    fs.add(f1);
    fs.add(f2);
    return fs;
  }

  template <unsigned N, unsigned D>
  faces_set<N, D>
  operator+(const faces_set<N, D>& fs, const face_handle<N, D>& f)
  {
    faces_set<N, D> fs2(fs);
    fs2.add(f);
    return fs2;
  }


  /*----------------------.
  | ``Any-face'' handle.  |
  `----------------------*/

  template <unsigned D>
  any_face_handle<D>::any_face_handle()
    : cplx_(0),
      n_(std::numeric_limits<unsigned>::max()),
      face_id_(std::numeric_limits<unsigned>::max())
  {
  }

  template <unsigned D>
  any_face_handle<D>::any_face_handle(complex<D>& c, unsigned n,
				      unsigned face_id)
    : cplx_(&c), n_(n), face_id_(face_id)
  {
    // Ensure N is compatible with D.
    mln_precondition(n <= D);
  }

  template <unsigned D>
  template <unsigned N>
  any_face_handle<D>::any_face_handle(const face_handle<N, D>& f)
    : cplx_(&f.cplx()), n_(N), face_id_(f.face_id())
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();
    
  }

  template <unsigned D>
  any_face_handle<D>::any_face_handle(const any_face_handle<D>& rhs)
    : cplx_(rhs.cplx_), n_(rhs.n_), face_id_(rhs.face_id_)
  {
  }

  template <unsigned D>
  any_face_handle<D>&
  any_face_handle<D>::operator=(const any_face_handle<D>& rhs)
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
  bool
  any_face_handle<D>::is_valid() const
  {
    return cplx_ != 0 && n_ <= D && face_id_ < cplx_->nfaces(n_);
  }

  template <unsigned D>
  void
  any_face_handle<D>::invalidate()
  {
    set_n(std::numeric_limits<unsigned>::max());
    set_face_id(std::numeric_limits<unsigned>::max());
  }

  template <unsigned D>
  complex<D>&
  any_face_handle<D>::cplx() const
  {
    mln_precondition(cplx_);
    return *cplx_;
  }

  template <unsigned D>
  unsigned
  any_face_handle<D>::n() const
  {
    return n_;
  }

  template <unsigned D>
  unsigned
  any_face_handle<D>::face_id() const
  {
    return face_id_;
  }

  template <unsigned D>
  void
  any_face_handle<D>::set_cplx(complex<D>& cplx)
  {
    cplx_ = &cplx;
  }

  template <unsigned D>
  void
  any_face_handle<D>::set_n(unsigned n)
  {
    n_ = n;
  }

  template <unsigned D>
  void
  any_face_handle<D>::set_face_id(unsigned face_id)
  {
    face_id_ = face_id;
  }

  template <unsigned D>
  template <unsigned N>
  face<N, D>&
  any_face_handle<D>::to_face() const
  {
    mln_precondition(n_ == N);
    mln_precondition(is_valid());
    return cplx_->template face_<N>(face_id_);
  }


  template <unsigned D>
  bool
  operator==(const any_face_handle<D>& lhs, const any_face_handle<D>& rhs)
  {
    // Ensure LHS and RHS belong to the same complex.
    mln_precondition(&lhs.face.cplx() == &rhs.face.cplx());
    return
      lhs.face().n() == rhs.face().n() &&
      lhs.face().id() == rhs.face().id();
  }

  template <unsigned D>
  bool
  operator< (const any_face_handle<D>& lhs, const any_face_handle<D>& rhs)
  {
    // Ensure LHS and RHS belong to the same complex.
    mln_precondition(&lhs.face.cplx() == &rhs.face.cplx());
    // Ensure LHS and RHS have the same dimension.
    mln_precondition(lhs.face().n() == rhs.face().n());
    return lhs.face().id() < rhs.face().id();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_FACE_HH
