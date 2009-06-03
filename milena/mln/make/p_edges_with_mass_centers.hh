// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MAKE_P_EDGES_WITH_MASS_CENTERS_HH
# define MLN_MAKE_P_EDGES_WITH_MASS_CENTERS_HH

/// \file mln/make/p_edges_with_mass_centers.hh
///
/// Construct a p_edges from a watershed image and a region adjacency
/// graph (RAG). Map each graph edge to a pair of mass centers of two
/// adjacent regions.
///
/// \sa edge_image, p_edges


# include <mln/core/concept/image.hh>
# include <mln/core/concept/graph.hh>
# include <mln/core/site_set/p_edges.hh>

# include <mln/labeling/compute.hh>

# include <mln/accu/center.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/util/site_pair.hh>


namespace mln
{

  namespace make
  {

    /// Construct a p_edges from a watershed image and a region adjacency
    /// graph (RAG). Map each graph edge to a pair of mass centers of two
    /// adjacent regions.
    ///
    /// \param wst_    A watershed image.
    /// \param g_      A region adjacency graph.
    ///
    /// \return A p_edges.
    ///
    /// \sa edge_image, p_edges, make::region_adjacency_graph
    //
    template <typename W, typename G>
    inline
    p_edges<G, fun::i2v::array<util::site_pair<mln_site(W)> > >
    p_edges_with_mass_centers(const Image<W>& wst_,
			      const Graph<G>& g_);


# ifndef MLN_INCLUDE_ONLY


    template <typename W, typename G>
    inline
    p_edges<G, fun::i2v::array<util::site_pair<mln_site(W)> > >
    p_edges_with_mass_centers(const Image<W>& wst_,
			      const Graph<G>& g_)
    {
      trace::entering("make::p_edges_with_mass_centers");

      const W& wst = exact(wst_);
      const G& g = exact(g_);
      mln_precondition(wst.is_valid());
      mln_precondition(g.is_valid());

      typedef mln_site(W) P;

      // nbasins does not include the background label.
      mln_value(W) nbasins = g.v_nmax() - 1;
      util::array<mln_vec(P)>
	mass_centers = labeling::compute(accu::center<P>(), wst, nbasins);

      typedef fun::i2v::array< util::site_pair<P> > edge_sites_t;
      edge_sites_t edge_sites(g.e_nmax());

      mln_edge_iter(G) e(g);
      for_all(e)
	edge_sites(e.id()) = util::site_pair<P>(mass_centers[e.v1()],
						mass_centers[e.v2()]);

      p_edges<G, edge_sites_t> pe(g, edge_sites);

      trace::exiting("make::p_edges_with_mass_centers");
      return pe;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_P_EDGES_WITH_MASS_CENTERS_HH
