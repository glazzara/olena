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

#ifndef MLN_TOPO_FACE_DATA_HH
# define MLN_TOPO_FACE_DATA_HH

/// \file mln/topo/face_data.hh
/// \brief Data associated to the faces of a complex.
///
/// These data are actually lists of adjacent faces (of lower and
/// higher dimensions).

# include <vector>

# include <mln/topo/n_face.hh>


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



# ifndef MLN_INCLUDE_ONLY

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

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_FACE_DATA_HH
