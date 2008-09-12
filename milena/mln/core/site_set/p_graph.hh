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

# include <mln/core/concept/point_site.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/accu/bbox.hh>
# include <mln/util/tracked_ptr.hh>
# include <mln/util/graph.hh>

# include <mln/core/image/graph_psite.hh>
# include <mln/core/site_set/p_graph_piter.hh>


namespace mln
{
  /* FIXME: Contray to, e.g., p_array, the sole parameter P of p_graph
     is expected to be a point, not a psite!!  We should have a
     uniform scheme for point site sets.  */

  template <typename P>
  struct p_graph
    : public internal::site_set_base_< graph_psite<P>, p_graph<P> >
  {
    typedef util::graph<P> graph;

    /// \brief Construct a graph psite set from a graph of points.
    ///
    /// \param gr The graph upon which the graph psite set is built.
    ///
    /// \a gr is \em copied internally, so that the graph psite set is
    /// still valid after the initial graph has been removed.
    p_graph (const graph& gr);

    /// Point_Site associated type.
    typedef graph_psite<P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_graph_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_graph_bkd_piter_<P> bkd_piter;

    /// \brief Return The number of points (sites) of the set, i.e.,
    /// the number of \em vertices.
    ///
    /// Required by the mln::Point_Set concept.
    std::size_t npoints() const;

    /// Return The number of vertices in the graph.
    std::size_t nvertices() const;
    /// Return The number of edges in the graph.
    std::size_t nedges() const;

    /// Give the exact bounding box.
    const box<P>& bbox() const;

    bool has(const psite& p) const;

    /// Return the graph point (FIXME site?) from an index
    const P& point_from_id(const util::vertex_id& id) const;
    P& point_from_id(const util::vertex_id& id);


    /// Return the point contained in the first vertex adjacent
    // to the edge id \a e.
    const P& vertex1(const util::edge_id& e) const;
    /// Return the point contained in the second vertex adjacent
    /// to the edge  id \a e.
    const P& vertex2(const util::edge_id& e) const;

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

    /// Return the graph associated to the p_graph domain.
    // FIXME: Rename as something else (graph() ?).
    const graph& to_graph() const;
    graph& to_graph();


    // FIXME: Should be private.
  public:
    util::tracked_ptr<graph> gr_;
    // FIXME: (Roland) Is it really useful/needed?
    /* 2007-12-19: It seems so, since graph_image must implement a method
       named bbox().  Now the question is: should each image type have a
       bounding box?  */
  private:
    box<P> bb_;
  };


  /// \brief Comparison between two mln::p_graph's.
  ///
  /// Two mln::p_graph's are considered equal if they share the
  /// same graph.
  template <typename P>
  bool
  operator==(const p_graph<P>& lhs, const p_graph<P>& rhs);

  /// \brief Inclusion of a mln::p_graph in another one.
  ///
  /// This inclusion relation is very strict for the moment, since our
  /// infrastrure for graphs is simple: a mln::p_graph is included
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
    accu::bbox<P> a;
    for (unsigned i = 0; i < npoints(); ++i)
      a.take(gr_->vertex_data(i));
    bb_ = a.to_result();
  }

  template <typename P>
  inline
  std::size_t
  p_graph<P>::npoints() const
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
  const box<P>&
  p_graph<P>::bbox() const
  {
    return bb_;
  }

  template <typename P>
  inline
  bool
  p_graph<P>::has(const psite& p) const
  {
    return
      // Check whether P is compatible with this psite set.
      (&p.pg() == this) &&
      // Check that the vertex id of P belongs to the range of valid vertex ids.
      (p.id() < gr_->nvertices());
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
    mln_assertion(lhs < this->npoints());
    mln_assertion(rhs < this->npoints());

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
    mln_assertion(lhs < this->npoints());
    mln_assertion(rhs < this->npoints());

    // Check whether LHS and RHS are equal.
    if (lhs == rhs)
      return true;
    // Check whether LHS and RHS are adjacent.
    return adjacent(lhs, rhs);
  }

  template <typename P>
  const typename p_graph<P>::graph&
  p_graph<P>::to_graph() const
  {
    return this->gr_;
  }

  template <typename P>
  typename p_graph<P>::graph&
  p_graph<P>::to_graph()
  {
    return this->gr_;
  }


  template <typename P>
  bool
  operator==(const p_graph<P>& lhs, const p_graph<P>& rhs)
  {
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
