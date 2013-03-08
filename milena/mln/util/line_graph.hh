// Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
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

#ifndef MLN_UTIL_LINE_GRAPH_HH
# define MLN_UTIL_LINE_GRAPH_HH

/// \file
///
/// Definition of an (undirected) line graph (adaptor).

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
    class line_graph;
  }


  namespace internal
  {

    /*!
      \internal
      \brief Data structure for mln::util::line_graph<G>.
    */
    template <typename G>
    struct data< util::line_graph<G> >
    {
      typedef util::line_graph<G> lg_t;
      typedef std::vector<std::vector<util::edge_id_t> > vertices_t;
      typedef std::vector<util::ord_pair<util::vertex_id_t> > edges_t;

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

    /// \brief Undirected line graph of a graph of type \p G.
    ///
    /// \ingroup modgraph
    template <typename G>
    class line_graph : public internal::graph_base< line_graph<G> >
    {
      /// The super class.
      typedef internal::graph_base< line_graph<G> > super;

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
      typedef mln::internal::vertex_fwd_iterator< line_graph<G> >
	vertex_fwd_iter;
      typedef mln::internal::vertex_bkd_iterator< line_graph<G> >
	vertex_bkd_iter;
      typedef vertex_fwd_iter vertex_iter;
      /// \}

      /// Edge iterators
      /// \{
      typedef mln::internal::edge_fwd_iterator< line_graph<G> >
	edge_fwd_iter;
      typedef mln::internal::edge_bkd_iterator< line_graph<G> >
	edge_bkd_iter;
      typedef edge_fwd_iter edge_iter;
      /// \}

      /// Edge nbh edge iterators
      /// \{
      typedef mln::internal::edge_nbh_edge_fwd_iterator< line_graph<G> >
	edge_nbh_edge_fwd_iter;
      typedef mln::internal::edge_nbh_edge_bkd_iterator< line_graph<G> >
	edge_nbh_edge_bkd_iter;
      typedef edge_nbh_edge_fwd_iter edge_nbh_edge_iter;
      /// \}

      /// Vertex nbh vertex iterators
      /// \{
      typedef mln::internal::vertex_nbh_vertex_fwd_iterator< line_graph<G> >
	vertex_nbh_vertex_fwd_iter;
      typedef mln::internal::vertex_nbh_vertex_bkd_iterator< line_graph<G> >
	vertex_nbh_vertex_bkd_iter;
      typedef vertex_nbh_vertex_fwd_iter vertex_nbh_vertex_iter;
      /// \}

      /// Vertex nbh edge iterators
      /// \{
      typedef mln::internal::vertex_nbh_edge_fwd_iterator< line_graph<G> >
	vertex_nbh_edge_fwd_iter;
      typedef mln::internal::vertex_nbh_edge_bkd_iterator< line_graph<G> >
	vertex_nbh_edge_bkd_iter;
      typedef vertex_nbh_edge_fwd_iter vertex_nbh_edge_iter;
      /// \}
      /// \}

      line_graph();
      line_graph(const G& g);

      /// Vertex oriented.
      /// \{
      /// Shortcuts factoring the insertion of vertices and edges.
      /// \{
      /// Return the vertex whose id is \a v.
      /// \{
      vertex_t vertex(const vertex_id_t& id_v) const;
      /// \}

      /// Return the number of vertices in the graph.
      // FIXME: Rename as nvertices.
      size_t v_nmax() const;

      /// Check whether a vertex id \p id_v exists in the line graph.
      // FIXME: Is the `_v' suffix really needed?
      bool has_v(const vertex_id_t& id_v) const;

      /// Check whether a vertex \p v exists in the line graph.
      template <typename G2>
      bool has(const util::vertex<G2>& v) const;


      /// Return the number of adjacent edges of vertex \p id_v.
      size_t v_nmax_nbh_edges(const vertex_id_t& id_v) const;

      /// Returns the \p i th edge adjacent to the vertex \p id_v.
      edge_id_t v_ith_nbh_edge(const vertex_id_t& id_v, unsigned i) const;

      /// Return the number of adjacent vertices of vertex \p id_v.
      size_t v_nmax_nbh_vertices(const vertex_id_t& id_v) const;

      /// Returns the \p i th vertex adjacent to the vertex \p id_v.
      vertex_id_t v_ith_nbh_vertex(const vertex_id_t& id_v, unsigned i) const;
      /// \}




      /// Edge oriented.
      /// \{
      /// Return the edge whose id is \a e.
      edge_t edge(const edge_id_t& e) const;

      /// Return the number of edges in the graph.
      // FIXME: Rename as nedges.
      size_t e_nmax() const;

      /// Return whether \p id_e is in the line graph.
      // FIXME: Is the `_e' suffix really needed?
      bool has_e(const util::edge_id_t& id_e) const;

      /// Return whether \p e is in the line graph.
      template <typename G2>
      bool has(const util::edge<G2>& e) const;


      /// Return the first vertex associated to the edge \p id_e.
      vertex_id_t v1(const edge_id_t& id_e) const;

      /// Return the second vertex associated to edge \p id_e
      vertex_id_t v2(const edge_id_t& id_e) const;

      /// Return the number max of adjacent edge, given an edge \p id_e.
      size_t e_nmax_nbh_edges(const edge_id_t& id_e) const;

      /// Return the \p i th edge adjacent to the edge \p id_e.
      edge_id_t e_ith_nbh_edge(const edge_id_t& id_e, unsigned i) const;

      /// Return whether this graph is a subgraph
      /// Return true if g and *this have the same graph_id.
      template <typename G2>
      bool is_subgraph_of(const G2& g) const;

      /// Return the underlying graph
      const G& graph() const;
      /// \}

    protected:
      using super::data_;
    };

    template <typename G>
    std::ostream&
    operator<<(std::ostream& ostr, const line_graph<G>& g);

  } // end of namespace mln::util

} // end of namespace mln

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace internal
  {

    template <typename G>
    inline
    data< util::line_graph<G> >::data()
    {
    }

    template <typename G>
    inline
    data< util::line_graph<G> >::data(const G& g)
    {
      g_ = g;

      // Initialize vertices and edges.
      // FIXME: use an adjacency matrix!!
      std::set<util::ord_pair<util::vertex_id_t> > edges_set;

      vertices_.resize(g.e_nmax());
      mln_edge_iter(G) e(g);
      mln_edge_nbh_edge_iter(G) ne(e);

      for_all(e)
      {
	util::vertex_id_t v1(e.id().value());
	for_all(ne)
	{
	  util::vertex_id_t v2(ne.id().value());
	  util::ord_pair<util::vertex_id_t> edge(v1, v2);
	  if (edges_set.find(edge) == edges_set.end())
	  {
	    vertices_[v1].push_back(edges_.size());
	    vertices_[v2].push_back(edges_.size());
	    edges_set.insert(edge);
	    edges_.push_back(edge);
	  }
	}
      }
    }

    template <typename G>
    inline
    data< util::line_graph<G> >::~data()
    {
    }

  } // end of namespace mln::internal

  namespace util
  {

    template <typename G>
    inline
    line_graph<G>::line_graph()
    {
      this->data_ = new mln::internal::data< util::line_graph<G> >();
    }

    template <typename G>
    inline
    line_graph<G>::line_graph(const G& g)
    {
      this->data_ = new mln::internal::data< util::line_graph<G> >(g);
    }

    /*---------------.
    | Vertex related |
    `---------------*/

    template <typename G>
    inline
    typename line_graph<G>::vertex_t
    line_graph<G>::vertex(const vertex_id_t& id_v) const
    {
      mln_assertion(has_v(id_v));
      return vertex_t(*this, id_v);
    }


    template <typename G>
    inline
    size_t
    line_graph<G>::v_nmax() const
    {
      return data_->g_.e_nmax();
    }

    template <typename G>
    inline
    bool
    line_graph<G>::has_v(const vertex_id_t& id_v) const
    {
      return data_->g_.has_v(id_v);
    }

    template <typename G>
    template <typename G2>
    inline
    bool
    line_graph<G>::has(const util::vertex<G2>& v) const
    {
      // FIXME: not sure...
      return v.graph().is_subgraph_of(*this) && has_v(v.id());
    }

    template <typename G>
    inline
    size_t
    line_graph<G>::v_nmax_nbh_edges(const vertex_id_t& id_v) const
    {
      mln_precondition(has_v(id_v));
      return data_->vertices_[id_v].size();
    }

    template <typename G>
    inline
    edge_id_t
    line_graph<G>::v_ith_nbh_edge(const vertex_id_t& id_v, unsigned i) const
    {
      mln_precondition(has_v(id_v));
      if (i >= v_nmax_nbh_edges(id_v))
        return v_nmax();
      return data_->vertices_[id_v][i];
    }

    template <typename G>
    inline
    size_t
    line_graph<G>::v_nmax_nbh_vertices(const vertex_id_t& id_v) const
    {
      mln_precondition(has_v(id_v));
      return v_nmax_nbh_edges(id_v);
    }

    template <typename G>
    inline
    vertex_id_t
    line_graph<G>::v_ith_nbh_vertex(const vertex_id_t& id_v, unsigned i) const
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
    typename line_graph<G>::edge_t
    line_graph<G>::edge(const edge_id_t& e) const
    {
      mln_assertion(e < e_nmax());
      return edge_t(*this, e);
    }

    template <typename G>
    inline
    size_t
    line_graph<G>::e_nmax() const
    {
      return data_->edges_.size();
    }

    template <typename G>
    inline
    bool
    line_graph<G>::has_e(const edge_id_t& id_e) const
    {
      return id_e < data_->edges_.size();
    }

    template <typename G>
    template <typename G2>
    inline
    bool
    line_graph<G>::has(const util::edge<G2>& e) const
    {
      return e.graph().is_subgraph_of(*this) && has_e(e.id());
    }

    template <typename G>
    inline
    vertex_id_t
    line_graph<G>::v1(const edge_id_t& id_e) const
    {
      mln_precondition(has_e(id_e));
      return data_->edges_[id_e].first();
    }

    template <typename G>
    inline
    vertex_id_t
    line_graph<G>::v2(const edge_id_t& id_e) const
    {
      mln_precondition(has_e(id_e));
      return data_->edges_[id_e].second();
    }

    template <typename G>
    inline
    size_t
    line_graph<G>::e_nmax_nbh_edges(const edge_id_t& id_e) const
    {
      mln_precondition(has_e(id_e));
      return v_nmax_nbh_edges(v1(id_e)) + v_nmax_nbh_edges(v2(id_e));
    }

    template <typename G>
    inline
    edge_id_t
    line_graph<G>::e_ith_nbh_edge(const edge_id_t& id_e, unsigned i) const
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
    line_graph<G>::is_subgraph_of(const G2& g) const
    {
      return g.id() == this->id();
    }

    template <typename G>
    inline
    const G&
    line_graph<G>::graph() const
    {
      return this->data_->g_;
    }

    // FIXME: move to graph_base
    template <typename G>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const line_graph<G>& g)
    {
      typedef line_graph<G> LG;
      mln_vertex_iter(LG) v(g);
      mln_vertex_nbh_edge_iter(LG) e(v);
      for_all(v)
      {
	ostr << "v(" << v << ") : ";
	for_all(e)
	  ostr << e << " ";
	ostr << std::endl;
      }

      return ostr;
    }

  } // end of namespace mln::util

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_UTIL_LINE_GRAPH_HH
