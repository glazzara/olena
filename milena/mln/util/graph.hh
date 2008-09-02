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

#ifndef MLN_UTIL_GRAPH_HH
# define MLN_UTIL_GRAPH_HH

/// \file mln/util/graph.hh
/// \brief Definitions of undirected graphs.

# include <mln/util/internal/graph_base.hh>

namespace mln
{

  namespace util
  {
    /*-----------.
    | Fwd decl.  |
    `-----------*/

    /// \brief Undirected graph.
    template<typename V = void, typename E = void>
    class graph;

    /*--------------------.
    | graph<void, void>.  |
    `--------------------*/

    /// Specialization for undirected graphs with no data on vertices nor
    /// on edges.
    template <>
    class graph<void, void> : public internal::graph_base<void, void>
    {
    public:
      /// The super class.
      typedef internal::graph_base<void, void> super;

      /// \brief Add a vertex.
      ///
      /// \return The id of the new vertex.
      vertex_id add_vertex();
      /// \brief Add an edge between vertices with ids \p v1 and \p v2.
      ///
      /// \return The id of the new edge if it does not exist yet;
      /// otherwise, return <tt>mln_max(edge_id)</tt>.
      edge_id add_edge(vertex_id v1, vertex_id v2);
    };

    /*-----------------.
    | graph<V, void>.  |
    `-----------------*/

    /// Specialization for undirected graphs with data on vertices.
    template <typename V>
    class graph<V, void> : public internal::graph_base<V, void>
    {
    public:
      /// The super class.
      typedef internal::graph_base<V, void> super;

      /// \brief Add a vertex.
      ///
      /// \return The id of the new vertex.
      vertex_id add_vertex(const V& data);
      /// \brief Add an edge between vertices with ids \p v1 and \p v2.
      ///
      /// \return The id of the new edge if it does not exist yet;
      /// otherwise, return <tt>mln_max(edge_id)</tt>.
      edge_id add_edge(vertex_id v1, vertex_id v2);

      /// Return the data associated to vertex with id \a v.
      /// \{
      V& vertex_data(vertex_id v);
      const V& vertex_data(vertex_id v) const;
      /// \}
    };

    /*--------------.
    | graph<V, E>.  |
    `--------------*/

    /// Specialization for undirected graphs with data on vertices and
    /// edges.
    template <typename V, typename E>
    class graph : public internal::graph_base<V, E>
    {
    public:
      /// The super class.
      typedef internal::graph_base<V, E> super;

      /// \brief Add a vertex.
      ///
      /// \return The id of the new vertex.
      vertex_id add_vertex(const V& data);
      /// \brief Add an edge between vertices with ids \p v1 and \p v2.
      ///
      /// \return The id of the new edge if it does not exist yet;
      /// otherwise, return <tt>mln_max(edge_id)</tt>.
      edge_id add_edge(vertex_id v1, vertex_id v2, const E& data);

      /// Return the data associated to vertex with id \a v.
      /// \{
      V& vertex_data(vertex_id v);
      const V& vertex_data(vertex_id v) const;
      /// \}

      /// Return the data associated to the edge with id \a e.
      /// \{
      E& edge_data(edge_id e);
      const E& edge_data(edge_id e) const;
      /// \}

      /// Return the data associated to the edge between vertices with
      /// ids \a v1 and \a v2.
      /// \{
      E& edge_data(vertex_id v1, vertex_id v2);
      const E& edge_data(vertex_id v1, vertex_id v2) const;
      /// \}
    };



# ifndef MLN_INCLUDE_ONLY

    /*--------------------.
    | graph<void, void>.  |
    `--------------------*/

    /* Note that definition of members from fully specialized
       template classes are not preceded by `template<>'.  */
    inline
    vertex_id
    graph<void, void>::add_vertex()
    {
      return super::add_vertex_(new util::vertex<void>);
    }

