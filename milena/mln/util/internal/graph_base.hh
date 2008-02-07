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

# include <mln/core/concept/object.hh>
# include <cstddef>
# include <ostream>
# include <vector>
# include <list>
# include <algorithm>
# include <mln/util/ordpair.hh>

namespace mln
{

  namespace util
  {
    /// \bref The type used to identify nodes.
    ///
    /// Used internally as a key to manipulate nodes.
    typedef unsigned node_id;

    /// \bref The type used to identify edges.
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
    template<>
    struct node<void>
    {
      std::list<node_id> edges;
    };


    /*-------.
    | Edge.  |
    `-------*/

    /// \brief Edge of a graph, holding a value of type \p T.
    template<typename T>
    struct edge
    {
      edge(node_id n1, node_id n2)
	: pair_node_ (n1, n2)
      {}

      /// Return the lowest node id adjacent to this edge.
      node_id n1 () const { return pair_node_.first; }
      /// Return the highest node id adjacent to this edge.
      node_id n2 () const { return pair_node_.second; }

      T	data;
      ordpair_<node_id> pair_node_;
    };

    /// \brief Specialization of mln::util::node for edges with no
    /// associated value.
    template<>
    struct edge <void>
    {
      edge(node_id n1, node_id n2)
	: pair_node_ (n1, n2)
      {}

      /// Return the lowest node id adjacent to this edge.
      node_id n1 () const { return pair_node_.first; }
      /// Return the highest node id adjacent to this edge.
      node_id n2 () const { return pair_node_.second; }

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
      public:
	/* FIXME: Do we really want handle nodes and edges through
	   pointers?  In my (Roland) opinion, this is just a drawback,
	   here.  */
	/// The type of the set of nodes.
	typedef std::vector< node<N>* > nodes_t;
	/// The type of the set of edges.
	typedef std::vector< edge<E>* > edges_t;

	/// Constructor.
	graph_base();
	/// Destructor.
	~graph_base();

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
	nodes_t& nodes ();
	const nodes_t& nodes () const;
	/// \}

	/// Return the whole edges of the graph.
	/// \{
	edges_t& edges ();
	const edges_t& edges () const;
	/// \}
      
	/// \brief Return the number of nodes in the graph.
	size_t nnodes() const;
	/// \brief Return the number of edges in the graph.
	size_t nedges() const;

	// FIXME: We might want to externalize this in routine of
	// namespace mln::debug.
	/** \brief Print on \p ostr the graph.

	    \param[in] ostr  The output stream.  */
	void print_debug (std::ostream& ostr) const;

      protected:
	/// Shortcuts factoring the insertion of nodes and edges.
	/// \{
	void add_node_ (util::node<N>* node);
	void add_edge_ (util::edge<E>* edge);
	/// \}

      protected:
	/// The nodes.
	nodes_t nodes_;
	/// The edges.
	edges_t edges_;
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

      /*----------------.
      | Ctor and dtor.  |
      `----------------*/

      template<typename N, typename E>
      inline
      graph_base<N, E>::graph_base()
	: nodes_(), edges_()
      {
      }

      template<typename N, typename E>
      inline
      graph_base<N, E>::~graph_base()
      {
	// FIXME: Delete data dynamically allocated in nodes_ and
	// edges_.
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

      template<typename N, typename E>
      inline
      void
      graph_base<N, E>::add_edge_(util::edge<E>* edge)
      {
	// Does this edge already exist in the graph?
	if (std::find(edges_.begin(), edges_.end(), edge) != edges_.end ())
	  {
	    delete edge;
	    edge = 0;
	  }
	else
	  {
	    edges_.push_back (edge);
	    nodes_[edge->n1()]->edges.push_back(edge->n2());
	    nodes_[edge->n2()]->edges.push_back(edge->n1());
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
	    for (typename edges_t::const_iterator e = (*n)->edges.begin();
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
