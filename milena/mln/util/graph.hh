// Copyright (C) 2007, 2008, 2009, 2010, 2012 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_UTIL_GRAPH_HH
# define MLN_UTIL_GRAPH_HH

/// \file
///
/// Definition of a basic undirected graph.

# include <mln/util/internal/graph_base.hh>
# include <mln/util/internal/graph_iter.hh>
# include <mln/util/internal/graph_nbh_iter.hh>
# include <mln/util/vertex.hh>
# include <mln/util/edge.hh>
# include <mln/util/ord_pair.hh>

namespace mln
{

  /// Forward declaration.
  namespace util { class graph; }


  namespace internal
  {

    /*!
      \internal
      \brief Data structure for mln::util::graph.
    */
    template <>
    struct data<util::graph>
    {
      typedef util::graph G;
      typedef std::vector<std::vector<util::edge_id_t> > vertices_t;
      typedef std::vector<util::ord_pair<util::vertex_id_t> > edges_t;
      typedef std::set<util::ord_pair<util::vertex_id_t> > edges_set_t;

      data();
      /// Constructor.
      /// Allocate enough space in order to support \p nvertices vertices.
      data(unsigned nvertices);
      ~data();

      /// The vertices.
      vertices_t vertices_;
      /// The edges.
      edges_t edges_;

# ifndef NDEBUG
      /// An index of the set of edges, for fast-access purpose.
      edges_set_t edges_set_;
# endif // ! NDEBUG
    };

  } // end of namespace mln::internal


  namespace util
  {

    /// \brief Undirected graph.
    ///
    /// \ingroup modgraph
    //
    class graph : public internal::graph_base<graph>
    {
      /// The super class.
      typedef internal::graph_base<graph> super;

      typedef super::vertex_data_t vertex_data_t;
      typedef super::edge_data_t edge_data_t;

    public:
      /// The type of the set of vertices.
      typedef std::vector<vertex_data_t> vertices_t;

      /// The type of the set of edges.
      typedef std::vector<edge_data_t> edges_t;
      /// A set to test the presence of a given edge.
      typedef std::set<edge_data_t> edges_set_t;

      /// Iterator types
      /// \{
      /// Vertex iterators
      /// \{
      typedef mln::internal::vertex_fwd_iterator<graph> vertex_fwd_iter;
      typedef mln::internal::vertex_bkd_iterator<graph> vertex_bkd_iter;
      typedef vertex_fwd_iter vertex_iter;
      /// \}

      /// Vertex centered edge iterators
      /// \{
      typedef mln::internal::vertex_nbh_edge_fwd_iterator<graph> vertex_nbh_edge_fwd_iter;
      typedef mln::internal::vertex_nbh_edge_bkd_iterator<graph> vertex_nbh_edge_bkd_iter;
      typedef vertex_nbh_edge_fwd_iter vertex_nbh_edge_iter;
      /// \}

      /// Vertex centered vertex iterators
      /// \{
      typedef mln::internal::vertex_nbh_vertex_fwd_iterator<graph> vertex_nbh_vertex_fwd_iter;
      typedef mln::internal::vertex_nbh_vertex_bkd_iterator<graph> vertex_nbh_vertex_bkd_iter;
      typedef vertex_nbh_vertex_fwd_iter vertex_nbh_vertex_iter;
      /// \}

      /// Edge iterators
      /// \{
      typedef mln::internal::edge_fwd_iterator<graph> edge_fwd_iter;
      typedef mln::internal::edge_bkd_iterator<graph> edge_bkd_iter;
      typedef edge_fwd_iter edge_iter;
      /// \}

      /// Edge centered edge iterators.
      /// \{
      typedef mln::internal::edge_nbh_edge_fwd_iterator<graph> edge_nbh_edge_fwd_iter;
      typedef mln::internal::edge_nbh_edge_bkd_iterator<graph> edge_nbh_edge_bkd_iter;
      typedef edge_nbh_edge_fwd_iter edge_nbh_edge_iter;
      /// \}
      /// \}

      /// Constructor.
      graph();
      /// Construct a graph with \p nvertices vertices.
      graph(unsigned nvertices);

      /// Vertex oriented.
      /// \{

      /// Shortcuts factoring the insertion of vertices and edges.
      /// \{

      /// Add a vertex.
      ///
      /// \return The id of the new vertex.
      unsigned add_vertex();

      /// Add \p n vertices to the graph.
      ///
      /// \return A range of vertex ids.
      std::pair<vertex_id_t, vertex_id_t> add_vertices(unsigned n);

