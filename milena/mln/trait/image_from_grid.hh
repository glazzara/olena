// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_TRAIT_IMAGE_FROM_GRID_HH
# define MLN_TRAIT_IMAGE_FROM_GRID_HH

/// \file
///
/// Definition of the "image from mesh" trait.
///
/// This file shall not include the concrete image types it returns;
/// otherwise we get circular dependencies.  As a consequence the user
/// shall include the appropriate image type she deals with.

# include <mln/core/grids.hh>


# define mln_image_from_grid(G, V) typename mln::trait::image_from_grid< G, V >::ret



namespace mln
{


  // Fwd decls.
  template <typename T> struct image1d;
  template <typename T> class image2d;
  template <typename T> struct image3d;


  namespace trait
  {

    template <typename M, typename V> struct image_from_grid;

    template <typename V>
    struct image_from_grid< grid::tick, V >
    {
      typedef image1d<V> ret;
    };

    template <typename V>
    struct image_from_grid< grid::square, V >
    {
      typedef image2d<V> ret;
    };

    template <typename V>
    struct image_from_grid< grid::cube, V >
    {
      typedef image3d<V> ret;
    };


    // FIXME: Return other image types than imagend when size trait is not regular...

    // FIXME: Add cases when the mesh is not a grid...


  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_IMAGE_FROM_GRID_HH
