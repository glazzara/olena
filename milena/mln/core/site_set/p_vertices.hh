// Copyright (C) 2008, 2009, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_VERTICES_HH
# define MLN_CORE_SITE_SET_P_VERTICES_HH

/// \file
///
/// Definition of a point set based on graph vertices.

# include <mln/core/concept/function.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/site_set/p_graph_piter.hh>
# include <mln/core/site_set/p_vertices_psite.hh>
# include <mln/util/graph.hh>
# include <mln/util/internal/id2element.hh>



namespace mln
{

  // Forward declaration.
  template <typename G, typename F> class p_vertices;


  namespace trait
  {

    template <typename G, typename F>
      struct site_set_< p_vertices<G,F> >
      {
	typedef trait::site_set::nsites::known   nsites;
	// FIXME: !
	typedef trait::site_set::bbox::unknown   bbox;
	typedef trait::site_set::contents::fixed contents;
	typedef trait::site_set::arity::unique   arity;
      };

  } // end of namespace mln::trait


  /// \brief Site set based mapping graph vertices to sites.
  ///
  /// \ingroup modsitesetgraph
  //
  template <typename G, typename F = util::internal::id2element<G,util::vertex<G> > >
  class p_vertices
    : public internal::site_set_base_< mln_result(F), p_vertices<G,F> >
  {

    typedef p_vertices<G,F> self_;
    typedef internal::site_set_base_< mln_result(F), self_ > super_;

  public:

    /// Graph associated type.
    typedef G graph_t;

    /// Function associated type.
    typedef F fun_t;

    /// Type of graph vertex.
    typedef util::vertex<G> vertex;


    /// Type of graph element this site set focuses on.
    typedef util::vertex<G> graph_element;


    /// Constructor without argument.
    p_vertices();

    /// Construct a graph psite set from a graph of points.
    /// \param gr The graph upon which the graph psite set is built.
    /// The identity function is used.
    p_vertices(const Graph<G>& gr);

    /// Construct a graph psite set from a graph of points.
    /// \param gr The graph upon which the graph psite set is built.
    /// \param f the function which maps a vertex to a site.
    p_vertices(const Graph<G>& gr, const Function<F>& f);

    /// Construct a graph psite set from a graph of points.
    /// \param gr The graph upon which the graph psite set is built.
    /// \param f the function which maps a vertex to a site.
    ///		 It must be convertible to the function type \c F.
    template <typename F2>
    p_vertices(const Graph<G>& gr, const Function<F2>& f);
    /// \}

    /// Copy constructor.
    template <typename F2>
    p_vertices(const p_vertices<G,F2>& other);

    /// Associated types.
    /// \{

    /// Element associated type.
    typedef mln_site(super_) element;

    /// Point_Site associated type.
    typedef p_vertices_psite<G,F> psite;

    /// Forward Site_Iterator associated type.
    typedef p_graph_piter< self_, mln_vertex_fwd_iter(G) > fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_graph_piter< self_, mln_vertex_bkd_iter(G) > bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;

    /// \}


    /// \brief Return The number of points (sites) of the set, i.e.,
    /// the number of \em vertices.
    ///
    /// Required by the mln::Point_Set concept.
    unsigned nsites() const;

    /// Return The number of vertices in the graph.
    unsigned nvertices() const;

    /// Test this site set validity.
    bool is_valid() const;

    /// Invalidate this site set.
    void invalidate();

    /// Does this site set has \a p?
    bool has(const psite& p) const;

    /// Does this site set has \a v?
    template <typename G2>
    bool has(const util::vertex<G2>& v) const;

    /// Does this site set has \a vertex_id?
    /// FIXME: causes ambiguities while calling has(mln::neighb_fwd_niter<>);
    /// bool has(unsigned vertex_id) const;

    // FIXME: Dummy.
    std::size_t memory_size() const;

    /// Return the value associated to an element of this site set.
    /// \{
    mln_result(F) operator()(const psite& p) const;
    mln_result(F) operator()(const util::vertex<G>& p) const;
    mln_result(F) operator()(unsigned id_v) const;
    /// \}

    /// Accessors.
    /// \{
    /// Return the graph associated to this site set (const version)
    const G& graph() const;
    /// Return the association function.
    const F& function() const;
    /// \}

  private:
    G g_;
    F f_;
  };


  /// \brief Comparison between two mln::p_vertices's.
  ///
  /// Two mln::p_vertices's are considered equal if they share the
  /// same graph.
  template <typename G, typename F>
  bool
  operator==(const p_vertices<G,F>& lhs, const p_vertices<G,F>& rhs);


  /* FIXME: Extend the `ord' mechanism instead of this ill-defined
     pseudo-order. */

