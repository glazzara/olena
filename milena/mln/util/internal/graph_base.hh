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

#ifndef MLN_UTIL_INTERNAL_GRAPH_BASE_HH
# define MLN_UTIL_INTERNAL_GRAPH_BASE_HH

/// \file mln/util/internal/graph.hh
/// \brief Factored implementation of undirected graphs.

# include <cstddef>

# include <algorithm>

# include <vector>
# include <set>
# include <ostream>

# include <mln/core/concept/object.hh>
# include <mln/util/ordpair.hh>

// FIXME: Rename node(s) as vertex (vertices).

namespace mln
{

  namespace util
  {
    /* FIXME: We should create actual types for node_id and edge_id,
       (not just typedefs), at least to prevent the user from using a
       node_id where an edge_id is expected (and vice versa).
       Conversion to and from unsigned would still be useful, but it
       might be safer to turn them into explicit ones.  */

    /// \brief The type used to identify nodes.
    ///
    /// Used internally as a key to manipulate nodes.
    typedef unsigned node_id;

    /// \brief The type used to identify edges.
    ///
    /// Used internally as a key to manipulate edges.
    typedef unsigned edge_id;


    /*-------.
    | Node.  |
    `-------*/

    /// \brief Node of a graph, holding a value of type \p T.
    template<typename T>
    struct node
    {
      node(T data)
	: data(data)
      {}

      T data;
      std::vector<edge_id> edges;
    };

    /// \brief Specialization of mln::util::node for nodes with no
    /// associated value.
    template <>
    struct node<void>
    {
      std::vector<edge_id> edges;
    };


    /*-------.
    | Edge.  |
    `-------*/

    /// \brief Edge of a graph, holding a value of type \p T.
    template<typename T>
    struct edge
    {
      edge(node_id n1, node_id n2)
	: pair_node_(n1, n2)
      {}

      /// Return the lowest node id adjacent to this edge.
      node_id n1() const { return pair_node_.first; }
      /// Return the highest node id adjacent to this edge.
      node_id n2() const { return pair_node_.second; }

      T	data;
      ordpair_<node_id> pair_node_;
    };

    /// \brief Specialization of mln::util::node for edges with no
    /// associated value.
    template <>
    struct edge<void>
    {
      edge(node_id n1, node_id n2)
	: pair_node_(n1, n2)
      {}

      /// Return the lowest node id adjacent to this edge.
      node_id n1() const { return pair_node_.first; }
      /// Return the highest node id adjacent to this edge.
      node_id n2() const { return pair_node_.second; }

      ordpair_<node_id> pair_node_;
    };

    // FIXME: Document this.  In particular, we should state that edges are
    // only compared w.r.t. their adjacent nodes, not the data they
    // possibly hold!
    template <typename E>
    bool
    operator==(const edge<E>& lhs, const edge<E>& rhs);

    template <typename E>
    bool
    operator< (const edge<E>& lhs, const edge<E>& rhs);


    /*--------.
    | Graph.  |
    `--------*/

    namespace internal
    {
      /// \brief Base class for undirected graphs.
      template<typename N, typename E>
      class graph_base
      {
	typedef graph_base<N, E> self_t;

      public:
	/* FIXME: Do we really want handle nodes and edges through
	   pointers?  In my (Roland) opinion, this is just a drawback,
	   here.  */
	/// The type of the set of nodes.
	typedef std::vector< util::node<N>* > nodes_t;
	/// The type of the set of edges.
	typedef std::vector< util::edge<E>* > edges_t;
	/// A set to test the presence of a given edge.
	typedef std::set< util::edge<E>* > edges_set_t;

	/// Construction, assignments and destruction.
	/// \{
	graph_base();
	graph_base(const self_t& rhs);
	self_t& operator=(const self_t& rhs);
	~graph_base();
	/// \}

	/// Return the node whose id is \a n.
	/// \{
	util::node<N>& node(node_id n);
	const util::node<N>& node(edge_id n) const;
	/// \}

