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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_SITE_SET_P_GRAPH_HH
# define MLN_CORE_SITE_SET_P_GRAPH_HH

/// \file mln/core/site_set/p_graph.hh
/// \brief Definition of a point set based on a graph.

# include <mln/core/internal/site_set_base.hh>
# include <mln/util/graph.hh>
# include <mln/util/tracked_ptr.hh>

# include <mln/core/image/graph_psite.hh>
# include <mln/core/site_set/p_graph_piter.hh>

/* FIXME: This class shares a lot with p_line_graph.  Factor as much
   as possible.  */

/* FIXME: We should move the `adjacent'/`adjacent_or_equal' methods
   out of this class (into iterators on *graph*).  */


namespace mln
{
  // Forward declaration.
  template <typename P> struct p_graph;

  namespace trait
  {
    template <typename P>
    struct site_set_< p_graph<P> >
    {
      typedef trait::site_set::nsites::known   nsites;
      // FIXME: Depends on P!
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };
  } // end of namespace mln::trait


  template <typename P>
  struct p_graph
    : public internal::site_set_base_< P, p_graph<P> >
  {
    typedef p_graph<P> self_;
    typedef internal::site_set_base_< P, self_ > super_;

    typedef util::graph<P> graph;

    /// \brief Construct a graph psite set from a graph of points.
    ///
    /// \param gr The graph upon which the graph psite set is built.
    ///
    /// \a gr is \em copied internally, so that the graph psite set is
    /// still valid after the initial graph has been removed.
    p_graph(const graph& gr);

    /// Associated types.
    /// \{
    /// Element associated type.
    typedef mln_site(super_) element;
    /// Point_Site associated type.
    typedef graph_psite<P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_graph_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_graph_bkd_piter_<P> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;
    /// \}

    /// \brief Return The number of points (sites) of the set, i.e.,
    /// the number of \em vertices.
    ///
    /// Required by the mln::Point_Set concept.
    /* FIXME: Return type should be std::size_t (see
       mln/core/concept/site_set.hh). */
    unsigned nsites() const;

    /// Return The number of vertices in the graph.
    std::size_t nvertices() const;
    /// Return The number of edges in the graph.
    std::size_t nedges() const;

    /// Is this site set valid?
    bool is_valid() const;

    /// Does this site set has \a p?
    bool has(const psite& p) const;

    // FIXME: Dummy.
    std::size_t memory_size() const;

    /// Accessors.
    /// \{
    /// Return the graph associated to this site set (const version)
    const graph& gr() const;
    /// Return the graph associated to this site set (mutable version).
    graph& gr();
    /// \}

    /// \brief Graph-related services
    ///
    /// \todo Move them into iterators on graphs.
    /// \{
    /// Return the graph point (FIXME site?) from an index
    const P& point_from_id(const util::vertex_id& id) const;
    P& point_from_id(const util::vertex_id& id);

    /// Return the point contained in the first vertex adjacent
    // to the edge id \a e.
    const P& vertex1(const util::edge_id& e) const;
    /// Return the point contained in the second vertex adjacent
    /// to the edge  id \a e.
    const P& vertex2(const util::edge_id& e) const;

    // FIXME: These would probably be no longer needed as soon as
    // iterators on graphs are available.

    /// Adjacency tests.
    /// \{
    /// Return true if the psites \a lhs and \a rhs are adjacent.
    /// (If \a lhs and \a rhs are equal, return false).
    bool adjacent(const psite& lhs, const psite& rhs) const;
    /// Return true if the vertices \a lhs and \a rhs are adjacent.
    /// (If \a lhs and \a rhs are equal, return false).
    bool adjacent(const util::vertex_id& lhs, const util::vertex_id& rhs) const;

    /// Return true if the psites \a lhs and \a rhs are adjacent, or equal.
    bool adjacent_or_equal(const psite& lhs, const psite& rhs) const;
    /// Return true if the vertices \a lhs and \a rhs are adjacent, or equal
    bool adjacent_or_equal(const util::vertex_id& lhs,
			   const util::vertex_id& rhs) const;
    /// \}

    /// \}

  public:
    // FIXME: Should be private.
    util::tracked_ptr<graph> gr_;
  };


  /// \brief Comparison between two mln::p_graph's.
  ///
  /// Two mln::p_graph's are considered equal if they share the
  /// same graph.
  template <typename P>
  bool
  operator==(const p_graph<P>& lhs, const p_graph<P>& rhs);


  /* FIXME: Extend the `ord' mechanism instead of this ill-defined
     pseudo-order. */

