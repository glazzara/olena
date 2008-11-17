// Copyright (C) 2008 EPITA Research and Development Laboratory
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_UTIL_DUAL_GRAPH_HH
# define MLN_UTIL_DUAL_GRAPH_HH

/// \file mln/util/dual_graph.hh
/// Definitions of undirected graphs.

# include <mln/util/internal/graph_base.hh>
# include <mln/util/internal/graph_iter.hh>
# include <mln/util/internal/graph_nbh_iter.hh>
# include <mln/util/ord_pair.hh>

namespace mln
{

  namespace util
  {
    /// Fwd declaration.
    template <typename G>
    class dual_graph;
  }


  namespace internal
  {

    /// Data structure for \c mln::image2d<T>.
    template <typename G>
    struct data< util::dual_graph<G> >
    {

      typedef std::vector<std::vector<unsigned> > vertices_t;
      typedef std::vector<util::ord_pair<unsigned> > edges_t;

      data();
      data(const G& g);
      ~data();

      G g_;
      /// The vertices.
      vertices_t vertices_;
      /// The edges.
      edges_t edges_;
    };

  } // end of namespace mln::internal


  namespace util
  {

    /// Undirected dual graph of a graph of type \tparam G.
    template <typename G>
    class dual_graph : public internal::graph_base< dual_graph<G> >
    {
      /// The super class.
      typedef internal::graph_base< dual_graph<G> > super;

      typedef typename super::vertex_t vertex_t;
      typedef typename super::edge_t edge_t;

      typedef typename super::vertex_data_t vertex_data_t;
      typedef typename super::edge_data_t edge_data_t;

    public:
      /// The type of the set of vertices.
      typedef std::vector<vertex_data_t> vertices_t;

      /// The type of the set of edges.
      typedef std::vector<edge_data_t> edges_t;

      /// Iterator types
      /// \{
      /// Vertex iterators
      /// \{
      typedef mln::internal::vertex_fwd_iterator< dual_graph<G> >
	vertex_fwd_iter;
      typedef mln::internal::vertex_bkd_iterator< dual_graph<G> >
	vertex_bkd_iter;
      typedef vertex_fwd_iter vertex_iter;

      typedef mln::internal::edge_fwd_iterator< dual_graph<G> >
	edge_fwd_iter;
      typedef mln::internal::edge_bkd_iterator< dual_graph<G> >
	edge_bkd_iter;
      typedef edge_fwd_iter edge_iter;

      /// \}
      /// \}

      dual_graph();
      dual_graph(const G& g);

      /// Vertex oriented.
      /// \{
      /// Shortcuts factoring the insertion of vertices and edges.
      /// \{
      /// Return the vertex whose id is \a v.
      /// \{
      vertex_t vertex(unsigned id_v) const;
      /// \}

      /// Return the number of vertices in the graph.
      size_t v_nmax() const;

      /// Check whether a vertex id \p id_v exists in the graph.
      bool has_v(unsigned id_v) const;
      /// Check whether an edge \p v exists in the graph.
      template <typename G2>
      bool has_v(const util::vertex<G2>& v) const;


      /// Return the number of adjacent edges of vertex \p id_v.
      size_t v_nmax_nbh_edges(unsigned id_v) const;

      /// Returns the \p i th edge adjacent to the vertex \p id_v.
      unsigned v_ith_nbh_edge(unsigned id_v, unsigned i) const;

      /// Return the number of adjacent vertices of vertex \p id_v.
      size_t v_nmax_nbh_vertices(unsigned id_v) const;

      /// Returns the \p i th vertex adjacent to the vertex \p id_v.
      unsigned v_ith_nbh_vertex(unsigned id_v, unsigned i) const;
      /// \}




      /// Edge oriented.
      /// \{
      /// Return the edge whose id is \a e.
      edge_t edge(unsigned e) const;

      /// Return the number of edges in the graph.
      size_t e_nmax() const;

      /// Return whether \p id_e is in the graph.
      bool has_e(unsigned id_e) const;
      /// Return whether \p e is in the graph.
      template <typename G2>
      bool has_e(const util::edge<G2>& e) const;


      /// Return the first vertex associated to the edge \p id_e.
      unsigned v1(unsigned id_e) const;

      /// Return the second vertex associated to edge \p id_e
      unsigned v2(unsigned id_e) const;

      /// Return the number max of adjacent edge, given an edge \p id_e.
      size_t e_nmax_nbh_edges(unsigned id_e) const;

      /// Return the \p i th edge adjacent to the edge \p id_e.
      unsigned e_ith_nbh_edge(unsigned id_e, unsigned i) const;

      /// Return whether this graph is a subgraph
      /// Return true if g and *this have the same graph_id.
      template <typename G2>
      bool is_subgraph_of(const G2& g) const;
      /// \}

