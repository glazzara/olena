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

#ifndef MLN_TOPO_FACE_DATA_HH
# define MLN_TOPO_FACE_DATA_HH

/// \file
///
/// \brief Data associated to the faces of a complex.
///
/// These data are actually lists of adjacent faces (of lower and
/// higher dimensions).

# include <vector>

# include <mln/topo/algebraic_n_face.hh>


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

      template <unsigned N, unsigned D>
      struct lower_dim_adj_faces_if_dim_matches_;
      template <unsigned N, unsigned D>
      struct higher_dim_adj_faces_if_dim_matches_;
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
	typedef std::vector< algebraic_n_face<N - 1, D> > lower_dim_faces_type;

      public:
	void connect_lower_dim_face(const algebraic_n_face<N - 1, D>& f);

      private:
	friend struct mln::topo::internal::lower_dim_faces_set_mixin<N, D>;
	friend struct mln::topo::internal::lower_dim_adj_faces_if_dim_matches_<N, D>;
	friend lower_dim_faces_type
	mln::topo::n_face<N, D>::lower_dim_adj_faces() const;

	// FIXME: Rename as lower_dim_adj_faces_ (as well as related members).
	lower_dim_faces_type lower_dim_faces_;
      };

      template <unsigned N, unsigned D>
      class higher_dim_faces_data_mixin
      {
	typedef std::vector< algebraic_n_face<N + 1, D> > higher_dim_faces_type;

      public:
	void connect_higher_dim_face(const algebraic_n_face<N + 1, D>& f);

      private:
	friend struct mln::topo::internal::higher_dim_faces_set_mixin<N, D>;
	friend struct mln::topo::internal::higher_dim_adj_faces_if_dim_matches_<N, D>;
	friend higher_dim_faces_type
	mln::topo::n_face<N, D>::higher_dim_adj_faces() const;

	// FIXME: Rename as higher_dim_adj_faces_ (as well as related members).
	higher_dim_faces_type higher_dim_faces_;
      };
      /// \}

    } // end of namespace mln::topo::internal



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {
      template <unsigned N, unsigned D>
      inline
      void
      lower_dim_faces_data_mixin<N, D>::connect_lower_dim_face(const algebraic_n_face<N - 1, D>& f)
      {
	lower_dim_faces_.push_back(f);
      }

      template <unsigned N, unsigned D>
      inline
      void
      higher_dim_faces_data_mixin<N, D>::connect_higher_dim_face(const algebraic_n_face<N + 1, D>& f)
      {
	higher_dim_faces_.push_back(f);
      }

    } // end of namespace mln::topo::internal

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_FACE_DATA_HH