      /// Return the vertex whose id is \a v.
      /// \{
      vertex_t vertex(vertex_id_t id_v) const;
      /// \}

      /// Return the number of vertices in the graph.
      // FIXME: Rename as nvertices.
      size_t v_nmax() const;

      /// Check whether a vertex id \p id_v exists in the graph.
      // FIXME: Is the `_v' suffix really needed?
      bool has_v(const vertex_id_t& id_v) const;


      /// Return the number of adjacent edges of vertex \p id_v.
      size_t v_nmax_nbh_edges(const vertex_id_t& id_v) const;

      /// Returns the \p i th edge adjacent to the vertex \p id_v.
      edge_id_t
      v_ith_nbh_edge(const vertex_id_t& id_v,
		     unsigned i) const;

      /// Return the number of adjacent vertices of vertex \p id_v.
      size_t v_nmax_nbh_vertices(const vertex_id_t& id_v) const;

      /// Returns the \p i th vertex adjacent to the vertex \p id_v.
      vertex_id_t v_ith_nbh_vertex(const vertex_id_t& id_v,
				   unsigned i) const;
      /// \}



      /// Edge oriented.
      /// \{
      /// Add an edge.
      ///
      /// \return The id of the new edge if it does not exist yet;
      /// otherwise, return <tt>mln_max(unsigned)</tt>.
      edge_id_t add_edge(const vertex_id_t& id_v1, const vertex_id_t& id_v2);

      /// Return the edge whose id is \a e.
      edge_t edge(const edge_id_t& e) const;


      /// Return the list of all edges.
      const std::vector<util::ord_pair<vertex_id_t> >& edges() const;

      /// Return the number of edges in the graph.
      // FIXME: Rename as nedges.
      size_t e_nmax() const;

      /// Return whether \p id_e is in the graph.
      // FIXME: Is the `_e' suffix really needed?
      bool has_e(const edge_id_t& id_e) const;

      /// Return the corresponding edge id if exists. If it is not, returns
      /// an invalid edge.
      edge_t edge(const vertex_t& v1, const vertex_t& v2) const;

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
      /// \}

    };

    std::ostream&
    operator<<(std::ostream& ostr, const graph& g);

  } // end of namespace mln::util

} // end of namespace mln




# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace internal
  {

    inline
    data<util::graph>::data()
    {
    }

    inline
    data<util::graph>::data(unsigned nvertices)
    {
      vertices_.resize(nvertices);
    }

    inline
    data<util::graph>::~data()
    {
    }

  } // end of namespace mln::internal

  namespace util
  {

    inline
    graph::graph()
    {
      this->data_ = new mln::internal::data<util::graph>();
    }

    inline
    graph::graph(unsigned nvertices)
    {
      this->data_ = new mln::internal::data<util::graph>(nvertices);
    }

    /*--------------------------.
    | Vertex-related services.  |
    `--------------------------*/

    inline
    unsigned
    graph::add_vertex()
    {
      /* FIXME: This is not thread-proof (these two lines should
         form an atomic section).  */
      data_->vertices_.resize(data_->vertices_.size() + 1);

      return v_nmax() - 1;
    }

    inline
    std::pair<vertex_id_t, vertex_id_t>
    graph::add_vertices(unsigned n)
    {
      /* FIXME: This is not thread-proof (these two lines should
         form an atomic section).  */
      data_->vertices_.resize(data_->vertices_.size() + n);

      return std::make_pair(v_nmax() - n,
			    v_nmax() - 1);
    }

    inline
    graph::vertex_t
    graph::vertex(vertex_id_t id_v) const
    {
      mln_assertion(has_v(id_v));
      return vertex_t(*this, id_v);
    }


    inline
    size_t
    graph::v_nmax() const
    {
      return data_->vertices_.size();
    }

    inline
    bool
    graph::has_v(const vertex_id_t& id_v) const
    {
      return id_v < data_->vertices_.size();
    }

    inline
    size_t
    graph::v_nmax_nbh_edges(const vertex_id_t& id_v) const
    {
      mln_precondition(has_v(id_v));
      return data_->vertices_[id_v].size();
    }

    inline
    edge_id_t
    graph::v_ith_nbh_edge(const vertex_id_t& id_v, unsigned i) const
    {
      mln_precondition(has_v(id_v));
      if (i >= v_nmax_nbh_edges(id_v))
        return edge_id_t();
      return data_->vertices_[id_v][i];
    }

    inline
    size_t
    graph::v_nmax_nbh_vertices(const vertex_id_t& id_v) const
    {
      mln_precondition(has_v(id_v));
      return v_nmax_nbh_edges(id_v);
    }

    inline
    vertex_id_t
    graph::v_ith_nbh_vertex(const vertex_id_t& id_v, unsigned i) const
    {
      mln_precondition(has_v(id_v));

      edge_id_t id_e = v_ith_nbh_edge(id_v, i);
      return v_other(id_e, id_v);
    }


    /*-------------------------.
    | Edges-related services.  |
    `-------------------------*/

    inline
    edge_id_t
    graph::add_edge(const vertex_id_t& id_v1, const vertex_id_t& id_v2)
    {
      mln_precondition(id_v1 != id_v2);
      mln_precondition(has_v(id_v1));
      mln_precondition(has_v(id_v2));

      // Does this edge already exist in the graph?
      edge_data_t edge(id_v1, id_v2);
      /* FIXME: This is not sound: the behavior of the algorithm
	 changes when NDEBUG is defined.  */
# ifndef NDEBUG
      if (data_->edges_set_.find(edge) != data_->edges_set_.end ())
        {
          // Return the erroneous value.
          return edge_id_t();
        }
      else
        {
# endif // ! NDEBUG
          // Otherwise insert it into the graph.
          /* FIXME: This is not thread-proof (these two lines should
             form an atomic section).  */
          data_->edges_.push_back(edge);
          edge_id_t id = data_->edges_.size() - 1;

          // Update the set of edges.
# ifndef NDEBUG
          data_->edges_set_.insert(edge);
# endif // ! NDEBUG
          data_->vertices_[edge.first()].push_back(id);
          data_->vertices_[edge.second()].push_back(id);

          return id;

# ifndef NDEBUG
        }
# endif // ! NDEBUG

    }

    inline
    const std::vector<util::ord_pair<vertex_id_t> >&
    graph::edges() const
    {
      return this->data_->edges_;
    }

    inline
    graph::edge_t
    graph::edge(const edge_id_t& e) const
    {
      mln_assertion(e < e_nmax());
      return edge_t(*this, e);
    }

    inline
    size_t
    graph::e_nmax() const
    {
      return data_->edges_.size();
    }

    inline
    bool
    graph::has_e(const edge_id_t& id_e) const
    {
      return id_e < data_->edges_.size();
    }

    inline
    graph::edge_t
    graph::edge(const vertex_t& v1, const vertex_t& v2) const
    {
      mln_precondition(has_v(v1));
      mln_precondition(has_v(v2));

      vertex_id_t
	id_v1 = v1.id(),
	id_v2 = v2.id();

      if (id_v1 > id_v2)
	std::swap(id_v1, id_v2);

      for (unsigned i = 0; i < data_->vertices_[id_v1].size(); ++i)
	if (data_->edges_[data_->vertices_[id_v1][i]].second() == id_v2)
	  return edge_t(*this, data_->vertices_[id_v1][i]);

      // Not edges available. Return an invalid edge.
      return edge_t();
    }

    inline
    vertex_id_t
    graph::v1(const edge_id_t& id_e) const
    {
      mln_precondition(has_e(id_e));
      return data_->edges_[id_e].first();
    }

    inline
    vertex_id_t
    graph::v2(const edge_id_t& id_e) const
    {
      mln_precondition(has_e(id_e));
      return data_->edges_[id_e].second();
    }

    inline
    size_t
    graph::e_nmax_nbh_edges(const edge_id_t& id_e) const
    {
      mln_precondition(has_e(id_e));
      return v_nmax_nbh_edges(v1(id_e)) + v_nmax_nbh_edges(v2(id_e));
    }

    inline
    edge_id_t
    graph::e_ith_nbh_edge(const edge_id_t& id_e, unsigned i) const
    {
      mln_precondition(has_e(id_e));
      if (i >= e_nmax_nbh_edges(id_e))
        return e_nmax();

      unsigned v1_nmax = v_nmax_nbh_edges(v1(id_e));
      if (i < v1_nmax)
        return v_ith_nbh_edge(v1(id_e), i);
      return  v_ith_nbh_edge(v2(id_e), i - v1_nmax);
    }


    template <typename G2>
    inline
    bool
    graph::is_subgraph_of(const G2& g) const
    {
      return g.id() == this->id();
    }

    // FIXME: move to graph_Base.
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const graph& g)
    {
      mln_vertex_iter_(graph) v(g);
      mln_vertex_nbh_edge_iter_(graph) e(v);
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


#endif // ! MLN_UTIL_GRAPH_HH