    protected:
      using super::data_;
    };

  } // end of namespace mln::util

} // end of namespace mln

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace internal
  {

    template <typename G>
    inline
    data< util::dual_graph<G> >::data()
    {
    }

    template <typename G>
    inline
    data< util::dual_graph<G> >::data(const G& g)
    {
      g_ = g;
      vertices_.resize(g.e_nmax());
      mln_edge_iter(G) e(g);
      mln_edge_nbh_edge_iter(G) ne(e);

      for_all(e)
	for_all(ne)
	{
	  vertices_[e].push_back(edges_.size());
	  edges_.push_back(util::ord_pair<unsigned>(e, ne));
	}
    }

    template <typename G>
    inline
    data< util::dual_graph<G> >::~data()
    {
    }

  } // end of namespace mln::internal

  namespace util
  {

    template <typename G>
    inline
    dual_graph<G>::dual_graph()
    {
      this->data_ = new mln::internal::data< util::dual_graph<G> >();
    }

    template <typename G>
    inline
    dual_graph<G>::dual_graph(const G& g)
    {
      this->data_ = new mln::internal::data< util::dual_graph<G> >(g);
    }

    /*---------------.
    | Vertex related |
    `---------------*/

    template <typename G>
    inline
    typename dual_graph<G>::vertex_t
    dual_graph<G>::vertex(unsigned id_v) const
    {
      mln_assertion(has_v(id_v));
      return vertex_t(*this, id_v);
    }


    template <typename G>
    inline
    size_t
    dual_graph<G>::v_nmax() const
    {
      return data_->g_.e_nmax();
    }

    template <typename G>
    inline
    bool
    dual_graph<G>::has_v(unsigned id_v) const
    {
      return data_->g_.has_e(id_v);
    }

    template <typename G>
    template <typename G2>
    inline
    bool
    dual_graph<G>::has_v(const util::vertex<G2>& v) const
    {
      //FIXME: not sure...
      return v.graph().is_subgraph_of(*this) && has_v(v.id());
    }

    template <typename G>
    inline
    size_t
    dual_graph<G>::v_nmax_nbh_edges(unsigned id_v) const
    {
      mln_precondition(has_v(id_v));
      return data_->g_.e_nmax_nbh_edges(id_v);
    }

    template <typename G>
    inline
    unsigned
    dual_graph<G>::v_ith_nbh_edge(unsigned id_v, unsigned i) const
    {
      mln_precondition(has_v(id_v));
      if (i >= v_nmax_nbh_edges(id_v))
        return v_nmax();
      return data_->vertices_[id_v][i];
    }

    template <typename G>
    inline
    size_t
    dual_graph<G>::v_nmax_nbh_vertices(unsigned id_v) const
    {
      mln_precondition(has_v(id_v));
      return v_nmax_nbh_edges(id_v);
    }

    template <typename G>
    inline
    unsigned
    dual_graph<G>::v_ith_nbh_vertex(unsigned id_v, unsigned i) const
    {
      mln_precondition(has_v(id_v));

      unsigned id_e = this->v_ith_nbh_edge(id_v, i);
      return this->v_other(id_e, id_v);
     }


    /*--------------.
    | Edges related |
    `---------------*/

    template <typename G>
    inline
    typename dual_graph<G>::edge_t
    dual_graph<G>::edge(unsigned e) const
    {
      mln_assertion(e < e_nmax());
      return edge_t(*this, e);
    }

    template <typename G>
    inline
    size_t
    dual_graph<G>::e_nmax() const
    {
      return data_->edges_.size();
    }

    template <typename G>
    inline
    bool
    dual_graph<G>::has_e(unsigned id_e) const
    {
      return id_e < data_->edges_.size();
    }

    template <typename G>
    template <typename G2>
    inline
    bool
    dual_graph<G>::has_e(const util::edge<G2>& e) const
    {
      return e.graph().is_subgraph_of(*this) && has_e(e.id());
    }

    template <typename G>
    inline
    unsigned
    dual_graph<G>::v1(unsigned id_e) const
    {
      mln_precondition(has_e(id_e));
      return data_->edges_[id_e].first();
    }

    template <typename G>
    inline
    unsigned
    dual_graph<G>::v2(unsigned id_e) const
    {
      mln_precondition(has_e(id_e));
      return data_->edges_[id_e].second();
    }

    template <typename G>
    inline
    size_t
    dual_graph<G>::e_nmax_nbh_edges(unsigned id_e) const
    {
      mln_precondition(has_e(id_e));
      return v_nmax_nbh_edges(v1(id_e)) + v_nmax_nbh_edges(v2(id_e));
    }

    template <typename G>
    inline
    unsigned
    dual_graph<G>::e_ith_nbh_edge(unsigned id_e, unsigned i) const
    {
      mln_precondition(has_e(id_e));
      if (i >= e_nmax_nbh_edges(id_e))
        return e_nmax();

      unsigned v1_nmax = v_nmax_nbh_edges(v1(id_e));
      if (i < v1_nmax)
        return v_ith_nbh_edge(v1(id_e), i);
      return  v_ith_nbh_edge(v2(id_e), i - v1_nmax);
    }


    template <typename G>
    template <typename G2>
    inline
    bool
    dual_graph<G>::is_subgraph_of(const G2& g) const
    {
      return g.graph_id() == this->graph_id();
    }

  } // end of namespace mln::util

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_UTIL_DUAL_GRAPH_HH
