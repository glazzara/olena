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

#ifndef OLN_CORE_INTERNAL_F_GRID_TO_BOX_HH
# define OLN_CORE_INTERNAL_F_GRID_TO_BOX_HH

# include <oln/core/concept/grid.hh>


#define oln_f_grid_to_box(G) typename oln::internal::f_grid_to_box_< G >::ret


namespace oln
{

  /// \{
  /// Forward declarations.

  // Grid types.

  struct grid1d;

  struct grid2d;
  struct grid2d_hex;
  struct grid2d_tri;

  // FIXME: 3D...

  // Box types.

  struct box1d;
  struct box2d;
  template <typename P> class gen_box;

  /// \}



  namespace internal
  {

    // Fwd decl.

    template <typename G> struct f_grid_to_;


    /// \{
    /// Definitions.

    template <typename G>
    struct grid_to_box__;

    template <>
    struct grid_to_box__< grid1d >
    {
      typedef box1d ret;
    };

    template <>
    struct grid_to_box__< grid2d >
    {
      typedef box2d ret;
    };

    // FIXME: 2D hex/tri, 3D...

    /// \}


    template <typename G>
    struct f_grid_to_box_ : private mlc::assert_< mlc_is_a(G, Grid) >,
			    public grid_to_box__< G >
		      
    {
    };

  } // end of namespace oln::internal

} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_F_GRID_TO_BOX_HH