  /// \brief Inclusion of a mln::p_graph in another one.
  ///
  /// This inclusion relation is very strict for the moment, since our
  /// infrastructure for graphs is simple: a mln::p_graph is included
  /// in another one if their are equal.
  ///
  /// \todo Refine this later, when we are able to express subgraph
  /// relations.
  template <typename P>
  bool
  operator<=(const p_graph<P>& lhs, const p_graph<P>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_graph<P>::p_graph(const util::graph<P>& gr)
    // Create a deep, managed copy of GR.
    : gr_ (new util::graph<P>(gr))
  {
  }

  template <typename P>
  inline
  unsigned
  p_graph<P>::nsites() const
  {
    return nvertices();
  }

  template <typename P>
  inline
  std::size_t
  p_graph<P>::nvertices() const
  {
    return this->gr_->nvertices();
  }

  template <typename P>
  inline
  std::size_t
  p_graph<P>::nedges() const
  {
    return this->gr_->nedges();
  }

  template <typename P>
  inline
  bool
  p_graph<P>::is_valid() const
  {
    // FIXME: Might be too low-level, again.
    return gr_.ptr_;
  }

  template <typename P>
  inline
  bool
  p_graph<P>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return
      // Check whether P is compatible with this psite set.
      (p.target_() == this) &&
      // Check that the vertex id of P belongs to the range of valid
      // vertex ids.
      (p.is_valid());
  }

  template <typename P>
  inline
  std::size_t
  p_graph<P>::memory_size() const
  {
    // FIXME: Dummy; implement (see other site sets).
    abort();
    return 0;
  }

  template <typename P>
  const typename p_graph<P>::graph&
  p_graph<P>::gr() const
  {
    mln_precondition(is_valid());
    return *gr_.ptr;
  }

  template <typename P>
  typename p_graph<P>::graph&
  p_graph<P>::gr()
  {
    mln_precondition(is_valid());
    return *gr_.ptr;
  }

  template <typename P>
  const P&
  p_graph<P>::point_from_id(const util::vertex_id& id) const
  {
    return this->gr_->vertex_data(id);
  }

  template <typename P>
  P&
  p_graph<P>::point_from_id(const util::vertex_id& id)
  {
    return this->gr_->vertex_data(id);
  }

  template <typename P>
  const P&
  p_graph<P>::vertex1(const util::edge_id& e) const
  {
    util::vertex_id v1 = this->gr_->edge(e).v1();
    return this->point_from_id(v1);
  }

  template <typename P>
  const P&
  p_graph<P>::vertex2(const util::edge_id& e) const
  {
    util::vertex_id v2 = this->gr_->edge(e).v2();
    return this->point_from_id(v2);
  }

  template <typename P>
  inline
  bool
  p_graph<P>::adjacent(const psite& lhs, const psite& rhs) const
  {
    mln_assertion(&lhs.pg() == this && rhs.pg() == this);
    return adjacent(lhs.id(), rhs.id());
  }

  /* FIXME: This could be more efficient, if the graph structure had a
     richer interface.  */
  template <typename P>
  inline
  bool
  p_graph<P>::adjacent(const util::vertex_id& lhs,
		       const util::vertex_id& rhs) const
  {
    mln_assertion(lhs < this->nsites());
    mln_assertion(rhs < this->nsites());

    // Ensure LHS and RHS are *not* equal.
    if (rhs == lhs)
      return false;

    // Check whether LHS and RHS are adjacent (i.e., whether RHS is
    // among the neighbors of LHS).
    typedef std::vector<util::edge_id> edges_t;
    const edges_t& lhs_neighbs = gr_->vertices()[lhs]->edges;
    for (edges_t::const_iterator e = lhs_neighbs.begin();
	 e != lhs_neighbs.end(); ++e)
      if (gr_->edges()[*e]->v1() == rhs ||
	  gr_->edges()[*e]->v2() == rhs)
	return true;

    return false;
  }

  template <typename P>
  inline
  bool
  p_graph<P>::adjacent_or_equal(const psite& lhs, const psite& rhs) const
  {
    mln_assertion(&lhs.pg() == this && rhs.pg() == this);
    return adjacent_or_equal(lhs.id(), rhs.id());
  }

  template <typename P>
  inline
  bool
  p_graph<P>::adjacent_or_equal(const util::vertex_id& lhs,
				const util::vertex_id& rhs) const
  {
    mln_assertion(lhs < this->nsites());
    mln_assertion(rhs < this->nsites());

    // Check whether LHS and RHS are equal.
    if (lhs == rhs)
      return true;
    // Check whether LHS and RHS are adjacent.
    return adjacent(lhs, rhs);
  }


  template <typename P>
  bool
  operator==(const p_graph<P>& lhs, const p_graph<P>& rhs)
  {
    /* FIXME: We should not rely on pointer equality here, as graph
       will soon become shells using (shared) tracked pointers to
       actual data.  So, delegate the equality test to the graphs
       themselves.  */
    return lhs.gr_.ptr_ == rhs.gr_.ptr_;
  }

  template <typename P>
  bool
  operator<=(const p_graph<P>& lhs, const p_graph<P>& rhs)
  {
    return lhs == rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_CORE_SITE_SET_P_GRAPH_HH
