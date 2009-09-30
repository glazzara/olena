// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOODS_HH
# define MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOODS_HH

/// \file
/// \brief Definitions of some neighborhoods of complex images.

# include <mln/core/internal/complex_neighborhood_base.hh>
# include <mln/core/image/complex_windows.hh>

# include <mln/topo/complex_iterators.hh>

/** \brief Generate a complex neighborhood class (based on a window).

    \param Nbh  The name of the class to generate.
    \param Win  The underlying window (with no parameter).

    The generated class inherits from

      mln::internal::complex_neighborhood_base< Win <D, G>, Nbh <D, G> >

    where D and G are template parameters of the generated Nbh class.  */
# define mln_gen_complex_neighborhood(Nbh, Win)				\
  template <unsigned D, typename G>					\
  struct Nbh								\
    : internal::complex_neighborhood_base< Win <D, G>, Nbh <D, G> >	\
  {									\
  }


namespace mln
{

  /// \brief Neighborhood centered on an n-face of complex returning
  /// its adjacent (n-1)-faces.
  mln_gen_complex_neighborhood(complex_lower_neighborhood,
			       complex_lower_window);

  /// \brief Neighborhood centered on an n-face of complex returning
  /// its adjacent (n+1)-faces.
  mln_gen_complex_neighborhood(complex_higher_neighborhood,
			       complex_higher_window);

  /// \brief Neighborhood centered on an n-face of complex returning
  /// its adjacent (n-1)-faces and (n+1)-faces.
  mln_gen_complex_neighborhood(complex_lower_higher_neighborhood,
			       complex_lower_higher_window);

  /// \brief Neighborhood centered on an n-face of complex returning
  /// the n-faces sharing an (n-1)-face with the center n-face.
  mln_gen_complex_neighborhood(complex_lower_dim_connected_n_face_neighborhood,
			       complex_lower_dim_connected_n_face_window);

  /// \brief Neighborhood centered on an n-face of complex returning
  /// the n-faces sharing an (n+1)-face with the center n-face.
  mln_gen_complex_neighborhood(complex_higher_dim_connected_n_face_neighborhood,
			       complex_higher_dim_connected_n_face_window);

  /// \brief Neighborhood centered on an n-face of complex returning
  /// the m-faces transitively adjacent to this center n-face.
  mln_gen_complex_neighborhood(complex_m_face_neighborhood,
			       complex_m_face_window);

} // end of namespace mln

# undef mln_gen_complex_neighborhood

#endif // ! MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOODS_HH
