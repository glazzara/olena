// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH
# define MLN_CORE_IMAGE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH

/// \file mln/core/image/line_graph_elt_neighborhood.hh
/// Definition of the elementary ``neighborhood'' on a line graph.

# include <set>

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/internal/graph_neighborhood_base.hh>
# include <mln/util/internal/graph_edge_psite.hh>
# include <mln/core/image/line_graph_neighborhood_piter.hh>

# include <mln/core/image/line_graph_elt_window.hh>


namespace mln
{

  /// Elementary neighborhood on line graph class.
  template <typename G, typename F>
  class line_graph_elt_neighborhood
    : public graph_neighborhood_base<G,
				     F,
				     internal::edge_psite<G, F>,
				     line_graph_elt_neighborhood<G, F> >
  {
    typedef line_graph_elt_neighborhood<G, F> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of psite corresponding to the neighborhood.
    typedef internal::edge_psite<G, F> psite;

    /// Site_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the ordering of edges.
    typedef line_graph_neighborhood_fwd_piter<G, F, self_> fwd_niter;

    /// Site_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the reverse ordering of edges.
    typedef line_graph_neighborhood_bkd_piter<G, F, self_> bkd_niter;

    /// The default niter type.
    typedef fwd_niter niter;
    /// \}

    /// Services for iterators.
    /// \{
    /// Compute the set of sites for this neighborhood around \a piter.
    template <typename Piter>
    void compute_sites_(Site_Iterator<Piter>& piter) const;
    /// \}

  protected:
    typedef graph_neighborhood_base<G, F, psite, self_> super_;
    typedef typename super_::sites_t sites_t;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename G, typename F>
  template <typename Piter>
  inline
  void
  line_graph_elt_neighborhood<G, F>::compute_sites_(Site_Iterator<Piter>& piter_) const
  {
    Piter& piter = exact(piter_);
    unsigned central_edge = piter.center().e().id();
    sites_t& sites = piter.sites();
    sites.clear();

    const G& g = piter.center().site_set().graph();

    for (unsigned i = 0; i < g.e_nmax_nbh_edges(central_edge); ++i)
    {
      unsigned n = g.e_ith_nbh_edge(central_edge, i);
      if (n != central_edge)
	sites.insert(g.e_ith_nbh_edge(central_edge, i));
    }
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH
