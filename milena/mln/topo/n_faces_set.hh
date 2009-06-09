// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_TOPO_N_FACES_SET_HH
# define MLN_TOPO_N_FACES_SET_HH

/// \file
/// \brief Set of n-face of a complex.

#include <vector>

#include <mln/core/contract.hh>
#include <mln/topo/algebraic_n_face.hh>

// FIXME: Rename as algebraic_n_faces_set?


namespace mln
{

  namespace topo
  {

    // Forward declaration.
    template <unsigned D> class complex;


    /*------------------------------------.
    | Set of (algebraic) n-face handles.  |
    `------------------------------------*/

    /// \brief Set of face handles of dimension \p N.
    template <unsigned N, unsigned D>
    class n_faces_set
    {
    public:
      /// \brief Append an algebraic face \a f to the set.
      void add(const algebraic_n_face<N, D>& f);

      /// \brief Reserve \a n cells in the set.
      ///
      /// This methods does not change the content of \a faces_; it
      /// only pre-allocate memory.  Method reserve is provided for
      /// efficiency purpose, and its use is completely optional.
      void reserve(size_t n);

      /// The type of the set of face handles.
      typedef std::vector< algebraic_n_face<N, D> > faces_type;

      /// \brief Accessors.
      ///
      /// Return the set of handles.
      /// \{
      const faces_type& faces() const;
      /// \}

    private:
      friend class complex<D>;

      /// The set of face handles.
      faces_type faces_;
    };


    /*-----------------------.
    | Construction helpers.  |
    `-----------------------*/

    /* FIXME: We can probably reduce the number of operators, given
       the fact that ``a - b'' is equivalent to ``a + (-b)''.  */

    /// Addition.
    /// \{
    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator+(const algebraic_n_face<N, D>& f1,
	      const algebraic_n_face<N, D>& f2);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator+(const algebraic_n_face<N, D>& f1, const n_face<N, D>& f2);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator+(const n_face<N, D>& f1, const algebraic_n_face<N, D>& f2);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator+(const n_face<N, D>& f1, const n_face<N, D>& f2);


    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator+(const n_faces_set<N, D>& fs, const algebraic_n_face<N, D>& f);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator+(const n_faces_set<N, D>& fs, const n_face<N, D>& f);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>&
    operator+=(n_faces_set<N, D>& fs, const algebraic_n_face<N, D>& f);
    /// \}

    /// Subtraction.
    /// \{
    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator-(const algebraic_n_face<N, D>& f1,
	      const algebraic_n_face<N, D>& f2);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator-(const algebraic_n_face<N, D>& f1, const n_face<N, D>& f2);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator-(const n_face<N, D>& f1, const algebraic_n_face<N, D>& f2);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator-(const n_face<N, D>& f1, const n_face<N, D>& f2);


    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator-(const n_faces_set<N, D>& fs, const algebraic_n_face<N, D>& f);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>
    operator-(const n_faces_set<N, D>& fs, const n_face<N, D>& f);

    template <unsigned N, unsigned D>
    n_faces_set<N, D>&
    operator-=(n_faces_set<N, D>& fs, const algebraic_n_face<N, D>& f);
    /// \}



# ifndef MLN_INCLUDE_ONLY

    /*------------------------------------.
    | Set of (algebraic) n-face handles.  |
    `------------------------------------*/

    template <unsigned N, unsigned D>
    inline
    void
    n_faces_set<N, D>::add(const algebraic_n_face<N, D>& f)
    {
      // Check consistency.
      if (!faces_.empty())
	mln_precondition(faces_.front().cplx() == f.cplx());
      faces_.push_back(f);
    }

    template <unsigned N, unsigned D>
    inline
    void
    n_faces_set<N, D>::reserve(size_t n)
    {
      faces_.reserve(n);
    }

    template <unsigned N, unsigned D>
    inline
    const std::vector< algebraic_n_face<N, D> >&
    n_faces_set<N, D>::faces() const
    {
      return faces_;
    }

    /*-----------------------.
    | Construction helpers.  |
    `-----------------------*/

    // ---------- //
    // Addition.  //
    // ---------- //

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator+(const algebraic_n_face<N, D>& f1,
	      const algebraic_n_face<N, D>& f2)
    {
      n_faces_set<N, D> fs;
      fs.add(f1);
      fs.add(f2);
      return fs;
    }

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator+(const algebraic_n_face<N, D>& f1, const n_face<N, D>& f2)
    {
      n_faces_set<N, D> fs;
      fs.add(f1);
      fs.add(make_algebraic_n_face(f2, true));
      return fs;
    }

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator+(const n_face<N, D>& f1, const algebraic_n_face<N, D>& f2)
    {
      n_faces_set<N, D> fs;
      fs.add(make_algebraic_n_face(f1, true));
      fs.add(f2);
      return fs;
    }

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator+(const n_face<N, D>& f1, const n_face<N, D>& f2)
    {
      n_faces_set<N, D> fs;
      fs.add(make_algebraic_n_face(f1, true));
      fs.add(make_algebraic_n_face(f2, true));
      return fs;
    }


    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator+(const n_faces_set<N, D>& fs, const algebraic_n_face<N, D>& f)
    {
      n_faces_set<N, D> fs2(fs);
      fs2.add(f);
      return fs2;
    }

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator+(const n_faces_set<N, D>& fs, const n_face<N, D>& f)
    {
      n_faces_set<N, D> fs2(fs);
      fs2.add(make_algebraic_n_face(f, true));
      return fs2;
    }

    template <unsigned N, unsigned D>
    n_faces_set<N, D>&
    operator+=(n_faces_set<N, D>& fs, const algebraic_n_face<N, D>& f)
    {
      fs.add(f);
      return fs;
    }

    // ------------- //
    // Subtraction.  //
    // ------------- //

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator-(const algebraic_n_face<N, D>& f1,
	      const algebraic_n_face<N, D>& f2)
    {
      n_faces_set<N, D> fs;
      fs.add(f1);
      fs.add(-f2);
      return fs;
    }

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator-(const algebraic_n_face<N, D>& f1, const n_face<N, D>& f2)
    {
      n_faces_set<N, D> fs;
      fs.add(f1);
      fs.add(make_algebraic_n_face(f2, false));
      return fs;
    }

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator-(const n_face<N, D>& f1, const algebraic_n_face<N, D>& f2)
    {
      n_faces_set<N, D> fs;
      fs.add(make_algebraic_n_face(f1, true));
      fs.add(-f2);
      return fs;
    }

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator-(const n_face<N, D>& f1, const n_face<N, D>& f2)
    {
      n_faces_set<N, D> fs;
      fs.add(make_algebraic_n_face(f1, true));
      fs.add(make_algebraic_n_face(f2, false));
      return fs;
    }


    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator-(const n_faces_set<N, D>& fs, const algebraic_n_face<N, D>& f)
    {
      n_faces_set<N, D> fs2(fs);
      fs2.add(-f);
      return fs2;
    }

    template <unsigned N, unsigned D>
    inline
    n_faces_set<N, D>
    operator-(const n_faces_set<N, D>& fs, const n_face<N, D>& f)
    {
      n_faces_set<N, D> fs2(fs);
      fs2.add(make_algebraic_n_face(f, false));
      return fs2;
    }

    template <unsigned N, unsigned D>
    n_faces_set<N, D>&
    operator-=(n_faces_set<N, D>& fs, const algebraic_n_face<N, D>& f)
    {
      fs.add(-f);
      return fs;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_N_FACES_SET_HH
