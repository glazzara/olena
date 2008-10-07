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

#ifndef MLN_CORE_SITE_SET_P_LINE_GRAPH_HH
# define MLN_CORE_SITE_SET_P_LINE_GRAPH_HH

/// \file mln/core/site_set/p_line_graph.hh
/// \brief Definition of a point set based on a line graph.

# include <mln/core/internal/site_set_base.hh>
# include <mln/util/graph.hh>
# include <mln/util/tracked_ptr.hh>
# include <mln/util/site_pair.hh>

# include <mln/core/image/line_graph_psite.hh>
# include <mln/core/site_set/p_line_graph_piter.hh>

/* FIXME: This class shares a lot with p_graph.  Factor as much as
   possible.  */

/* FIXME: We should move the `adjacent'/`adjacent_or_equal' methods
   out of this class (into iterators on *graph*).  */

namespace mln
{

  // Forward declaration.
  template<typename P> struct p_line_graph;


  namespace trait
  {
    template <typename P>
    struct site_set_< p_line_graph<P> >
    {
      typedef trait::site_set::nsites::known   nsites;
      // FIXME: Depends on P!
      typedef trait::site_set::bbox::unknown   bbox;
      typedef trait::site_set::contents::fixed contents;
      typedef trait::site_set::arity::unique   arity;
    };
  } // end of namespace mln::trait


  template<typename P>
  struct p_line_graph
    : public internal::site_set_base_< site_pair<P>, p_line_graph<P> >
  {
    typedef p_line_graph<P> self_;
    typedef internal::site_set_base_< site_pair<P>, self_ > super_;

    typedef util::graph<P> graph;

    /// \brief Construct a line graph psite set from a graph of points.
    ///
    /// \param gr The graph upon which the line graph psite set is built.
    ///
    /// \a gr is \em copied internally, so that the line graph psite
    /// set is still valid after the initial graph has been removed.
    p_line_graph(const graph& gr);

    /// Associated types.
    /// \{
    /// Element associated type.
    typedef mln_site(super_) element;

    /// Point_Site associated type.
    typedef line_graph_psite<P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_line_graph_fwd_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_line_graph_bkd_piter_<P> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;
    /// \}

    /// \brief Return The number of sites of the set, i.e., the number
    /// of \em edges.
    ///
    /// (Required by the mln::Site_Set concept, since the property
    /// trait::site_set::nsites::known of this site set is set to
    /// `known'.)
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

    // FIXME: These would probably be no longer needed as soon as
    // iterators on graphs are available.

    /// Adjacency tests.
    /// \{
    /// Return true if the psites \a lhs and \a rhs are adjacent.
    /// (If \a lhs and \a rhs are equal, return false).
    bool adjacent(const psite& lhs, const psite& rhs) const;
    /// Return true if the edges \a lhs and \a rhs are adjacent.
    /// (If \a lhs and \a rhs are equal, return false).
    bool adjacent(const util::edge_id& lhs, const util::edge_id& rhs) const;

    /// Return true if the psites \a lhs and \a rhs are adjacent, or equal.
    bool adjacent_or_equal(const psite& lhs, const psite& rhs) const;
    /// Return true if the edges \a lhs and \a rhs are adjacent, or equal
    bool adjacent_or_equal(const util::edge_id& lhs,
			   const util::edge_id& rhs) const;
    /// \}

    // FIXME: Should be private.
    /// The graph on which the pset is built.
    util::tracked_ptr<graph> gr_;
  };


  /// \brief Comparison between two mln::p_line_graph's.
  ///
  /// Two mln::p_line_graph's are considered equal if they share the
  /// same graph.
  template <typename P>
  bool
  operator==(const p_line_graph<P>& lhs, const p_line_graph<P>& rhs);


  /* FIXME: Extend the `ord' mechanism instead of this ill-defined
     pseudo-order. */

