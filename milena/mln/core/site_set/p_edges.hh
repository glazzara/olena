// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_EDGES_HH
# define MLN_CORE_SITE_SET_P_EDGES_HH

  /// \file mln/core/site_set/p_edges.hh
  /// \brief Definition of a site set based on graph edges.

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/site_set/p_graph_piter.hh>
# include <mln/util/internal/graph_edge_psite.hh>
# include <mln/util/graph.hh>


namespace mln
{
  // Forward declaration.
  template <typename G, typename F> struct p_edges;

  namespace trait
  {
    template <typename G, typename F>
      struct site_set_< p_edges<G, F> >
      {
	// FIXME: I don't know what to use yet!
	typedef trait::site_set::nsites::known   nsites;
	typedef trait::site_set::bbox::unknown   bbox;
	typedef trait::site_set::contents::fixed contents;
	typedef trait::site_set::arity::unique   arity;
      };
  } // end of namespace mln::trait


  template <typename G, typename F>
  class p_edges
    : public internal::site_set_base_< mln_result(F), p_edges<G, F> >
  {

    typedef p_edges<G, F> self_;
    typedef internal::site_set_base_< mln_result(F), self_ > super_;

  public:

    /// Type of graph edge.
    typedef util::edge<G> edge;

    /// Type of graph element this site set focuses on.
    typedef util::edge<G> graph_element;


    /// Construct a graph edge psite set from a graph and a function.
    ///
    /// \param gr The graph upon which the graph edge psite set is built.
    /// \param f the function mapping edges and sites.
    p_edges(const G& gr, const F& f);

    /// Associated types.
    /// \{
    /// Element associated type.
    typedef mln_site(super_) element;

    /// Point_Site associated type.
    typedef internal::edge_psite<G, F> psite;

    /// Forward Site_Iterator associated type.
    typedef p_graph_piter< self_, mln_edge_fwd_iter(G) > fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_graph_piter< self_, mln_edge_bkd_iter(G) > bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;
    /// \}

    /// \brief Return The number of points (sites) of the set, i.e.,
    /// the number of \em edges.
    unsigned nsites() const;

    /// Return The number of edges in the graph.
    unsigned nedges() const;

    /// Is this site set valid?
    bool is_valid() const;
    /// Invalidate this site set.
    void invalidate();

    /// Does this site set has site \a p?
    bool has(const psite& p) const;

    /// Does this site set has edge \a e?
    template <typename G2>
    bool has(const util::edge<G2>& e) const;

    // FIXME: Dummy.
    std::size_t memory_size() const;

    /// Accessors.
    /// \{
    /// Return the graph associated to this site set
    const G& graph() const;
    /// Return the mapping function.
    const F& function() const;
    /// \}

  private:

    const G* g_;
    F f_;
  };


  /// \brief Comparison between two mln::p_edges's.
  ///
  /// Two mln::p_edges's are considered equal if they share the
  /// same graph.
  template <typename G, typename F>
  bool
  operator==(const p_edges<G, F>& lhs, const p_edges<G, F>& rhs);


  /// \brief Inclusion of a mln::p_edges in another one.
  ///
  /// \todo Refine this later, when we are able to express subgraph
  /// relations.
  template <typename G, typename F>
  bool
  operator<=(const p_edges<G, F>& lhs, const p_edges<G, F>& rhs);

} // end of mln


# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  template <typename G, typename F>
  inline
  p_edges<G, F>::p_edges(const G& g, const F& f)
    : g_ (&g), f_(f)
  {
  }

  template <typename G, typename F>
  inline
  unsigned
  p_edges<G, F>::nsites() const
  {
    return nedges();
  }

  template <typename G, typename F>
  inline
  unsigned
  p_edges<G, F>::nedges() const
  {
    return this->g_->e_nmax();
  }

  template <typename G, typename F>
  inline
  bool
  p_edges<G, F>::is_valid() const
  {
    return g_ != 0;
  }

  template <typename G, typename F>
  inline
  void
  p_edges<G, F>::invalidate()
  {
    g_ = 0;
  }

  template <typename G, typename F>
  inline
  bool
  p_edges<G, F>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return has(p.e());
  }

  template <typename G, typename F>
  template <typename G2>
  inline
  bool
  p_edges<G, F>::has(const util::edge<G2>& e) const
  {
    mln_precondition(is_valid());
    return e.graph().is_subgraph_of(*g_) && g_->has(e) && e.is_valid();
  }

  template <typename G, typename F>
  inline
  std::size_t
  p_edges<G, F>::memory_size() const
  {
    // FIXME: Dummy; implement (see other site sets).
    abort();
    return 0;
  }

  template <typename G, typename F>
  inline
  const G&
  p_edges<G, F>::graph() const
  {
    mln_precondition(is_valid());
    return *g_;
  }

  template <typename G, typename F>
  inline
  const F&
  p_edges<G, F>::function() const
  {
    return f_;
  }

  template <typename G, typename F>
  bool
  operator==(const p_edges<G, F>& lhs, const p_edges<G, F>& rhs)
  {
    return lhs.graph() == rhs.graph();
  }

  template <typename G, typename F>
  bool
  operator<=(const p_edges<G, F>& lhs, const p_edges<G, F>& rhs)
  {
    return lhs == rhs;
  }

} // end of mln

# endif // ! MLN_INCLUDE_ONLY

#endif // MLN_CORE_SITE_SET_P_EDGES_HH
