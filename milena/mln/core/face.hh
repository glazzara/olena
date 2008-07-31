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
///
/// FIXME: More.

#include <vector>

#include <mln/core/contract.hh>


namespace mln
{

  // Forward declarations.
  template <unsigned D> class complex;
  template <unsigned N, unsigned D> class face_handle;
  namespace internal
  {
    template <unsigned N, unsigned D>
    struct faces_set_mixin;
  }


  /*-------.
  | Face.  |
  `-------*/

  /* FIXME: we might want to factor connect_{higher,lower}_dim_cell
     and {higher,lower_dim_faces_} as member of super classes.  */

  template <unsigned N, unsigned D>
  class face
  {
  public:
    void connect_higher_dim_face(const face_handle<N + 1, D>& f);
    void connect_lower_dim_face (const face_handle<N - 1, D>& f);

  private:
    friend class mln::internal::faces_set_mixin<N, D>;

    // FIXME: Provide accessors instead of using `friend' if there are
    // clients other than mln::internal::faces_set_mixin<N, D>.
    std::vector< face_handle<N + 1, D> > higher_dim_faces_;
    std::vector< face_handle<N - 1, D> > lower_dim_faces_;
  };

  // Specialization for the faces of lowest dimension (0).
  template <unsigned D>
  class face<0u, D>
  {
  public:
    void connect_higher_dim_face(const face_handle<1u, D>& f);

  private:
    friend class mln::internal::faces_set_mixin<0, D>;

    // FIXME: Provide accessors instead of using `friend; if there are
    // clients other than mln::internal::faces_set_mixin<0, D>.
    std::vector< face_handle<1u, D> > higher_dim_faces_;
  };

  // Specialization for the faces of highest dimension (D).
  template <unsigned D>
  class face<D, D>
  {
  public:
    void connect_lower_dim_face(const face_handle<1u, D>& f);

  private:
    friend class mln::internal::faces_set_mixin<D, D>;

    // FIXME: Provide accessors instead of using `friend; if there are
    // clients other than mln::internal::faces_set_mixin<0, D>.
    std::vector< face_handle<1u, D> > lower_dim_faces_;
  };

  // Specialization for the case of a 0-complex.
  template <>
  class face<0u, 0u>
  {
  };


  /*--------------.
  | Face handle.  |
  `--------------*/

  // Face handle in a complex.
  template <unsigned N, unsigned D>
  struct face_handle
  {
    face_handle(complex<D>& complex, unsigned face_id);

    /// Copy and assignment.
    /// \{
    face_handle(const face_handle<N, D>& rhs);
    face_handle<N, D>& operator=(const face_handle<N, D>& rhs);
    /// \}

    /// Accessors.
    /// \{
    /// Return the mln::face pointed by this handle.
    face<N, D>& get() const;
    /// \}

    /* FIXME: Hide data.  */
    // A const face_handle can be used to modify a complex.
    mutable complex<D>& c_;
    unsigned face_id_;
  };

  template <unsigned N, unsigned D>
  face_handle<N, D>
  make_face_handle(const complex<D>& c, unsigned face_id);


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



# ifndef MLN_INCLUDE_ONLY

  /*--------------.
  | Face handle.  |
  `--------------*/

  template <unsigned N, unsigned D>
  face_handle<N, D>::face_handle(complex<D>& c, unsigned face_id)
    : c_(c), face_id_(face_id)
  {
  }

  template <unsigned N, unsigned D>
  face_handle<N, D>::face_handle(const face_handle<N, D>& rhs)
    : c_(rhs.c_), face_id_(rhs.face_id_)
  {
  }

  template <unsigned N, unsigned D>
  face_handle<N, D>&
  face_handle<N, D>::operator=(const face_handle<N, D>& rhs)
  {
    if (&rhs != this)
      {
	c_ = rhs.c_;
	face_id_ = rhs.face_id_;
      }
    return *this;
  }

  template <unsigned N, unsigned D>
  face<N, D>&
  face_handle<N, D>::get() const
  {
    return c_.template face_<N>(face_id_);
  }


  template <unsigned N, unsigned D>
  face_handle<N, D>
  make_face_handle(const complex<D>& c, unsigned face_id)
  {
    return face_handle<N, D>(c, face_id);
  }


  /*--------.
  | Faces.  |
  `--------*/

  template <unsigned N, unsigned D>
  void
  face<N, D>::connect_higher_dim_face(const face_handle<N + 1, D>& f)
  {
    higher_dim_faces_.push_back(f); 
  }

  template <unsigned N, unsigned D>
  void
  face<N, D>::connect_lower_dim_face(const face_handle<N - 1, D>& f)
  {
    lower_dim_faces_.push_back(f); 
  }

  template <unsigned D>
  void
  face<0u, D>::connect_higher_dim_face(const face_handle<1u, D>& f)
  {
    higher_dim_faces_.push_back(f);
  }

  template <unsigned D>
  void
  face<D, D>::connect_lower_dim_face(const face_handle<1u, D>& f)
  {
    lower_dim_faces_.push_back(f);
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
      mln_assertion(&faces_.front().c_ == &f.c_);

    /* FIXME: This is not thread-proof (these two lines should
       form an atomic section).  */
    faces_.push_back (f);
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

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_FACE_HH
