// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_HH
# define MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_HH

/// \file mln/core/image/graph_elt_neighborhood.hh
/// \brief Definition of the elementary ``neighborhood'' on a graph.

/* FIXME: Factor those classes:
   - mln::graph_elt_window
   - mln::graph_elt_neighborhood
   - mln::line_graph_elt_window
   - mln::line_graph_elt_neighborhood.

   See https://trac.lrde.org/olena/ticket/139.  */

# include <set>

# include <mln/core/concept/neighborhood.hh>
# include <mln/util/internal/graph_vertex_psite.hh>
# include <mln/core/image/graph_neighborhood_piter.hh>

# include <mln/core/image/graph_elt_window.hh>


namespace mln
{
  // Fwd decls.
  template <typename G, typename F, typename N> class graph_neighborhood_fwd_piter;
  template <typename G, typename F, typename N> class graph_neighborhood_bkd_piter;


  /// \brief Elementary neighborhood on graph class.
  template <typename G, typename F>
  class graph_elt_neighborhood
    : public Neighborhood< graph_elt_neighborhood<G, F> >
  {
    typedef graph_elt_neighborhood<G, F> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of psite corresponding to the neighborhood.
    typedef internal::vertex_psite<G, F> psite;
    /// The type of site corresponding to the neighborhood.
    typedef mln_site(psite) site;
    // The type of the set of neighbors (vertex ids adjacent to the
    // reference psite).
    typedef std::set<unsigned> sites_t;

    /// \brief Site_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the ordering of vertices.
    typedef graph_neighborhood_fwd_piter<G, F, self_> fwd_niter;

    /// \brief Site_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the reverse ordering of vertices.
    typedef graph_neighborhood_bkd_piter<G, F, self_> bkd_niter;

    /// The default niter type.
    typedef fwd_niter niter;
    /// \}

    /// Conversions.
    /// \{
    /// The window type corresponding to this neighborhood.
    typedef graph_elt_window<G, F> window;
    /// Create a window corresponding to this neighborhood.
    window win() const;
    /// \}

    /// Services for iterators.
    /// \{
    /// Compute the set of sites for this neighborhood around \a piter.
    template <typename Piter>
    void compute_sites_(Site_Iterator<Piter>& piter) const;
    /// \}
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename G, typename F>
  inline
  graph_elt_window<G, F>
  graph_elt_neighborhood<G, F>::win() const
  {
    return graph_elt_window<G, F>();
  }

  template <typename G, typename F>
  template <typename Piter>
  inline
  void
  graph_elt_neighborhood<G, F>::compute_sites_(Site_Iterator<Piter>& piter_) const
  {
    Piter& piter = exact(piter_);
    unsigned central_vertex = piter.center().v().id();
    const G& g = piter.center().graph();

    sites_t& sites = piter.sites();
    sites.clear();

    // Adjacent vertices.
    for (unsigned i = 0; i < g.v_nmax_nbh_vertices(central_vertex); ++i)
      sites.insert(g.v_ith_nbh_vertex(central_vertex, i));
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_HH
