// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_P_VERTICES_WITH_ACCU_HH
# define MLN_MAKE_P_VERTICES_WITH_ACCU_HH

/// \file mln/make/p_vertices_with_mass_centers.hh
///
/// Create a p_vertices using region mass centers as vertex site.
///
/// \sa vertex_image, p_vertices


# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>
# include <mln/core/site_set/p_vertices.hh>

# include <mln/labeling/compute.hh>

# include <mln/accu/center.hh>
# include <mln/util/couple.hh>
# include <mln/fun/i2v/array.hh>


namespace mln
{

  namespace make
  {

    /// Construct a p_vertices from a watershed image and a region adjacency
    /// graph (RAG). Map each graph vertex with an accumulator on its
    /// corresponding region.
    ///
    /// \param wst_             A watershed image.
    /// \param nbasins          The number of basins.
    /// \param g_               A region adjacency graph.
    /// \param accumulator      An accumulator to label vertices.
    ///
    /// \return A p_vertices.
    ///
    /// \sa edge_image, vertex_image, p_vertices, p_edges,
    /// make::region_adjacency_graph
    //
    template <typename W, typename G, typename A>
    inline
    p_vertices<G, fun::i2v::array<mln_site(W)> >
    p_vertices_with_with_accu(const Image<W>& wst_,
                              const mln_value(W)& nbasins,
                              const Graph<G>& g_,
                              const A& accumulator);


# ifndef MLN_INCLUDE_ONLY


    template <typename W, typename G, typename A>
    inline
    p_vertices<G, fun::i2v::array<util::couple<mln_site(W), int> > >
    p_vertices_with_accu(const Image<W>& wst_,
                         const mln_value(W)& nbasins,
                         const Graph<G>& g_,
                         const A& accumulator)
    {
      trace::entering("make::p_vertices_with_accu");

      const W& wst = exact(wst_);
      const G& g = exact(g_);
      mln_precondition(wst.is_valid());
      mln_precondition(g.is_valid());

      typedef mln_site(W) P;
      typedef fun::i2v::array<util::couple<P, int> > vertex_sites_t;

      vertex_sites_t vertex_sites;
      convert::from_to(labeling::compute(accumulator, wst, nbasins),
		       vertex_sites);

      p_vertices<G, vertex_sites_t> pv(g, vertex_sites);

      trace::exiting("make::p_vertices_with_accu");
      return pv;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_P_VERTICES_WITH_ACCU_HH
