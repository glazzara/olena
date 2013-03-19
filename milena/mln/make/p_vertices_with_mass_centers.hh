// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_P_VERTICES_WITH_MASS_CENTERS_HH
# define MLN_MAKE_P_VERTICES_WITH_MASS_CENTERS_HH

/// \file
///
/// Create a p_vertices using region mass centers as vertex site.
///
/// \sa vertex_image, p_vertices


# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>
# include <mln/core/site_set/p_vertices.hh>

# include <mln/labeling/compute.hh>

# include <mln/accu/center.hh>

# include <mln/fun/i2v/array.hh>


namespace mln
{

  namespace make
  {

    /// Construct a p_vertices from a watershed image and a region adjacency
    /// graph (RAG). Map each graph vertex to the mass center of its
    /// corresponding region.
    ///
    /// \param wst_    A watershed image.
    /// \param g_      A region adjacency graph.
    ///
    /// \return A p_vertices.
    ///
    /// \sa edge_image, vertex_image, p_vertices, p_edges, make::region_adjacency_graph
    //
    template <typename W, typename G>
    inline
    p_vertices<G, fun::i2v::array<mln_site(W)> >
    p_vertices_with_mass_centers(const Image<W>& wst_,
				 const Graph<G>& g_);


# ifndef MLN_INCLUDE_ONLY


    template <typename W, typename G>
    inline
    p_vertices<G, fun::i2v::array<mln_site(W)> >
    p_vertices_with_mass_centers(const Image<W>& wst_,
				 const Graph<G>& g_)
    {
      mln_trace("make::p_vertices_with_mass_centers");

      const W& wst = exact(wst_);
      const G& g = exact(g_);
      mln_precondition(wst.is_valid());
      mln_precondition(g.is_valid());

      typedef mln_site(W) P;
      typedef fun::i2v::array<P> vertex_sites_t;

      // nbasins does not include the background label.
      mln_value(W) nbasins = g.v_nmax() - 1;
      vertex_sites_t vertex_sites;
      convert::from_to(labeling::compute(accu::center<P>(), wst, nbasins),
		       vertex_sites);

      p_vertices<G, vertex_sites_t> pv(g, vertex_sites);

      return pv;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_P_VERTICES_WITH_MASS_CENTERS_HH
