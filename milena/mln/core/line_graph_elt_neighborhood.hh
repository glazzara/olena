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

#ifndef MLN_CORE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH
# define MLN_CORE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH

/// \file mln/core/line_graph_elt_neighborhood.hh
/// \brief Definition of the elementary ``neighborhood'' on a line graph.

/* FIXME: Have a consistent naming: we have window (without '_') but
   point_, neighb_, etc.  */

/* FIXME: Factor those classes:
   - mln::graph_elt_window
   - mln::graph_elt_neighborhood
   - mln::line_graph_elt_window
   - mln::line_graph_elt_neighborhood.  */

/* FIXME: Due to the poor interface of mln::p_line_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

# include <set>

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/line_graph_psite.hh>
# include <mln/core/line_graph_neighborhood_piter.hh>


namespace mln
{
  // Fwd decls.
  template <typename P, typename N> class line_graph_neighborhood_fwd_piter;
  template <typename P, typename N> class line_graph_neighborhood_bkd_piter;


  /// \brief Elementary neighborhood on line graph class.
  template <typename P>
  class line_graph_elt_neighborhood
    : public Neighborhood< line_graph_elt_neighborhood<P> >
  {
    typedef line_graph_elt_neighborhood<P> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of point corresponding to the neighborhood.
    typedef P point;
    /// The type of psite corresponding to the neighborhood.
    typedef line_graph_psite<P> psite;
    // The type of the set of neighbors (edge ids adjacent to the
    // reference psite).
    typedef std::set<util::edge_id> sites_t;

    // FIXME: This is a dummy value.
    typedef void dpoint;

    /// \brief Point_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the ordering of edges.
    typedef line_graph_neighborhood_fwd_piter<P, self_> fwd_niter;

    /// \brief Point_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the reverse ordering of edges.
    typedef line_graph_neighborhood_bkd_piter<P, self_> bkd_niter;

    /// The default qiter type.
    typedef fwd_niter niter;
    /// \}

    /// Services for iterators.
    /// \{
    /// Compute the set of sites for this neighborhood around \a piter.
    template <typename Piter>
    void compute_sites_(Point_Iterator<Piter>& piter) const;
    /// \}
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  template <typename Piter>
  inline
  void
  line_graph_elt_neighborhood<P>::compute_sites_(Point_Iterator<Piter>& piter_) const
  {
    Piter& piter = exact(piter_);
    util::edge_id ref_edge_id = piter.p_ref().id();
    sites_t& sites = piter.sites();
    sites.clear();
    /* FIXME: Move this computation out of the neighborhood. In fact,
       this should be a service of the graph, also proposed by the
       p_line_graph.  */
    // Ajacent edges connected through vertex 1.
    util::vertex_id id1 = piter.p_ref().first_id();
    const util::vertex<P>& vertex1 = piter.plg().gr_->vertex(id1);
    for (std::vector<util::edge_id>::const_iterator e =
	   vertex1.edges.begin(); e != vertex1.edges.end(); ++e)
      // We explicitly enforce that the reference piter edge id is
      // *not* inserted into SITES.
      if (*e != ref_edge_id)
	sites.insert(*e);
    // Ajacent edges connected through vertex 2.
    util::vertex_id id2 = piter.p_ref().second_id();
    const util::vertex<P>& vertex2 = piter.plg().gr_->vertex(id2);
    for (std::vector<util::edge_id>::const_iterator e =
	   vertex2.edges.begin(); e != vertex2.edges.end(); ++e)
      // Same remark as above.
      if (*e != ref_edge_id)
	sites.insert(*e);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH
