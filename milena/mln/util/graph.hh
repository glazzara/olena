// Copyright (C) 2007 EPITA Research and Development Laboratory
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

# include <mln/core/concept/object.hh>
# include <cstddef>
# include <ostream>
# include <vector>
# include <list>
# include <algorithm>

/*! \file mln/util/graph.hh
 *
 * \brief Definition of a graph.
 */


namespace mln
{
  namespace util
  {

    /*! \brief Structure of generic node.
     *
     */
    template<typename T>
    struct s_node
    {
      T data;
      std::vector<unsigned> links;
    };


    /*! \brief Structure of node with void parameter.
     *
     */
    template<>
    struct s_node<void>
    {
      std::list<unsigned> links;
    };


    /*! \brief Structure of generic edge.
     *
     */
    template<typename T>
    struct s_edge
    {
      T	data;
      unsigned node1;
      unsigned node2;
    };

    /*! \brief Structure of edge with void parameter.
     *
     */
    template<>
    struct s_edge <void>
    {
      unsigned node1;
      unsigned node2;
    };

    /*! \brief Generic graph using s_node and s_edge.
     *
     */
    template<typename N, typename E = void>
    struct graph
    {
      /*! \brief Constructor.
       *
       */
      graph ();

      /*! \brief Add a void node.
       *
       */
      void add_node (void);


      /*! \brief Add a void edge between \p n1 and \p n2.
       *
       * \param[in] n1 The first node to link.
       * \param[in] n2 The second node to link.
       *
       * \pre n1 < nb_node_.
       * \pre n2 < nb_node_.
       *
       */
      void add_edge (unsigned n1, unsigned n2);


      /*! \brief Check the consistency of the graph.
       *
       * Check if all edge have their node in the graph.
       *
       * \pre nodes_.size () == nb_node_.
       * \pre links_.size () == nb_link_.
       *
       */
      void consistency () const;


      /*! \brief Print on \p ostr the graph.
       *
       * \param[in] ostr The output stream.
       *
       */
      void print_debug (std::ostream& ostr) const;



      /// The nuber of nodes.
      unsigned nb_node_;

      /// The nuber of links.
      unsigned nb_link_;

      /// The vector where is stocked the pointers of nodes.
      std::vector<struct s_node<N>*> nodes_;

      /// The vector where is stocked the pointers of links.
      std::vector<struct s_edge<E>*> links_;
    };

# ifndef MLN_INCLUDE_ONLY

    template<typename N, typename E>
    inline
    graph<N, E>::graph ()
      : nb_node_ (0),
	nb_link_ (0),
	nodes_ (0),
	links_ (0)
    {
    }

    template<typename N, typename E>
    inline
    void
    graph<N, E>::add_node (void)
    {
      struct s_node<N>* n = new struct s_node<N>;

      nodes_.push_back (n);
      ++nb_node_;
    }

    template<typename N, typename E>
    inline
    void
    graph<N, E>::add_edge (unsigned n1, unsigned n2)
    {
      mln_precondition(n1 < this->nb_node_);
      mln_precondition(n2 < this->nb_node_);

      struct s_edge<E>* edge;

      edge = new struct s_edge<E>;
      edge->node1 = n1;
      edge->node2 = n2;
      links_.push_back (edge);
      ++nb_link_;
      nodes_[n1]->links.push_back (n2);
    }

    template<typename N, typename E>
    inline
    void
    graph<N, E>::consistency () const
    {
      mln_precondition(nodes_.size () == this->nb_node_);
      mln_precondition(links_.size () == this->nb_link_);
      typename std::vector<struct s_node <N>*>::const_iterator it = nodes_.begin ();
      for (; it != nodes_.end (); ++it)
	{
	  typename std::list<unsigned>::const_iterator it2 = (*it)->links.begin ();
	  for (; it2 != (*it)->links.end (); ++it2)
	    mln_precondition((*it2) < nb_node_);
	}

      typename std::vector<struct s_edge<E>*>::const_iterator it3 = links_.begin ();
      for (; it3 != links_.end (); ++it3)
	{
	  mln_precondition((*it3)->node1 < nb_node_);
	  mln_precondition((*it3)->node2 < nb_node_);
	}
    }

    template<typename N, typename E>
    inline
    void
    graph<N, E>::print_debug (std::ostream& ostr) const
    {
      ostr << "nodes :"	<< std::endl;

      typename std::vector<struct s_node<N>*>::const_iterator it = nodes_.begin ();
      int i = 0;
      for (; it != nodes_.end (); ++it, ++i)
	{
	  ostr << "node number = " << i << " nbh : ";
	  typename std::list<unsigned>::const_iterator it2 = (*it)->links.begin ();
	  for (; it2 != (*it)->links.end (); ++it2)
	    {
	      ostr << (*it2) << " ";
	    }
	  ostr << std::endl;
	}
      ostr << std::endl;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of util

} // end of mln

#endif // MLN_GRAPH_HH
