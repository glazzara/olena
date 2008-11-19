// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_UTIL_GRAPH_HH
# define MLN_UTIL_GRAPH_HH

/// \file mln/util/graph.hh
///
/// Definition of a basic undirected graph.

# include <mln/util/internal/graph_base.hh>
# include <mln/util/internal/graph_iter.hh>
# include <mln/util/internal/graph_nbh_iter.hh>
# include <mln/util/ord_pair.hh>


namespace mln
{

  namespace util
  {
    /// Forward declaration.
    class graph;
  }


  namespace internal
  {

    /// Data structure for \c mln::image2d<T>.
    template <>
    struct data<util::graph>
    {
      typedef util::graph G;
      typedef std::vector<std::vector<unsigned> > vertices_t;
      typedef std::vector<util::ord_pair<unsigned> > edges_t;
      typedef std::set<util::ord_pair<unsigned> > edges_set_t;

      data();
      /// Constructor.
      /// Allocate enough space in order to support \p nvertices vertices.
      data(unsigned nvertices);
      ~data();

      /// The vertices.
      vertices_t vertices_;
      /// The edges.
      edges_t edges_;
      /// An index of the set of edges, for fast-access purpose.
      edges_set_t edges_set_;
    };

  } // end of namespace mln::internal


  namespace util
  {

    /// Undirected graph.
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
      std::pair<unsigned, unsigned> add_vertices(unsigned n);

      /// Return the vertex whose id is \a v.
      /// \{
      vertex_t vertex(unsigned id_v) const;
      /// \}

      /// Return the number of vertices in the graph.
      size_t v_nmax() const;

      /// Check whether a vertex id \p id_v exists in the graph.
      bool has_v(unsigned id_v) const;
      /// Check whether an edge \p v exists in the graph.
      template <typename G>
      bool has_v(const util::vertex<G>& v) const;


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
      /// Add an edge.
      ///
      /// \return The id of the new edge if it does not exist yet;
      /// otherwise, return <tt>mln_max(unsigned)</tt>.
      unsigned add_edge(unsigned id_v1, unsigned id_v2);

      /// Return the edge whose id is \a e.
      edge_t edge(unsigned e) const;

      /// Return the number of edges in the graph.
      size_t e_nmax() const;

      /// Return whether \p id_e is in the graph.
      bool has_e(unsigned id_e) const;
      /// Return whether \p e is in the graph.
      template <typename G>
      bool has_e(const util::edge<G>& e) const;


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

    };

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

    /*---------------.
    | Vertex related |
    `---------------*/

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
    std::pair<unsigned, unsigned>
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
    graph::vertex(unsigned id_v) const
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
    graph::has_v(unsigned id_v) const
    {
      return id_v < data_->vertices_.size();
    }

    template <typename G>
    inline
    bool
    graph::has_v(const util::vertex<G>& v) const
    {
      return v.graph().is_subgraph_of(*this) && has_v(v.id());
    }

    inline
    size_t
    graph::v_nmax_nbh_edges(unsigned id_v) const
    {
      mln_precondition(has_v(id_v));
      return data_->vertices_[id_v].size();
    }

    inline
    unsigned
    graph::v_ith_nbh_edge(unsigned id_v, unsigned i) const
    {
      mln_precondition(has_v(id_v));
      if (i >= v_nmax_nbh_edges(id_v))
        return v_nmax();
      return data_->vertices_[id_v][i];
    }

    inline
    size_t
    graph::v_nmax_nbh_vertices(unsigned id_v) const
    {
      mln_precondition(has_v(id_v));
      return v_nmax_nbh_edges(id_v);
    }

    inline
    unsigned
    graph::v_ith_nbh_vertex(unsigned id_v, unsigned i) const
    {
      mln_precondition(has_v(id_v));

      unsigned id_e = v_ith_nbh_edge(id_v, i);
      return v_other(id_e, id_v);
     }


    /*--------------.
    | Edges related |
    `---------------*/

    inline
    unsigned
    graph::add_edge(unsigned id_v1, unsigned id_v2)
    {
      // Does this edge already exist in the graph?
      edge_data_t edge(id_v1, id_v2);
      if (data_->edges_set_.find(edge) != data_->edges_set_.end ())
        {
          // Return the erroneous value.
          return mln_max(unsigned);
        }
      else
        {
          // Otherwise insert it into the graph.
          /* FIXME: This is not thread-proof (these two lines should
             form an atomic section).  */
          data_->edges_.push_back(edge);
          unsigned id = data_->edges_.size() - 1;

          // Update the set of edges.
          data_->edges_set_.insert(edge);
          data_->vertices_[edge.first()].push_back(id);
          data_->vertices_[edge.second()].push_back(id);

          return id;
        }
    }

    inline
    graph::edge_t
    graph::edge(unsigned e) const
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
    graph::has_e(unsigned id_e) const
    {
      return id_e < data_->edges_.size();
    }

    template <typename G>
    inline
    bool
    graph::has_e(const util::edge<G>& e) const
    {
      return e.graph().is_subgraph_of(*this) && has_e(e.id());
    }

    inline
    unsigned
    graph::v1(unsigned id_e) const
    {
      mln_precondition(has_e(id_e));
      return data_->edges_[id_e].first();
    }

    inline
    unsigned
    graph::v2(unsigned id_e) const
    {
      mln_precondition(has_e(id_e));
      return data_->edges_[id_e].second();
    }

    inline
    size_t
    graph::e_nmax_nbh_edges(unsigned id_e) const
    {
      mln_precondition(has_e(id_e));
      return v_nmax_nbh_edges(v1(id_e)) + v_nmax_nbh_edges(v2(id_e));
    }

    inline
    unsigned
    graph::e_ith_nbh_edge(unsigned id_e, unsigned i) const
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
      return g.graph_id() == this->graph_id();
    }

  } // end of namespace mln::util

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_UTIL_GRAPH_HH
