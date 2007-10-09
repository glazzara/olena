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

#ifndef MLN_TRAIT_IMAGE_FROM_MESH_HH
# define MLN_TRAIT_IMAGE_FROM_MESH_HH

/*! \file mln/trait/image_from_mesh.hh
 *
 * \brief Definition of the "image from mesh" trait.
 */

# include <mln/core/grids.hh>


# define mln_image_from_mesh(I, V) typename mln::trait::image_from_mesh< I, V >::ret



namespace mln
{


  // Fwd decls.
  template <typename T> struct image1d;
  template <typename T> struct image2d;
  template <typename T> struct image3d;


  namespace trait
  {

    template <typename M, typename V> struct image_from_mesh;

    template <typename V>
    struct image_from_mesh< grid::tick, V >
    {
      typedef image1d<V> ret;
    };

    template <typename V>
    struct image_from_mesh< grid::square, V >
    {
      typedef image2d<V> ret;
    };

    template <typename V>
    struct image_from_mesh< grid::cube, V >
    {
      typedef image3d<V> ret;
    };


  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_TRAIT_IMAGE_FROM_MESH_HH
