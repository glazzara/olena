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

#ifndef MLN_UTIL_INTERNAL_BOOST_GRAPH_HH
# define MLN_UTIL_INTERNAL_BOOST_GRAPH_HH

/// \file mln/util/internal/boost_graph.hh
///
/// Definition of the boost::adjacenly_list decorator.

# include <boost/graph/adjacency_list.hpp>

namespace mln
{

  namespace util
  {

    struct empty {};

    namespace internal
    {

      /// Boost graph decorator base
      /// Graph class which rests on boost::adjacency_list class.
      template <typename VertexProperty = empty,
		typename EdgeProperty = empty>
      class boost_graph
      {
	typedef boost_graph<VertexProperty, EdgeProperty> self_type;

      public:
	/// Types declartation.
	typedef boost::adjacency_list<boost::setS, boost::vecS,
				      boost::undirectedS,
				      VertexProperty, EdgeProperty> decorated;

	/// Descriptors.
	typedef typename decorated::vertex_descriptor vertex_descriptor;
	typedef typename decorated::edge_descriptor edge_descriptor;

	/// Iterators.
	typedef typename decorated::vertex_iterator vertex_iterator;
	typedef typename decorated::edge_iterator edge_iterator;
	typedef typename decorated::out_edge_iterator out_edge_iterator;
	typedef typename decorated::in_edge_iterator in_edge_iterator;
	typedef typename decorated::adjacency_iterator adjacency_iterator;
	typedef typename decorated::inv_adjacency_iterator
	inv_adjacency_iterator;

	/// Categories.
	typedef typename decorated::directed_category directed_category;
	typedef typename decorated::traversal_category traversal_category;
	typedef typename decorated::edge_parallel_category
	edge_parallel_category;
	typedef typename decorated::vertex_property_type vertex_property_type;
	typedef typename decorated::edge_property_type edge_property_type;
	typedef typename decorated::graph_tag graph_tag;

	/// Properties.
	typedef typename decorated::vertex_bundled vertex_bundled;

	/// Sizes.
	typedef typename decorated::vertices_size_type vertices_size_type;
	typedef typename decorated::edges_size_type edges_size_type;
	typedef typename decorated::degree_size_type degree_size_type;


	/// Constructor(s).
	boost_graph();
	boost_graph(const boost_graph& lhs);
	boost_graph(vertices_size_type n);

	/// Assignment operator.
	boost_graph&
	operator=(const boost_graph& lhs);

	/// Remove all of the edges and vertices from the graph.
	void clear();

	/// Swap the vertices, edges, and properties of this graph with
	/// the vertices, edges, and properties of graph x.
	void swap(boost_graph& rhs);

	/// Internal methods:

	/// Return the boost decorated graph.
	decorated&
	graph();

	/// Return the boost decorated graph (const version).
	const decorated&
	graph() const;

	/// Provides acces to the graph bundle properties
	vertex_bundled&	operator[](vertex_descriptor v);
	const vertex_bundled& operator[](vertex_descriptor v) const;


      protected:
	decorated graph_;
	/// add index on need
      };

      /// Graph method declaration

      /// Implementation

# ifndef MLN_INCLUDE_ONLY

      template <typename VertexProp, typename EdgeProp>
      boost_graph<VertexProp, EdgeProp>::boost_graph()
      {
      }

      template <typename VertexProp, typename EdgeProp>
      boost_graph<VertexProp, EdgeProp>::boost_graph(const boost_graph& lhs) :
	graph_(lhs.graph_)
      {
      }

      template <typename VertexProp, typename EdgeProp>
      boost_graph<VertexProp, EdgeProp>::boost_graph(vertices_size_type n) :
	graph_(n)
      {
      }

      template <typename VertexProp, typename EdgeProp>
      boost_graph<VertexProp, EdgeProp>&
      boost_graph<VertexProp, EdgeProp>::operator=(const boost_graph& lhs)
      {
	if (&lhs == this)
	  return *this;
	this->graph_ = lhs.graph_;
      }

      template <typename VertexProp, typename EdgeProp>
      void
      boost_graph<VertexProp, EdgeProp>::clear()
      {
	this->graph_.clear();
      }

      template <typename VertexProp, typename EdgeProp>
      void
      boost_graph<VertexProp, EdgeProp>::swap(boost_graph& rhs)
      {
	this->graph_.swap(rhs.graph_);
      }

      template <typename VertexProp, typename EdgeProp>
      inline
      typename boost_graph<VertexProp, EdgeProp>::decorated&
      boost_graph<VertexProp, EdgeProp>::graph()
      {
	return this->graph_;
      }

      template <typename VertexProp, typename EdgeProp>
      inline
      const typename boost_graph<VertexProp, EdgeProp>::decorated&
      boost_graph<VertexProp, EdgeProp>::graph() const
      {
	return this->graph_;
      }

      template <typename V, typename E>
      inline
      typename boost_graph<V, E>::vertex_bundled&
      boost_graph<V, E>::operator[](vertex_descriptor v)
      {
	return this->graph_[v];
      }

      template <typename V, typename E>
      inline
      const typename boost_graph<V, E>::vertex_bundled&
      boost_graph<V, E>::operator[](vertex_descriptor v) const
      {
	return this->graph_[v];
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::util::internal

  } // end of namespace mln::util

} // end of namespace mln

# include "boost_graph_access.hh"
# include "boost_graph_structure.hh"
# include "boost_graph_property.hh"

#endif // ! MLN_UTIL_INTERNAL_BOOST_GRAPH_HH