    /* Note that ddefinition of members from fully specialized
       template classes are not preceded by `template<>'.  */
    inline
    edge_id
    graph<void, void>::add_edge(vertex_id v1, vertex_id v2)
    {
      mln_assertion(v1 < this->nvertices());
      mln_assertion(v2 < this->nvertices());
      return super::add_edge_(new util::edge<void>(v1, v2));
    }

    /*-----------------.
    | graph<V, void>.  |
    `-----------------*/

    template<typename V>
    inline
    vertex_id
    graph<V, void>::add_vertex(const V& data)
    {
      return super::add_vertex_(new util::vertex<V>(data));
    }

    template<typename V>
    inline
    edge_id
    graph<V, void>::add_edge(vertex_id v1, vertex_id v2)
    {
      mln_assertion(v1 < this->nvertices());
      mln_assertion(v2 < this->nvertices());
      return super::add_edge_(new util::edge<void>(v1, v2));
    }

    template <class V>
    inline
    V&
    graph<V, void>::vertex_data(vertex_id v)
    {
      mln_assertion(v < this->nvertices());
      return this->vertices_[v]->data;
    }

    template<typename V>
    inline
    const V&
    graph<V, void>::vertex_data(vertex_id v) const
    {
      mln_assertion(v < this->nvertices());
      return this->vertices_[v]->data;
    }


    /*--------------.
    | graph<V, E>.  |
    `--------------*/

    template<typename V, typename E>
    inline
    vertex_id
    graph<V, E>::add_vertex(const V& data)
    {
      return super::add_vertex_(new util::vertex<V>(data));
    }

    template<typename V, typename E>
    inline
    edge_id
    graph<V, E>::add_edge(vertex_id v1, vertex_id v2, const E& data)
    {
      mln_assertion(v1 < this->nvertices());
      mln_assertion(v2 < this->nvertices());
      return super::add_edge_(new util::edge<E>(v1, v2, data));
    }

    template<typename V, typename E>
    inline
    V&
    graph<V, E>::vertex_data(vertex_id v)
    {
      mln_assertion(v < this->nvertices());
      return this->vertices_[v]->data;
    }

    template<typename V, typename E>
    inline
    const V&
    graph<V, E>::vertex_data(vertex_id v) const
    {
      mln_assertion(v < this->nvertices());
      return this->vertices_[v]->data;
    }

    template<typename V, typename E>
    inline
    E&
    graph<V, E>::edge_data(edge_id e)
    {
      mln_assertion(e < this->nedges());
      return this->edges_[e]->data;
    }

    template<typename V, typename E>
    inline
    const E&
    graph<V, E>::edge_data(edge_id e) const
    {
      mln_assertion(e < this->nedges());
      return this->edges_[e]->data;
    }

    template<typename V, typename E>
    inline
    E&
    graph<V, E>::edge_data(vertex_id v1, vertex_id v2)
    {
      mln_assertion(v1 < this->nvertices());
      mln_assertion(v2 < this->nvertices());
      ordpair_<vertex_id> vertex_pair (v1, v2);
      std::vector<edge_id>& edges_ids = this->vertices_[v1]->edges;
      for (std::vector<edge_id>::iterator e = edges_ids.begin();
	   e != edges_ids.end(); ++e)
	  if (this->edges_[*e] == vertex_pair)
	    return this->edges_[*e]->data;
      // If no edge between V1 and V2 was found, abort.
      abort();
    }

    template<typename V, typename E>
    inline
    const E&
    graph<V, E>::edge_data(vertex_id v1, vertex_id v2) const
    {
      mln_assertion(v1 < this->nvertices());
      mln_assertion(v2 < this->nvertices());
      ordpair_<vertex_id> vertex_pair (v1, v2);
      const std::vector<edge_id>& edges_ids = this->vertices_[v1]->edges;
      for (std::vector<edge_id>::const_iterator e = edges_ids.begin();
	   e != edges_ids.end(); ++e)
	  if (this->edges_[*e] == vertex_pair)
	    return this->edges_[*e]->data;
      // If no edge between V1 and V2 was found, abort.
      abort();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_GRAPH_HH
