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

#ifndef MLN_CORE_ALIAS_COMPLEX_GEOMETRY_HH
# define MLN_CORE_ALIAS_COMPLEX_GEOMETRY_HH

/// \file
///
/// Definition of mln::geom::complex_geometry aliases.

# include <mln/geom/complex_geometry.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/point3d.hh>


namespace mln
{

  /*!
    \class discrete_plane_1complex_geometry
    \headerfile <>

    \brief Type alias for the geometry of a 1-complex (e.g., a
    graph) located in a discrete 2-dimensional plane (with integer
    coordinates).
  */
  /// \cond ALIAS
  typedef mln::geom::complex_geometry<1, point2d>
  discrete_plane_1complex_geometry;
  /// \endcond

  /*!
    \class discrete_plane_2complex_geometry
    \headerfile <>

    \brief Type alias for the geometry of a 2-complex located in a
    discrete 2-dimensional plane (with integer coordinates).
  */
  /// \cond ALIAS
  typedef mln::geom::complex_geometry<2, point2d>
  discrete_plane_2complex_geometry;
  /// \endcond

  /*!
    \class space_2complex_geometry
    \headerfile <>

    \brief Type alias for the geometry of a 2-complex located in a
    3-dimensional space (with floating-point coordinates).
  */
  /// \cond ALIAS
  typedef mln::geom::complex_geometry<2, point3df> space_2complex_geometry;
  /// \endcond

} // end of namespace mln


#endif // ! MLN_CORE_ALIAS_COMPLEX_GEOMETRY_HH
