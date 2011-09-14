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

/// \file mln/util/internal/boost_graph_structure.hh
///
/// Operations that interract with the boost_graph structure.

#ifndef MLN_UTIL_INTERNAL_BOOST_GRAPH_STRUCTURE_HH_
# define MLN_UTIL_INTERNAL_BOOST_GRAPH_STRUCTURE_HH_

# include <mln/util/internal/boost_graph.hh>


/// forward declaration.
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

  /// Adds edge (u,v) to the graph and returns the edge descriptor for
  ///        the new edge.
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::edge_descriptor, bool>
  add_edge(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	   typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	   typename mlnu::boost_graph<VProp, EProp>& g);


  /// Adds edge (u,v) to the graph and attaches p as the value of the
  ///        edge's internal property storage.
  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::edge_descriptor, bool>
  add_edge(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	   typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	   const typename mlnu::boost_graph<VProp, EProp>::
	   edge_property_type& p,
	   typename mlnu::boost_graph<VProp, EProp>& g);

  /// Removes the edge (u,v) from the graph.
  template <typename VProp, typename EProp>
  void
  remove_edge(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	      typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	      typename mlnu::boost_graph<VProp, EProp>& g);

  /// Removes the edge e from the graph.
  /// This differs from the remove_edge(u, v, g) function in the case of
  /// a multigraph. This remove_edge(e, g) function removes a single edge,
  /// whereas the remove_edge(u, v, g) function removes all edges (u,v).
  template <typename VProp, typename EProp>
  void
  remove_edge(typename mlnu::boost_graph<VProp, EProp>::edge_descriptor e,
	      typename mlnu::boost_graph<VProp, EProp>& g);


  /// Same as remove_edge(*iter, g)
  template <typename VProp, typename EProp>
  void
  remove_edge(typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator iter,
	      typename mlnu::boost_graph<VProp, EProp>& g);


  /// Removes all out-edges of vertex u from the graph that
  ///        satisfy the predicate.
  template <typename VProp, typename EProp, class Predicate>
  void
  remove_out_edge_if(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		     Predicate predicate,
		     typename mlnu::boost_graph<VProp, EProp>& g);

  /// FIXME only available on directed graph
  template <typename VProp, typename EProp, class Predicate>
  void
  remove_in_edge_if(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
		    Predicate predicate,
		    typename mlnu::boost_graph<VProp, EProp>& g);

  /// Removes all edges of vertex u from the graph that
  ///        satisfy the predicate.
  template <typename VProp, typename EProp, class Predicate>
  void remove_edge_if(Predicate predicate,
		      typename mlnu::boost_graph<VProp, EProp>& g);

  /// Adds a vertex to the graph.
  ///        Returns the vertex descriptor for the new vertex.
  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  add_vertex(typename mlnu::boost_graph<VProp, EProp>& g);

  /// Adds a vertex to the graph with the specified properties.
  ///        Returns the vertex descriptor for the new vertex.
  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  add_vertex(const typename mlnu::boost_graph<VProp, EProp>::
	     vertex_property_type& p,
	     typename mlnu::boost_graph<VProp, EProp>& g);

  /// Removes all edges to and from vertex u.
  template <typename VProp, typename EProp>
  void
  clear_vertex(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	       typename mlnu::boost_graph<VProp, EProp>& g);

  /// Removes all out-edges to and from vertex u.
  template <typename VProp, typename EProp>
  void
  clear_out_edges(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		  typename mlnu::boost_graph<VProp, EProp>& g);

  /// FIXME: only define for the directed graphs.
  template <typename VProp, typename EProp>
  void
  clear_in_edges(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		  typename mlnu::boost_graph<VProp, EProp>& g);


  /// Remove vertex u from the vertex set of the graph.
  /// It is assumed that there are no edges to or from vertex u
  /// when it is removed
  template <typename VProp, typename EProp>
  void
  remove_vertex(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		typename mlnu::boost_graph<VProp, EProp>& g);

# ifndef MLN_INCLUDE_ONLY

  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::edge_descriptor,
	    bool>
  add_edge(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	   typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	   typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return add_edge(u, v, g.graph());
  }


  template <typename VProp, typename EProp>
  std::pair<typename mlnu::boost_graph<VProp, EProp>::edge_descriptor,
	    bool>
  add_edge(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	   typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	   const typename mlnu::boost_graph<VProp, EProp>::
	   edge_property_type& p,
	   typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return add_edge(u, v, p, g.graph());
  }

  template <typename VProp, typename EProp>
  void
  remove_edge(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	      typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
	      typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return remove_edge(u, v, g.graph());
  }

  template <typename VProp, typename EProp>
  void
  remove_edge(typename mlnu::boost_graph<VProp, EProp>::edge_descriptor e,
	      typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return remove_edge(e, g.graph());
  }

  template <typename VProp, typename EProp>
  void
  remove_edge(typename mlnu::boost_graph<VProp, EProp>::out_edge_iterator iter,
	      typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return remove_edge(iter, g.graph());
  }

  template <typename VProp, typename EProp, class Predicate>
  void
  remove_out_edge_if(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		     Predicate predicate,
		     typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return remove_out_edge_if(u, predicate, g.graph());
  }

  template <typename VProp, typename EProp, class Predicate>
  void
  remove_in_edge_if(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor v,
		    Predicate predicate,
		    typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return remove_in_edge_if(v, predicate, g.graph());
  }

  template <typename VProp, typename EProp, class Predicate>
  void remove_edge_if(Predicate predicate,
		      typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return remove_edge_if(predicate, g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  add_vertex(typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return add_vertex(g.graph());
  }

  template <typename VProp, typename EProp>
  typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor
  add_vertex(const typename mlnu::boost_graph<VProp, EProp>::
	     vertex_property_type& p,
	     typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return add_vertex(p, g.graph());
  }

  template <typename VProp, typename EProp>
  void
  clear_vertex(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
	       typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return clear_vertex(u, g.graph());
  }

  template <typename VProp, typename EProp>
  void
  clear_out_edges(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		  typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return clear_out_edges(u, g.graph());
  }

  template <typename VProp, typename EProp>
  void
  clear_in_edges(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		  typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return clear_in_edges(u, g.graph());
  }


  template <typename VProp, typename EProp>
  void
  remove_vertex(typename mlnu::boost_graph<VProp, EProp>::vertex_descriptor u,
		typename mlnu::boost_graph<VProp, EProp>& g)
  {
    return remove_vertex(u, g);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace boost

#endif // ! MLN_UTIL_INTERNAL_BOOST_GRAPH_STRUCTURE_HH_
