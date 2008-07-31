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

#ifndef MLN_CORE_COMPLEX_HH
# define MLN_CORE_COMPLEX_HH

/// \file mln/core/complex.hh
/// \brief Structures for general complexes.
///
/// A complexes defines a topological space which can be used as a
/// support for an image (i.e., as site sets).
///
/// FIXME: More.

# include <iosfwd>

# include <mln/metal/bool.hh>

# include <mln/core/face.hh>


namespace mln
{

  // Forward declaration.
  namespace internal
  {
    template <unsigned N, unsigned D>
    struct faces_set_mixin;
  }


  /*----------.
  | Complex.  |
  `----------*/

  /// \brief General complex of dimension \p D.
  template <unsigned D>
  class complex : private internal::faces_set_mixin<D, D>
  {
  public:
    /// Complex construction.
    /// \{

    /// \brief Add a 0-face to the complex.
    face_handle<0u, D> add_face();

    /// \brief Add a \p (N+1)-face to the complex (with \p N >= 0).
    ///
    /// \param adjacent_faces The (\p N-1)-faces adjacent to the new
    /// \p N-face.
    template <unsigned N>
    face_handle<N + 1, D> add_face(const faces_set<N, D>& adjacent_faces);
    /// \}

    /// Pretty-printing.
    /// \{
    /// Print the complex.
    void print(std::ostream& ostr) const;
    /// Print the faces of dimension \p N.
    template <unsigned N>
    void print_faces(std::ostream& ostr) const;
    /// \}

  private:
    /// Accessors.
    /// \{
    template <unsigned N, unsigned D_> friend class face_handle;

    template <unsigned N>
    face<N, D>& face_(unsigned face_id);

    template <unsigned N>
    const face<N, D>& face_(unsigned face_id) const;
    /// \}

    /// \brief connect two faces.
    ///
    /// \param f1 A face of dimension \p N
    /// \param f2 A face of dimension \p N + 1
    ///
    /// \pre \p N must be lower or equal to \p D.
    template <unsigned N>
    void connect_(const face_handle<N, D>& f1,
		  const face_handle<N + 1, D>& f2);
  };


  // FIXME: Move and renamed as mln::debug::println?
  /// Pretty print a complex.
  template <unsigned D>
  std::ostream&
  operator<<(std::ostream& ostr, const complex<D>& c);


  /*---------------------.
  | Faces of a complex.  |
  `---------------------*/

  /// The sets of n-faces of a complex are recursively aggregated as
  /// mixins.
  namespace internal
  {

    // FIXME: Factor common things here.

    /// \brief Recursive mixins of set of faces.
    /// \{
    template <unsigned N, unsigned D>
    struct faces_set_mixin : faces_set_mixin<N - 1, D>
    {
      std::vector< face<N, D> > faces_;

      /// Pretty-printing.
      /// \{
      /// Print the faces of dimension \p N.
      void print(std::ostream& ostr) const;
      /// Recursively print the faces of dimensions 0 to \p N
      /// (in ascending dimension).
      void print_rec_asc(std::ostream& ostr) const;
      /// \}
    };

    template <unsigned D>
    struct faces_set_mixin<0u, D>
    {
      std::vector< face<0u, D> > faces_;

      /// Pretty-printing.
      /// \{
      /// Print the faces of dimension 0.
      void print(std::ostream& ostr) const;
      void print_rec_asc(std::ostream& ostr) const;
      /// \}
    };

    template <unsigned D>
    struct faces_set_mixin<D, D> : faces_set_mixin<D - 1, D>
    {
      std::vector< face<D, D> > faces_;

      /// Pretty-printing.
      /// \{
      /// Print the faces of dimension \p D.
      void print(std::ostream& ostr) const;
      void print_rec_asc(std::ostream& ostr) const;
      /// \}
    };

    template <>
    struct faces_set_mixin<0u, 0u>
    {
      std::vector< face<0u, 0u> > faces_;

      /// Pretty-printing.
      /// \{
      /// Print the faces of dimension \p D.
      void print(std::ostream& ostr) const;
      void print_rec_asc(std::ostream& ostr) const;
      /// \}
    };
    /// \}

  } // end of namespace mln::internal



# ifndef MLN_INCLUDE_ONLY

  template <unsigned D>
  face_handle<0u, D>
  complex<D>::add_face()
  {
    /* FIXME: This is not thread-proof (these two lines should
       form an atomic section).  */
    internal::faces_set_mixin<0u, D>::faces_.push_back(face<0u, D>());
    unsigned id = internal::faces_set_mixin<0u, D>::faces_.size() - 1;

    return face_handle<0u, D>(*this, id);
  }

  template <unsigned D>
  template <unsigned N>
  face_handle<N + 1, D>
  complex<D>::add_face(const faces_set<N, D>& adjacent_faces)
  {
    // FIXME: Ensure ADJACENT_FACES are already part of the complex.

    face<N + 1, D> f;
    /* FIXME: This is not thread-proof (these two lines should
       form an atomic section).  */
    internal::faces_set_mixin<N + 1, D>::faces_.push_back(f);
    unsigned id = internal::faces_set_mixin<N + 1, D>::faces_.size() - 1;

    face_handle<N + 1, D> fh(*this, id);
    // Connect F and its ADJACENT_FACES.
    /* FIXME: Use <fonctional> or Milena's functors.  */
    for (typename std::vector< face_handle<N, D> >::const_iterator a =
	   adjacent_faces.faces().begin();
	 a != adjacent_faces.faces().end(); ++a)
      connect_(*a, fh);

    return fh;
  }

