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

#ifndef MLN_CORE_SITE_SET_P_VERTICES_HH
# define MLN_CORE_SITE_SET_P_VERTICES_HH

  /// \file mln/core/site_set/p_vertices.hh
  /// \brief Definition of a point set based on a graph.

# include <mln/core/internal/site_set_base.hh>
# include <mln/core/site_set/p_graph_piter.hh>
# include <mln/util/internal/graph_vertex_psite.hh>
# include <mln/util/graph.hh>

  //# include <mln/util/tracked_ptr.hh>

  //# include <mln/core/image/graph_psite.hh>
  //# include <mln/core/site_set/p_vertices_piter.hh>

namespace mln
{
  // Forward declaration.
  template <typename G, typename F> struct p_vertices;

  namespace trait
  {
    template <typename G, typename F>
      struct site_set_< p_vertices<G, F> >
      {
	typedef trait::site_set::nsites::known   nsites;
	// FIXME: !
	typedef trait::site_set::bbox::unknown   bbox;
	typedef trait::site_set::contents::fixed contents;
	typedef trait::site_set::arity::unique   arity;
      };
  } // end of namespace mln::trait


  template <typename G, typename F>
  class p_vertices
    : public internal::site_set_base_< typename F::result, p_vertices<G, F> >
  {
    typedef util::vertex<G> vertex_t;

    typedef p_vertices<G, F> self_;
    typedef internal::site_set_base_< typename F::result, self_ > super_;

  public:

    /// Type of the graph this site set is based on.
    typedef G graph_t;

    /// \brief Construct a graph psite set from a graph of points.
    /// \{
    p_vertices();

    /// \param gr The graph upon which the graph psite set is built.
    /// \param f the function which maps a vertex to a site.
    p_vertices(const graph_t& gr, const F& f);
    /// \}

    /// Associated types.
    /// \{
    /// Element associated type.
    typedef mln_site(super_) element;
    /// Point_Site associated type.
    typedef internal::vertex_psite<G, F> psite;

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
    /* FIXME: Return type should be std::size_t (see
       mln/core/concept/site_set.hh). */
    size_t nsites() const;

    /// Return The number of vertices in the graph.
    std::size_t nvertices() const;

    /// Is this site set valid?
    bool is_valid() const;
    /// Invalidate this site set.
    void invalidate();

    /// Does this site set has \a p?
    bool has(const psite& p) const;

    /// Does this site set has \a v?
    template <typename G2>
    bool has(const util::vertex<G2>& p) const;

    // FIXME: Dummy.
    std::size_t memory_size() const;

    /// Return the value associated to an element of this site set.
    /// \{
    const mln_result(F)& operator()(const psite& p) const;
    const mln_result(F)& operator()(const util::vertex<G>& p) const;
    const mln_result(F)& operator()(unsigned id_v) const;
    /// \}

    /// Accessors.
    /// \{
    /// Return the graph associated to this site set (const version)
    const graph_t& graph() const;
    /// Return the association function.
    const F& function() const;
    /// \}

  private:
    mlc_const(graph_t)* g_;
    F f_;
  };


  /// \brief Comparison between two mln::p_vertices's.
  ///
  /// Two mln::p_vertices's are considered equal if they share the
  /// same graph.
  template <typename G, typename F>
  bool
  operator==(const p_vertices<G, F>& lhs, const p_vertices<G, F>& rhs);


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
  operator<=(const p_vertices<G, F>& lhs, const p_vertices<G, F>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <typename G, typename F>
  inline
  p_vertices<G, F>::p_vertices()
    : f_(0)
  {
  }

  template <typename G, typename F>
  inline
  p_vertices<G, F>::p_vertices(const graph_t& g, const F& f)
    : g_ (&g), f_(f)
  {
  }

  template <typename G, typename F>
  inline
  size_t
  p_vertices<G, F>::nsites() const
  {
    return nvertices();
  }

  template <typename G, typename F>
  inline
  std::size_t
  p_vertices<G, F>::nvertices() const
  {
    return this->g_->v_nmax();
  }

  template <typename G, typename F>
  inline
  bool
  p_vertices<G, F>::is_valid() const
  {
    return g_ != 0;
  }

  template <typename G, typename F>
  inline
  void
  p_vertices<G, F>::invalidate()
  {
    g_ = 0;
  }

  template <typename G, typename F>
  inline
  bool
  p_vertices<G, F>::has(const psite& p) const
  {
    mln_precondition(is_valid());
    return has(p.v());
  }

  template <typename G, typename F>
  template <typename G2>
  inline
  bool
  p_vertices<G, F>::has(const util::vertex<G2>& p) const
  {
    mln_precondition(is_valid());
    return
      // Check whether P is compatible with this psite set.
      (p.graph() == *g_) &&
      // Check that the vertex id of P belongs to the range of valid
      // vertex ids.
      (p.is_valid());
  }

  template <typename G, typename F>
  inline
  std::size_t
  p_vertices<G, F>::memory_size() const
  {
    // FIXME: Dummy; implement (see other site sets).
    abort();
    return 0;
  }

  template <typename G, typename F>
  inline
  const mln_result(F)&
  p_vertices<G, F>::operator()(const psite& p) const
  {
    mln_precondition(g_.has(p.v()));
    return (*this)(p.v().id());
  }

  template <typename G, typename F>
  inline
  const mln_result(F)&
  p_vertices<G, F>::operator()(const util::vertex<G>& v) const
  {
    mln_precondition(g_->has_v(v));
    return (*this)(v.id());
  }

  template <typename G, typename F>
  inline
  const mln_result(F)&
  p_vertices<G, F>::operator()(unsigned id_v) const
  {
    mln_precondition(g_->has_v(id_v));
    return f_(id_v);
  }

  template <typename G, typename F>
  inline
  const typename p_vertices<G, F>::graph_t&
  p_vertices<G, F>::graph() const
  {
    mln_precondition(is_valid());
    return *g_;
  }

  template <typename G, typename F>
  inline
  const F&
  p_vertices<G, F>::function() const
  {
    return f_;
  }

  template <typename G, typename F>
  bool
  operator==(const p_vertices<G, F>& lhs, const p_vertices<G, F>& rhs)
  {
    return (*lhs.g_) == (*rhs.g_);
  }

  template <typename G, typename F>
  bool
  operator<=(const p_vertices<G, F>& lhs, const p_vertices<G, F>& rhs)
  {
    return lhs == rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_CORE_SITE_SET_P_VERTICES_HH
