// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_CORE_INTERNAL_F_GRID_COORD_TO_POINT_HH
# define OLN_CORE_INTERNAL_F_GRID_COORD_TO_POINT_HH

# include <oln/core/concept/grid.hh>


# define oln_f_grid_coord_to_point(G, C) \
typename oln::internal::f_grid_coord_to_point_< G, C >::ret


namespace oln
{

  /// \{
  /// Forward declarations.

  // Grid types.

  struct grid1d;
  struct grid2d; // FIXME: hex, tri...
  struct grid3d;

  // Point types.

  struct point1d;
  struct point2d;
  struct point3d;
  template <typename G, typename C> class point;

  /// \}



  namespace internal
  {

    template <typename G, typename C>
    struct grid_coord_to_point__;


    /// \{
    /// Definitions.

    template <typename G, typename C>
    struct grid_coord_to_point__
    {
      typedef oln::point<G, C> ret;
    };

    template <>
    struct grid_coord_to_point__< grid1d, int >
    {
      typedef point1d ret;
    };

    template <>
    struct grid_coord_to_point__< grid2d, int >
    {
      typedef point2d ret;
    };

    // FIXME: 2D hex/tri...

    template <>
    struct grid_coord_to_point__< grid3d, int >
    {
      typedef point3d ret;
    };

    /// \}


    template <typename G, typename C>
    struct f_grid_coord_to_point_ : private mlc::assert_< mlc_is_a(G, Grid) >,
				    public grid_coord_to_point__< G, C >
    {
    };

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_F_GRID_COORD_TO_POINT_HH