  template <unsigned D>
  template <unsigned N>
  face<N, D>&
  complex<D>::face_(unsigned face_id)
  {
    return internal::faces_set_mixin<N, D>::faces_[face_id];
  }

  template <unsigned D>
  template <unsigned N>
  const face<N, D>&
  complex<D>::face_(unsigned face_id) const
  {
    return internal::faces_set_mixin<N, D>::faces_[face_id];
  }

  template <unsigned D>
  template <unsigned N>
  void
  complex<D>::connect_(const face_handle<N, D>& f1,
		       const face_handle<N + 1, D>& f2)
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();

    f1.get().connect_higher_dim_face(f2);
    f2.get().connect_lower_dim_face(f1);
  }


  /*------------------.
  | Pretty-printing.  |
  `------------------*/

  template <unsigned D>
  std::ostream&
  operator<<(std::ostream& ostr, const complex<D>& c)
  {
    c.print(ostr);
    return ostr;
  }

  template <unsigned D>
  void
  complex<D>::print(std::ostream& ostr) const
  {
    internal::faces_set_mixin<D, D>::print_rec_asc(ostr);
  }

  template <unsigned D>
  template <unsigned N>
  void
  complex<D>::print_faces(std::ostream& ostr) const
  {
    // Ensure N is compatible with D.
    metal::bool_< N <= D >::check();

    internal::faces_set_mixin<N, D>::print(ostr);
  }


  namespace internal
  {

    // FIXME: Factor common things here.

    template <unsigned N, unsigned D>
    void
    faces_set_mixin<N, D>::print_rec_asc(std::ostream& ostr) const
    {
      faces_set_mixin<N - 1, D>::print_rec_asc(ostr);
      print(ostr);
    }

    template <unsigned D>
    void
    faces_set_mixin<0u, D>::print_rec_asc(std::ostream& ostr) const
    {
      print(ostr);
    }

    template <unsigned D>
    void
    faces_set_mixin<D, D>::print_rec_asc(std::ostream& ostr) const
    {
      faces_set_mixin<D - 1, D>::print_rec_asc(ostr);
      print(ostr);
    }

    void
    faces_set_mixin<0u, 0u>::print_rec_asc(std::ostream& ostr) const
    {
      print(ostr);
    }

    template <unsigned N, unsigned D>
    void
    faces_set_mixin<N, D>::print(std::ostream& ostr) const
    {
      ostr << "Faces of dimension " << N
	   << " and their ajacent faces of dimension "
	   << N - 1 << " and "
	   << N + 1 << std::endl;
      for (unsigned f = 0; f < faces_.size(); ++f)
	{
	  ostr << "  " << f << ":  dim " << N - 1 << ": { ";
	  for (typename std::vector< face_handle<N - 1, D> >::const_iterator l =
		 faces_[f].lower_dim_faces_.begin();
	       l != faces_[f].lower_dim_faces_.end();
	       ++l)
	    ostr << l->face_id_ << " ";
	  ostr << "},  dim " << N + 1 << ": { ";
	  for (typename std::vector< face_handle<N + 1, D> >::const_iterator h =
		 faces_[f].higher_dim_faces_.begin();
	       h != faces_[f].higher_dim_faces_.end();
	       ++h)
	    ostr << h->face_id_ << " ";
	  ostr << "}";
	  ostr << std::endl;
	}
    }

    template <unsigned D>
    void
    faces_set_mixin<0u, D>::print(std::ostream& ostr) const
    {
      // FIXME: Much could be factored with the previous routine.
      const unsigned N = 0u;
      ostr << "Faces of dimension " << N
	   << " and their ajacent faces of dimension "
	   << N + 1 << std::endl;
      for (unsigned f = 0; f < faces_.size(); ++f)
	{
	  ostr << "  " << f << ":  dim " << N + 1 << ": { ";
	  for (typename std::vector< face_handle<N + 1, D> >::const_iterator h =
		 faces_[f].higher_dim_faces_.begin();
	       h != faces_[f].higher_dim_faces_.end();
	       ++h)
	    ostr << h->face_id_ << " ";
	  ostr << "}";
	  ostr << std::endl;
	}
    }

    template <unsigned D>
    void
    faces_set_mixin<D, D>::print(std::ostream& ostr) const
    {
      // FIXME: Much could be factored with the previous routines.
      const unsigned N = D;
      ostr << "Faces of dimension " << N
	   << " and their ajacent faces of dimension "
	   << N - 1 << std::endl;
      for (unsigned f = 0; f < faces_.size(); ++f)
	{
	  ostr << "  " << f << ":  dim " << N - 1 << ": { ";
	  for (typename std::vector< face_handle<N - 1, D> >::const_iterator l =
		 faces_[f].lower_dim_faces_.begin();
	       l != faces_[f].lower_dim_faces_.end();
	       ++l)
	    ostr << l->face_id_ << " ";
	  ostr << "}";
	  ostr << std::endl;
	}
    }

    void
    faces_set_mixin<0u, 0u>::print(std::ostream& ostr) const
    {
      // FIXME: Much could be factored with the previous routines.
      const unsigned N = 0u;
      ostr << "Faces of dimension " << N << std::endl;
      for (unsigned f = 0; f < faces_.size(); ++f)
	ostr << "  " << f << std::endl;
    }

  } // end of namespace mln::internal

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_COMPLEX_HH
