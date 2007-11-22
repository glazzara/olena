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

#ifndef MLN_DRAW_MESH_HH
# define MLN_DRAW_MESH_HH

/*! \file mln/draw/mesh.hh
 *
 * \brief Draw an image from type mesh_image.
 *
 */

# include <mln/pw/image.hh>
# include <mln/level/fill.hh>
# include <mln/draw/line.hh>
# include <mln/core/mesh_p.hh>
# include <mln/core/mesh_image.hh>

namespace mln
{

  namespace draw
  {

    /*! Draw an image \p ima from a mesh_p \p m, with value \p node_v
     *  for nodes, value \p link_v for links and 0 for the background.
     *
     * \param[in,out] ima The image to be drawn.
     * \param[in] m The mesh_p which contains nodes and links positions.
     * \param[in] node_v The value to assign to pixels which contains nodes.
     * \param[in] link_v The value to assign to pixels which contains links.
     *
     */
    template <typename I, typename P>
    void
    mesh(Image<I>& ima, const mesh_p<P>& m,
	 mln_value(I) node_v,
	 mln_value(I) link_v);

    /*! Draw an image \p ima from a mesh_image \p mesh.
     *  The background is filled to value 0.
     *
     * \param[in,out] ima The image to be drawn.
     * \param[in] mesh The mesh_image which contains nodes, links
     * positions and the values of it.
     *
     */
    template <typename I,  typename P, typename V>
    void
    mesh(Image<I>& ima, const mesh_image<P, V>& mesh);

# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename P>
    void
    mesh(Image<I>& ima, const mesh_p<P>& m,
	 mln_value(I) node_v,
	 mln_value(I) link_v)
    {
      level::fill(ima, 0);

      for (unsigned i = 0; i < m.gr_.nb_link_; ++i)
	line (exact(ima),
	      m.loc_[m.gr_.links_[i]->node1],
	      m.loc_[m.gr_.links_[i]->node2],
	      link_v);

      for (unsigned i = 0; i < m.gr_.nb_node_; ++i)
 	exact(ima)(m.loc_[i]) = node_v;
    }

    template <typename I,  typename P, typename V>
    void
    mesh(Image<I>& ima, const mesh_image<P, V>& mesh)
    {
      level::fill(ima, 0);

      for (unsigned i = 0; i < mesh.domain().gr_.nb_link_; ++i)
	line (exact(ima),
	      mesh.access_location_link_node1 (i),
	      mesh.access_location_link_node2 (i),
	      1);

      for (unsigned i = 0; i < mesh.domain().gr_.nb_node_; ++i)
 	exact(ima)(mesh.domain().loc_[i]) = mesh.data_values ()[i];
    }

# endif // ! MLN_INCLUDE_ONLY

   } // end of namespace mln::draw

} // end of namespace mln

#endif // MLN_MESH_PSITE_HH
