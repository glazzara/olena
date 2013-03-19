// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_MORPHO_LINE_GRADIENT_HH
# define MLN_MORPHO_LINE_GRADIENT_HH

/// \file
///
/// Conversions to mln::line_graph_image.

# include <functional>

# include <map>
# include <vector>

# include <mln/math/abs.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/edge_image.hh>
# include <mln/core/alias/window2d.hh>

# include <mln/util/graph.hh>
# include <mln/util/site_pair.hh>


// FIXME: Generalize to other (input) images as well (image1d,
// image3d, etc.).

namespace mln
{

  namespace morpho
  {

    /*! \brief Create a line graph image representing the gradient
     * norm of a mln::image2d.
     *
     * \ingroup mlnmorpho
     */
    /* FIXME: Currently, the adjacency is set to 4-c and cannot be
       changed.  */
    template <typename V>
    edge_image<util::site_pair<point2d>, V, util::graph>
    line_gradient(const mln::image2d<V>& ima);


# ifndef MLN_INCLUDE_ONLY

    template <typename V>
    edge_image<util::site_pair<point2d>, V, util::graph>
    line_gradient(const mln::image2d<V>& ima)
    {
      mln_trace("morpho::line_gradient");
      mln_precondition(ima.is_valid());

      // FIXME: Precondition: Ensure the image is scalar.
      util::graph g;

      // Vertices.
      image2d<unsigned> vpsite(ima.domain());
      fun::i2v::array<point2d> fv2p(ima.domain().nsites());
      fun::i2v::array<V> vertex_values(ima.domain().nsites());

      mln_fwd_piter(image2d<V>) p(ima.domain());
      for_all (p)
      {
	g.add_vertex();
	unsigned id = g.v_nmax() - 1;
	vpsite(p) = id;
	fv2p(id) = p;
      }

      // Edges.
      // FIXME: The creation of this window should be generic.
      window2d next_c4_win;
      next_c4_win.insert(0, 1).insert(1, 0);
      typedef fun::i2v::array<V> edge_values_t;
      typedef fun::i2v::array< util::site_pair<point2d> > edge_sites_t;
      edge_values_t edge_values;
      edge_sites_t edge_sites;
      mln_fwd_qiter_(window2d) q(next_c4_win, p);
      for_all (p)
	for_all (q)
	if (ima.domain().has(q))
	  {
	    g.add_edge(vpsite(p), vpsite(q));
	    // The computed value is a norm of the gradient between P and Q.
	    edge_values.append(math::abs(ima(p) - ima(q)));
	    edge_sites.append(util::site_pair<point2d>(p, q));
	  }

      edge_image<util::site_pair<point2d>, V, util::graph>
	lg_ima(g, edge_sites, edge_values);

      return lg_ima;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_LINE_GRADIENT_HH