  /// \brief Inclusion of a mln::p_vertices in another one.
  ///
  /// This inclusion relation is very strict for the moment, since our
  /// infrastructure for graphs is simple: a mln::p_vertices is included
  /// in another one if their are equal.
  ///
  /// \todo Refine this later, when we are able to express subgraph
  /// relations.
  template <typename G, typename F>
  bool
  operator<=(const p_vertices<G,F>& lhs, const p_vertices<G,F>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <typename G, typename F>
  inline
  p_vertices<G,F>::p_vertices()
  {
  }

  template <typename G, typename F>
  inline
  p_vertices<G,F>::p_vertices(const Graph<G>& g)
  {
    typedef util::internal::id2element<G,util::vertex<G> > F_REF;
    mlc_equal(F, F_REF)::check();

    mln_precondition(exact(g).is_valid());
    g_ = exact(g);
    f_ = util::internal::id2element< G, util::vertex<G> >(g);
  }

  template <typename G, typename F>
  inline
  p_vertices<G,F>::p_vertices(const Graph<G>& g, const Function<F>& f)
  {
    mln_precondition(exact(g).is_valid());
    g_ = exact(g);
    f_ = exact(f);
  }

  template <typename G, typename F>
  template <typename F2>
  inline
  p_vertices<G,F>::p_vertices(const Graph<G>& g, const Function<F2>& f)
  {
    mln_precondition(exact(g).is_valid());
    mlc_converts_to(F2,F)::check();

    g_ = exact(g);
    convert::from_to(f, f_);
  }

  template <typename G, typename F>
  template <typename F2>
  inline
  p_vertices<G,F>::p_vertices(const p_vertices<G,F2>& other)
  {
    mln_precondition(other.is_valid());
    mlc_converts_to(F2,F)::check();

    g_ = other.graph();
    convert::from_to(other.function(), f_);
  }

  template <typename G, typename F>
  inline
  unsigned
  p_vertices<G,F>::nsites() const
  {
    return nvertices();
  }

  template <typename G, typename F>
  inline
  unsigned
  p_vertices<G,F>::nvertices() const
  {
    return this->g_.v_nmax();
  }

  template <typename G, typename F>
  inline
  bool
  p_vertices<G,F>::is_valid() const
  {
    return g_.is_valid();
  }

  template <typename G, typename F>
  inline
  void
  p_vertices<G,F>::invalidate()
  {
    g_.invalidate();
  }

  template <typename G, typename F>
  inline
  bool
  p_vertices<G,F>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return has(p.v());
  }

  template <typename G, typename F>
  template <typename G2>
  inline
  bool
  p_vertices<G,F>::has(const util::vertex<G2>& v) const
  {
    mln_precondition(is_valid());
    return
      // Check whether if the graph is 'compatible'.
      v.graph().is_subgraph_of(g_) &&
      g_.has(v) &&
      // Check that the vertex id of P belongs to the range of valid
      // vertex ids.
      (v.is_valid());
  }

//  template <typename G, typename F>
//  inline
//  bool
//  p_vertices<G,F>::has(unsigned vertex_id) const
//  {
//    mln_precondition(is_valid());
//    util::vertex<G> v(g_, vertex_id);
//    return has(v);
//  }

  template <typename G, typename F>
  inline
  std::size_t
  p_vertices<G,F>::memory_size() const
  {
    // FIXME: Dummy; implement (see other site sets).
    abort();
    return 0;
  }

  template <typename G, typename F>
  inline
  mln_result(F)
  p_vertices<G,F>::operator()(const psite& p) const
  {
    mln_precondition(g_.has(p.v()));
    return (*this)(p.v().id());
  }

  template <typename G, typename F>
  inline
  mln_result(F)
  p_vertices<G,F>::operator()(const util::vertex<G>& v) const
  {
    mln_precondition(g_.has_v(v));
    return (*this)(v.id());
  }

  template <typename G, typename F>
  inline
  mln_result(F)
  p_vertices<G,F>::operator()(unsigned id_v) const
  {
    mln_precondition(g_.has_v(id_v));
    return f_(id_v);
  }

  template <typename G, typename F>
  inline
  const G&
  p_vertices<G,F>::graph() const
  {
    mln_precondition(is_valid());
    return g_;
  }

  template <typename G, typename F>
  inline
  const F&
  p_vertices<G,F>::function() const
  {
    return f_;
  }

  template <typename G, typename F>
  bool
  operator==(const p_vertices<G,F>& lhs, const p_vertices<G,F>& rhs)
  {
    return (lhs.graph()) == (rhs.graph());
  }

  template <typename G, typename F>
  bool
  operator<=(const p_vertices<G,F>& lhs, const p_vertices<G,F>& rhs)
  {
    return lhs == rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_SITE_SET_P_VERTICES_HH
