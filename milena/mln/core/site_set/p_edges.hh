// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_SITE_SET_P_EDGES_HH
# define MLN_CORE_SITE_SET_P_EDGES_HH

/// \file
///
/// Definition of a site set based on graph edges.

# include <mln/core/concept/function.hh>
# include <mln/core/concept/graph.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/site_set/p_graph_piter.hh>
# include <mln/core/site_set/p_edges_psite.hh>
# include <mln/util/graph.hh>
# include <mln/util/internal/id2element.hh>

# include <mln/metal/equal.hh>


namespace mln
{

  // Forward declaration.
  template <typename G, typename F> class p_edges;


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


  /// \brief Site set mapping graph edges and image sites.
  ///
  /// \ingroup modsitesetgraph
  //
  template <typename G, typename F = util::internal::id2element<G,util::edge<G> > >
  class p_edges
    : public internal::site_set_base_< mln_result(F), p_edges<G, F> >
  {

    typedef p_edges<G, F> self_;
    typedef internal::site_set_base_< mln_result(F), self_ > super_;

  public:

    /// Graph associated type.
    typedef G graph_t;

    /// Function associated type.
    typedef F fun_t;

    /// Type of graph edge.
    typedef util::edge<G> edge;

    /// Type of graph element this site set focuses on.
    typedef util::edge<G> graph_element;

    /// Constructors
    /// @{
    /// Default constructor.
    p_edges();

    /// Construct a graph edge psite set from a graph.
    ///
    /// \param gr The graph upon which the graph edge psite set is built.
    p_edges(const Graph<G>& gr);

    /// Construct a graph edge psite set from a graph and a function.
    ///
    /// \param gr The graph upon which the graph edge psite set is built.
    /// \param f the function mapping edges and sites.
    p_edges(const Graph<G>& gr, const Function<F>& f);

    /// Construct a graph edge psite set from a graph and a function.
    ///
    /// \param gr The graph upon which the graph edge psite set is built.
    /// \param f the function mapping edges and sites.
    ///		 It must be convertible towards the function type \c F.
    template <typename F2>
    p_edges(const Graph<G>& gr, const Function<F2>& f);
    ///@}

    /// Associated types.
    /// \{
    /// Element associated type.
    typedef mln_site(super_) element;

    /// Point_Site associated type.
    typedef p_edges_psite<G, F> psite;

    /// Forward Site_Iterator associated type.
    typedef p_graph_piter< self_, mln_edge_fwd_iter(G) > fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_graph_piter< self_, mln_edge_bkd_iter(G) > bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;
    /// \}

    /// Return The number of points (sites) of the set, i.e.,
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

    /// Does this site set has \a vertex_id?
    /// FIXME: causes ambiguities while calling has(mln::neighb_fwd_niter<>);
    ///    bool has(unsigned vertex_id) const;

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

    G g_;
    F f_;
  };


  /// Comparison between two mln::p_edges's.
  ///
  /// Two mln::p_edges's are considered equal if they share the
  /// same graph.
  template <typename G, typename F>
  bool
  operator==(const p_edges<G, F>& lhs, const p_edges<G, F>& rhs);


  /// Inclusion of a mln::p_edges in another one.
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
  p_edges<G, F>::p_edges()
  {
  }

  template <typename G, typename F>
  inline
  p_edges<G, F>::p_edges(const Graph<G>& g)
  {
    typedef util::internal::id2element<G,util::edge<G> > F_REF;
    mlc_equal(F, F_REF)::check();

    mln_precondition(exact(g).is_valid());
    g_ = exact(g);
    f_ = util::internal::id2element< G, util::edge<G> >(g);
  }

  template <typename G, typename F>
  inline
  p_edges<G, F>::p_edges(const Graph<G>& g, const Function<F>& f)
  {
    mln_precondition(exact(g).is_valid());
    g_ = exact(g);
    f_ = exact(f);
  }

  template <typename G, typename F>
  template <typename F2>
  inline
  p_edges<G, F>::p_edges(const Graph<G>& g, const Function<F2>& f)
  {
    mln_precondition(exact(g).is_valid());
    mlc_converts_to(F2,F)::check();

    g_ = exact(g);
    convert::from_to(f, f_);
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
    return this->g_.e_nmax();
  }

  template <typename G, typename F>
  inline
  bool
  p_edges<G, F>::is_valid() const
  {
    return g_.is_valid();
  }

  template <typename G, typename F>
  inline
  void
  p_edges<G, F>::invalidate()
  {
    g_.invalidate();
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
    return e.graph().is_subgraph_of(g_) && g_.has(e) && e.is_valid();
  }

//  template <typename G, typename F>
//  inline
//  bool
//  p_edges<G,F>::has(unsigned edge_id) const
//  {
//    mln_precondition(is_valid());
//    util::edge<G> e(g_, edge_id);
//    return has(e);
//  }

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
    return g_;
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

#endif // ! MLN_CORE_SITE_SET_P_EDGES_HH
