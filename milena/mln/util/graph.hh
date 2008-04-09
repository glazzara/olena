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


// FIXME: More doc!

// FIXME: Rename node(s) as vertex (vertices).


namespace mln
{

  namespace util
  {
    /*-----------.
    | Fwd decl.  |
    `-----------*/

    /// \brief Undirected graph.
    template<typename N = void, typename E = void>
    class graph;

    /*--------------------.
    | graph<void, void>.  |
    `--------------------*/

    /// Specialization for undirected graphs with no data on nodes nor
    /// on edges.
    template <>
    class graph<void, void> : public internal::graph_base<void, void>
    {
    public:
      /// The super class.
      typedef internal::graph_base<void, void> super;

      // FIXME: We should return the id of the newly created node.
      /// \brief Add a node.
      void add_node();
      // FIXME: We should return the id of the newly created edge.
      /// \brief Add an edge between nodes with ids \p n1 and \p n2.
      void add_edge(node_id n1, node_id n2);
    };

    /*-----------------.
    | graph<N, void>.  |
    `-----------------*/

    /// Specialization for undirected graphs with data on nodes.
    template <typename N>
    class graph<N, void> : public internal::graph_base<N, void>
    {
    public:
      /// The super class.
      typedef internal::graph_base<N, void> super;

      // FIXME: We should return the id of the newly created node.
      /// \brief Add a node.
      void add_node(const N& data);
      // FIXME: We should return the id of the newly created edge.
      /// \brief Add an edge between nodes with ids \p n1 and \p n2.
      void add_edge(node_id n1, node_id n2);

      /// Return the data associated to node with id \a n.
      /// \{
      N& node_data(node_id n);
      const N& node_data(node_id n) const;
      /// \}
    };

    /*--------------.
    | graph<N, E>.  |
    `--------------*/

    /// Specialization for undirected graphs with data on nodes and
    /// edges.
    template <typename N, typename E>
    class graph : public internal::graph_base<N, E>
    {
    public:
      /// The super class.
      typedef internal::graph_base<N, E> super;

      // FIXME: We should return the id of the newly created node.
      /// \brief Add a node.
      void add_node(const N& data);
      /// \brief Add an edge between nodes with ids \p n1 and \p n2.
      // FIXME: We should return the id of the newly created edge.
      void add_edge(node_id n1, node_id n2, const E& data);

      /// Return the data associated to node with id \a n.
      /// \{
      N& node_data(node_id n);
      const N& node_data(node_id n) const;
      /// \}

      /// Return the data associated to the edge with id \a e.
      /// \{
      E& edge_data(edge_id e);
      const E& edge_data(edge_id e) const;
      /// \}

      /// Return the data associated to the edge between nodes with
      /// ids \a n1 and \a n2.
      /// \{
      E& edge_data(node_id n1, node_id n2);
      const E& edge_data(node_id n1, node_id n2) const;
      /// \}
    };



# ifndef MLN_INCLUDE_ONLY

    /*--------------------.
    | graph<void, void>.  |
    `--------------------*/

    /* Note that ddefinition of members from fully specialized
       template classes are not preceded by `template<>'.  */
    inline
    void
    graph<void, void>::add_node()
    {
      super::add_node_(new util::node<void>);
    }

    /* Note that ddefinition of members from fully specialized
       template classes are not preceded by `template<>'.  */
    inline
    void
    graph<void, void>::add_edge(node_id n1, node_id n2)
    {
      mln_assertion(n1 < this->nnodes());
      mln_assertion(n2 < this->nnodes());
      super::add_edge_(new util::edge<void>(n1, n2));
    }

    /*-----------------.
    | graph<N, void>.  |
    `-----------------*/

    template<typename N>
    inline
    void
    graph<N, void>::add_node(const N& data)
    {
      super::add_node_(new util::node<N>(data));
    }

    template<typename N>
    inline
    void
    graph<N, void>::add_edge(node_id n1, node_id n2)
    {
      mln_assertion(n1 < this->nnodes());
      mln_assertion(n2 < this->nnodes());
      super::add_edge_(new util::edge<void>(n1, n2));
    }

    template <class N>
    inline
    N&
    graph<N, void>::node_data(node_id n)
    {
      mln_assertion(n < this->nnodes());
      return this->nodes_[n]->data;
    }

    template<typename N>
    inline
    const N&
    graph<N, void>::node_data(node_id n) const
    {
      mln_assertion(n < this->nnodes());
      return this->nodes_[n]->data;
    }


    /*--------------.
    | graph<N, E>.  |
    `--------------*/

    template<typename N, typename E>
    inline
    void
    graph<N, E>::add_node(const N& data)
    {
      super::add_node_(new util::node<N>(data));
    }

    template<typename N, typename E>
    inline
    void
    graph<N, E>::add_edge(node_id n1, node_id n2, const E& data)
    {
      mln_assertion(n1 < this->nnodes());
      mln_assertion(n2 < this->nnodes());
      super::add_edge_(new util::edge<E>(n1, n2, data));
    }

    template<typename N, typename E>
    inline
    N&
    graph<N, E>::node_data(node_id n)
    {
      mln_assertion(n < this->nnodes());
      return this->nodes_[n]->data;
    }

    template<typename N, typename E>
    inline
    const N&
    graph<N, E>::node_data(node_id n) const
    {
      mln_assertion(n < this->nnodes());
      return this->nodes_[n]->data;
    }

    template<typename N, typename E>
    inline
    E&
    graph<N, E>::edge_data(edge_id e)
    {
      mln_assertion(e < this->nedges());
      return this->edges_[e]->data;
    }

    template<typename N, typename E>
    inline
    const E&
    graph<N, E>::edge_data(edge_id e) const
    {
      mln_assertion(e < this->nedges());
      return this->edges_[e]->data;
    }

    template<typename N, typename E>
    inline
    E&
    graph<N, E>::edge_data(node_id n1, node_id n2)
    {
      mln_assertion(n1 < this->nnodes());
      mln_assertion(n2 < this->nnodes());
      ordpair_<node_id> node_pair (n1, n2);
      std::vector<edge_id>& edges_ids = this->nodes_[n1]->edges;
      for (std::vector<edge_id>::iterator e = edges_ids.begin();
	   e != edges_ids.end(); ++e)
	  if (this->edges_[*e] == node_pair)
	    return this->edges_[*e]->data;
      // If no edge between N1 and N2 was found, abort.
      abort();
    }

    template<typename N, typename E>
    inline
    const E&
    graph<N, E>::edge_data(node_id n1, node_id n2) const
    {
      mln_assertion(n1 < this->nnodes());
      mln_assertion(n2 < this->nnodes());
      ordpair_<node_id> node_pair (n1, n2);
      const std::vector<edge_id>& edges_ids = this->nodes_[n1]->edges;
      for (std::vector<edge_id>::const_iterator e = edges_ids.begin();
	   e != edges_ids.end(); ++e)
	  if (this->edges_[*e] == node_pair)
	    return this->edges_[*e]->data;
      // If no edge between N1 and N2 was found, abort.
      abort();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_GRAPH_HH
