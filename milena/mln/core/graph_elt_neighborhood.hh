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

#ifndef MLN_CORE_GRAPH_ELT_NEIGHBORHOOD_HH
# define MLN_CORE_GRAPH_ELT_NEIGHBORHOOD_HH

/// \file mln/core/graph_elt_neighborhood.hh
/// \brief Definition of the elementary ``neighborhood'' on a graph.

/* FIXME: Have a consistent naming: we have neighborhood (without '_')
   but point_, neighb_, etc.  */

/* FIXME: Factor those classes:
   - mln::graph_elt_window
   - mln::graph_elt_neighborhood
   - mln::line_graph_elt_window
   - mln::line_graph_elt_neighborhood.  */

/* FIXME: Due to the poor interface of mln::p_line_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/graph_psite.hh>
# include <mln/core/graph_neighborhood_piter.hh>


namespace mln
{
  // Fwd decls.
  template <typename P, typename N> class graph_neighborhood_fwd_piter;
  template <typename P, typename N> class graph_neighborhood_bkd_piter;


  /// Elementary neighborhood on graph class.
  template <typename P>
  class graph_elt_neighborhood
    : public Neighborhood< graph_elt_neighborhood<P> >
  {
    typedef graph_elt_neighborhood<P> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of point corresponding to the neighborhood.
    typedef P point;
    /// The type of psite corresponding to the neighborhood.
    typedef graph_psite<P> psite;
    // The type of the set of neighbors (node ids adjacent to the
    // reference psite).
    typedef std::set<util::node_id> sites_t;

    // FIXME: This is a dummy value.
    typedef void dpoint;

    /// \brief Site_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the ordering of vertices.
    typedef graph_neighborhood_fwd_piter<P, self_> fwd_niter;

    /// \brief Site_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the reverse ordering of vertices.
    typedef graph_neighborhood_bkd_piter<P, self_> bkd_niter;

    /// The default niter type.
    typedef fwd_niter niter;
    /// \}

    /// Services for iterators.
    /// \{
    /// Compute the set of sites for this neighborhood around \a piter.
    template <typename Piter>
    void compute_sites_(Site_Iterator<Piter>& piter) const;
    /// \}
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  template <typename Piter>
  inline
  void
  graph_elt_neighborhood<P>::compute_sites_(Site_Iterator<Piter>& piter_) const
  {
    Piter& piter = exact(piter_);
    util::node_id ref_node_id = piter.p_ref().id();
    const util::node<P>& ref_node = piter.pg().gr_->node(ref_node_id);
    sites_t& sites = piter.sites();
    sites.clear();
    /* FIXME: Move this computation out of the neighborhood. In fact,
       this should be a service of the graph, also proposed by the
       p_line_graph.  */
    // Adjacent vertices.
    for (std::vector<util::edge_id>::const_iterator e = ref_node.edges.begin();
	 e != ref_node.edges.end(); ++e)
      {
	util::node_id n1 = piter.pg().gr_->edges()[*e]->n1();
	// We explicitly enforce that the reference piter node id is
	// *not* inserted into SITES.
	if (n1 != ref_node_id)
	  sites.insert(n1);
	util::node_id n2 = piter.pg().gr_->edges()[*e]->n2();
	// Likewise.
	if (n2 != ref_node_id)
	  sites.insert(n2);
      }
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_GRAPH_ELT_NEIGHBORHOOD_HH
