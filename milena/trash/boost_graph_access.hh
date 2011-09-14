// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/// \file mln/util/internal/boost_graph_access.hh
/// \brief boost_graph access operations.

#ifndef MLN_UTIL_INTERNAL_BOOST_GRAPH_ACCESS_HH_
# define MLN_UTIL_INTERNAL_BOOST_GRAPH_ACCESS_HH_


# include <utility>

/// fwd declaration
namespace mln
{
  namespace util
  {
    namespace internal
    {
      template <typename VertexProperty, typename EdgeProperty>
      class boost_graph;
    }
  }
}

namespace boost
{
  namespace mlnu = mln::util::internal;

  /// \brief Returns an iterator-range providing access to the vertex
  ///        set of graph g.
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::vertex_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::vertex_iterator>
  vertices(const mlnu::boost_graph<VProp, EProp>& g);


  /// \brief Returns an iterator-range providing access to
  ///        the edge set of graph g.
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::edge_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::edge_iterator>
  edges(const mlnu::boost_graph<VProp, EProp>& g);


  /// \brief Returns an iterator-range providing access to the vertices
  ///        adjacent to vertex u in graph g.
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::adjacency_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::adjacency_iterator>
  adjacent_vertices(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		    const mlnu::boost_graph<VProp, EProp>& g);


  /// \brief Returns an iterator-range providing access to the vertices in
  ///        graph g to which u is adjacent. (inv is for inverse.)
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::inv_adjacency_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::inv_adjacency_iterator>
  inv_adjacent_vertices(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
			const mlnu::boost_graph<VProp, EProp>& g);

  /// \brief Returns an iterator-range providing access access to
  ///        all edges incident on vertex u.
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator>
  out_edges(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	    const mlnu::boost_graph<VProp, EProp>& g);

  /// FIXME: this operation is undefined for undirected graph
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::in_edge_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::in_edge_iterator>
  in_edges(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	   const mlnu::boost_graph<VProp, EProp>& g);


  /// \brief Returns the source vertex of edge e.
  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  source(typename mlnu::boost_graph<VProp, EProp>::edge_descriptor e,
	 const mlnu::boost_graph<VProp, EProp>& g);

  /// \brief Returns the target vertex of edge e.
  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  target(typename mlnu::boost_graph<VProp, EProp>::edge_descriptor e,
	 const mlnu::boost_graph<VProp, EProp>& g);

  /// \brief Returns the number of edges leaving vertex u.
  template <typename V, typename E>
  typename mlnu::boost_graph<V, E>::degree_size_type
  out_degree(typename mlnu::boost_graph<V, E>::vertex_descriptor u,
	     const mlnu::boost_graph<V, E>& g);

  /// FIXME this operation is unavalaible for undirected graph.
  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::degree_size_type
  in_degree(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	    const mlnu::boost_graph<VProp, EProp>& g);

  /// \brief Returns the number of vertices in the graph g.
  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertices_size_type
  num_vertices(const mlnu::boost_graph<VProp, EProp>& g);

  /// \brief Returns the number of edges in the graph g.
  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::edges_size_type
  num_edges(const mlnu::boost_graph<VProp, EProp>& g);


  /// \brief Returns the nth vertex in the graph's vertex list.
  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  vertex(typename mlnu::boost_graph<VProp, EProp>::vertices_size_type n,
	 const typename mlnu::boost_graph<VProp, EProp>& g);


  /// \brief Returns an edge connecting vertex u to vertex v in graph g.
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::edge_descriptor,  bool>
  edge(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
       typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
       const mlnu::boost_graph<VProp, EProp>& g);

  /// \brief Returns a pair of out-edge iterators that give the range for all
  ///        the parallel edges from u to v.
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator>
  edge_range(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	     typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	     const mlnu::boost_graph<VProp, EProp>& g);

# ifndef MLN_INCLUDE_ONLY

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::vertex_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::vertex_iterator>
  vertices(const mlnu::boost_graph<VProp, EProp>& g)
  {
    return vertices(g.graph());
  }

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::edge_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::edge_iterator>
  edges(const mlnu::boost_graph<VProp, EProp>& g)
  {
    return edges(g.graph());
  }

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::adjacency_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::adjacency_iterator>
  adjacent_vertices(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		    const mlnu::boost_graph<VProp, EProp>& g)
  {
    return adjacent_vertices(u, g.graph());
  }

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::inv_adjacency_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::inv_adjacency_iterator>
  inv_adjacent_vertices(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
			const mlnu::boost_graph<VProp, EProp>& g)
  {
    return inv_adjacent_vertices(u, g.graph());
  }

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator>
  out_edges(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	    const mlnu::boost_graph<VProp, EProp>& g)
  {
    return out_edges(u, g.graph());
  }

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::in_edge_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::in_edge_iterator>
  in_edges(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	   const mlnu::boost_graph<VProp, EProp>& g)
  {
    return in_edges(v, g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  source(typename mlnu::boost_graph<VProp, EProp>::edge_descriptor e,
	 const mlnu::boost_graph<VProp, EProp>& g)
  {
    return source(e, g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  target(typename mlnu::boost_graph<VProp, EProp>::edge_descriptor e,
	 const mlnu::boost_graph<VProp, EProp>& g)
  {
    return target(e, g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::degree_size_type
  out_degree(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	     const mlnu::boost_graph<VProp, EProp>& g)
  {
    return out_degree(u, g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::degree_size_type
  in_degree(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	    const mlnu::boost_graph<VProp, EProp>& g)
  {
    return in_degree(u, g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertices_size_type
  num_vertices(const mlnu::boost_graph<VProp, EProp>& g)
  {
    return num_vertices(g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::edges_size_type
  num_edges(const mlnu::boost_graph<VProp, EProp>& g)
  {
    return num_edges(g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  vertex(typename mlnu::boost_graph<VProp, EProp>::vertices_size_type n,
	 const typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return vertex(n, g.graph());
  }

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::edge_descriptor,
	    bool>
  edge(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
       typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
       const mlnu::boost_graph<VProp, EProp>& g)
  {
    return edge(u, v, g.graph());
  }

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator,
	    typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator>
  edge_range(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	     typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	     const mlnu::boost_graph<VProp, EProp>& g)
  {
    return edge_range(u, v, g.graph());
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace boost

#endif // ! MLN_UTIL_INTERNAL_BOOST_GRAPH_ACCESS_HH_