  /// \brief Inclusion of a mln::p_line_graph in another one.
  ///
  /// This inclusion relation is very strict for the moment, since our
  /// infrastructure for graphs is simple: a mln::p_line_graph is
  /// included in another one if their are equal.
  ///
  /// \todo Refine this later, when we are able to express subgraph
  /// relations.
  template <typename P>
  bool
  operator<=(const p_line_graph<P>& lhs, const p_line_graph<P>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_line_graph<P>::p_line_graph(const graph& gr)
    // Create a deep, managed copy of GR.
    : gr_(new util::graph<P>(gr))
  {
  }

  template <typename P>
  inline
  unsigned
  p_line_graph<P>::nsites() const
  {
    return nedges();
  }

  template <typename P>
  inline
  std::size_t
  p_line_graph<P>::nvertices() const
  {
    return this->gr_->nvertices();
  }

  template <typename P>
  inline
  std::size_t
  p_line_graph<P>::nedges() const
  {
    return this->gr_->nedges();
  }

  template <typename P>
  inline
  bool
  p_line_graph<P>::is_valid() const
  {
    // FIXME: Might be too low-level, again.
    return gr_.ptr_;
  }

  template <typename P>
  inline
  bool
  p_line_graph<P>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return
      // Check whether P is compatible with this psite set.
      (p.target_() == this) &&
      // Check that the edge id of P belongs to the range of valid edge ids.
      (p.is_valid());
  }

  template <typename P>
  inline
  std::size_t
  p_line_graph<P>::memory_size() const
  {
    // FIXME: Dummy; implement (see other site sets).
    abort();
    return 0;
  }

  template <typename P>
  const util::graph<P>&
  p_line_graph<P>::gr() const
  {
    mln_precondition(is_valid());
    return *gr_.ptr_;
  }

  template <typename P>
  util::graph<P>&
  p_line_graph<P>::gr()
  {
    mln_precondition(is_valid());
    return *gr_.ptr_;
  }

  template <typename P>
  inline
  bool
  p_line_graph<P>::adjacent(const psite& lhs, const psite& rhs) const
  {
    mln_assertion(&lhs.plg() == this && rhs.plg() == this);
    return adjacent(lhs.id(), rhs.id());
  }

  /* FIXME: This could be more efficient, if the graph structure had a
     richer interface.  */
  template <typename P>
  inline
  bool
  p_line_graph<P>::adjacent(const util::edge_id& lhs,
			    const util::edge_id& rhs) const
  {
    mln_assertion(lhs < this->nsites());
    mln_assertion(rhs < this->nsites());

    // Ensure LHS and RHS are *not* equal.
    if (lhs == rhs)
      return false;

    // Check whether LHS and RHS are adjacent (i.e., whether LHS and
    // RHS share a common vertex.
    /* FIXME: This is too low-level.  Yet another service the graph
       should provide.  */
    if (gr_->edge(lhs).v1() == gr_->edge(rhs).v1() ||
	gr_->edge(lhs).v1() == gr_->edge(rhs).v2() ||
	gr_->edge(lhs).v2() == gr_->edge(rhs).v1() ||
	gr_->edge(lhs).v2() == gr_->edge(rhs).v2())
      return true;

    return false;
  }

  template <typename P>
  inline
  bool
  p_line_graph<P>::adjacent_or_equal(const psite& lhs, const psite& rhs) const
  {
    mln_assertion(&lhs.pg() == this && rhs.pg() == this);
    return adjacent_or_equal(lhs.id(), rhs.id());
  }

  template <typename P>
  inline
  bool
  p_line_graph<P>::adjacent_or_equal(const util::edge_id& lhs,
				     const util::edge_id& rhs) const
  {
    mln_assertion(lhs < this->nsites());
    mln_assertion(rhs < this->nsites());

    // Check whether LHS and RHS are equal.
    if (lhs == rhs)
      return true;
    // Check whether RHS and LHS are adjacent.
    return adjacent(lhs, rhs);
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <typename P>
  bool
  operator==(const p_line_graph<P>& lhs, const p_line_graph<P>& rhs)
  {
    /* FIXME: We should not rely on pointer equality here, as graph
       will soon become shells using (shared) tracked pointers to
       actual data.  So, delegate the equality test to the graphs
       themselves.  */
    return lhs.gr_.ptr_ == rhs.gr_.ptr_;
  }

  template <typename P>
  bool
  operator<=(const p_line_graph<P>& lhs, const p_line_graph<P>& rhs)
  {
    return lhs == rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_SITE_SET_P_LINE_GRAPH_HH