	/// Return the edge whose id is \a e.
	/// \{
	util::edge<E>& edge(edge_id e);
	const util::edge<E>& edge(edge_id e) const;
	/// \}

	/// Return the whole nodes of the graph.
	/// \{
	nodes_t& nodes();
	const nodes_t& nodes() const;
	/// \}

	/// Return the whole edges of the graph.
	/// \{
	edges_t& edges();
	const edges_t& edges() const;
	/// \}

	/// \brief Return the number of nodes in the graph.
	size_t nnodes() const;
	/// \brief Return the number of edges in the graph.
	size_t nedges() const;

	// FIXME: We might want to externalize this in routine of
	// namespace mln::debug.
	/** \brief Print on \p ostr the graph.

	    \param[in] ostr  The output stream.  */
	void print_debug(std::ostream& ostr) const;

      protected:
	/// Shortcuts factoring the insertion of nodes and edges.
	/// \{
	void add_node_(util::node<N>* node);
	void add_edge_(util::edge<E>* edge);
	/// \}

      protected:
	/// The nodes.
	nodes_t nodes_;
	/// The edges.
	edges_t edges_;
	/// An index of the set of edges, for fast-access purpose.
	edges_set_t edges_set_;
      };

    } // end of namespace mln::util::internal



# ifndef MLN_INCLUDE_ONLY

    template <typename E>
    bool
    operator==(const edge<E>& lhs, const edge<E>& rhs)
    {
      return lhs.pair_node_ == rhs.pair_node_;
    }

    template <typename E>
    bool
    operator< (const edge<E>& lhs, const edge<E>& rhs)
    {
      return lhs.pair_node_ < rhs.pair_node_;
    }

    namespace internal
    {

      /*--------------------------------------------.
      | Construction, assignments and destruction.  |
      `--------------------------------------------*/

      template<typename N, typename E>
      inline
      graph_base<N, E>::graph_base()
	: nodes_(), edges_(), edges_set_()
      {
      }

      template<typename N, typename E>
      inline
      graph_base<N, E>::graph_base(const graph_base<N, E>& rhs)
	: nodes_(), edges_(), edges_set_()
      {
	nodes_.reserve(rhs.nodes_.size());
	edges_.reserve(rhs.edges_.size());
	for (typename nodes_t::const_iterator n = rhs.nodes_.begin();
	     n != rhs.nodes_.end(); ++n)
	  nodes_.push_back(new util::node<N>(**n));
	for (typename edges_t::const_iterator e = rhs.edges_.begin();
	     e != rhs.edges_.end(); ++e)
	  edges_.push_back(new util::edge<E>(**e));
	std::copy(edges_.begin(), edges_.end(),
		  std::insert_iterator<edges_set_t>(edges_set_,
						    edges_set_.begin()));
      }

      template<typename N, typename E>
      inline
      graph_base<N, E>&
      graph_base<N, E>::operator=(const graph_base<N, E>& rhs)
      {
	if (this != &rhs)
	  {
	    /// Free previous nodes and edges.
	    for (typename nodes_t::iterator n = nodes_.begin();
		 n != nodes_.end(); ++n)
	      delete *n;
	    for (typename edges_t::iterator e = edges_.begin(); 
		 e != edges_.end(); ++e)
	      delete *e;
	    edges_set_.clear();
	    /// Assign values from RHS.
	    nodes_.reserve(rhs.nodes_.size());
	    edges_.reserve(rhs.edges_.size());
	    for (typename nodes_t::const_iterator n = rhs.nodes_.begin();
		 n != rhs.nodes_.end(); ++n)
	      nodes_.push_back(new util::node<N>(**n));
	    for (typename edges_t::const_iterator e = rhs.edges_.begin();
		 e != rhs.edges_.end(); ++e)
	      edges_.push_back(new util::edge<E>(**e));
	    std::copy(edges_.begin(), edges_.end(),
		      std::insert_iterator<edges_set_t>(edges_set_,
							edges_set_.begin()));
	  }
	return *this;
      }

      template<typename N, typename E>
      inline
      graph_base<N, E>::~graph_base()
      {
	for (typename nodes_t::iterator n = nodes_.begin(); n != nodes_.end();
	     ++n)
	  delete *n;
	for (typename edges_t::iterator e = edges_.begin(); e != edges_.end();
	     ++e)
	  delete *e;
	edges_set_.clear();
      }

      /*------------.
      | Accessors.  |
      `------------*/

      template<typename N, typename E>
      inline
      util::node<N>&
      graph_base<N, E>::node(node_id n)
      {
	mln_assertion(n < this->nnodes());
	return *nodes_[n];
      }

      template<typename N, typename E>
      inline
      const util::node<N>&
      graph_base<N, E>::node(node_id n) const
      {
	mln_assertion(n < this->nnodes());
	return *nodes_[n];
      }

      template<typename N, typename E>
      inline
      util::edge<E>&
      graph_base<N, E>::edge(edge_id e)
      {
	mln_assertion(e < this->nedges());
	return *edges_[e];
      }

      template<typename N, typename E>
      inline
      const util::edge<E>&
      graph_base<N, E>::edge(edge_id e) const
      {
	mln_assertion(e < this->nedges());
	return *edges_[e];
      }

      template<typename N, typename E>
      inline
      typename graph_base<N, E>::nodes_t&
      graph_base<N, E>::nodes()
      {
	return nodes_;
      }

      template<typename N, typename E>
      inline
      const typename graph_base<N, E>::nodes_t&
      graph_base<N, E>::nodes() const
      {
	return nodes_;
      }

      template<typename N, typename E>
      inline
      typename graph_base<N, E>::edges_t&
      graph_base<N, E>::edges()
      {
	return edges_;
      }

      template<typename N, typename E>
      inline
      const typename graph_base<N, E>::edges_t&
      graph_base<N, E>::edges() const
      {
	return edges_;
      }

      template<typename N, typename E>
      inline
      size_t
      graph_base<N, E>::nnodes() const
      {
	return nodes_.size();
      }

      template<typename N, typename E>
      inline
      size_t
      graph_base<N, E>::nedges() const
      {
	return edges_.size();
      }

      /*---------------.
      | Manipulators.  |
      `---------------*/

      template<typename N, typename E>
      inline
      void
      graph_base<N, E>::add_node_(util::node<N>* node)
      {
	nodes_.push_back (node);
      }

      // FIXME: Return the (new) edge id.
      template<typename N, typename E>
      inline
      void
      graph_base<N, E>::add_edge_(util::edge<E>* edge)
      {
	// Does this edge already exist in the graph?
	if (edges_set_.find(edge) != edges_set_.end ())
	  {
	    // Remove the previously allocated data for EDGE.
	    delete edge;
	    edge = 0;
	  }
	else
	  {
	    // Otherwise insert it into the graph.
	    edges_.push_back(edge);
	    edge_id id = edges_.size() - 1;
	    edges_set_.insert(edge);
	    nodes_[edge->n1()]->edges.push_back(id);
	    nodes_[edge->n2()]->edges.push_back(id);
	  }
      }

      /*--------.
      | Debug.  |
      `--------*/

      template<typename N, typename E>
      inline
      void
      graph_base<N, E>::print_debug (std::ostream& ostr) const
      {
	ostr << "graph: "	<< std::endl;
	int i = 0;
	for (typename nodes_t::const_iterator n = nodes_.begin ();
	     n != nodes_.end (); ++n, ++i)
	  {
	    ostr << "node: " << i << std::endl << " -- adjacent nodes: ";
	    /* FIXME: We shouldn't manipulate std::vector<edge_id>
	       directly, but use a typedef instead.  */
	    for (typename std::vector<util::edge_id>::const_iterator e =
		   (*n)->edges.begin();
		 e != (*n)->edges.end(); ++e)
	      ostr << *e << " ";
	    ostr << std::endl;
	  }
	ostr << std::endl;
      }

    } // end of namespace mln::util::internal

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_INTERNAL_GRAPH_BASE_HH
