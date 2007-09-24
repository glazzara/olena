#ifndef MLN_GRAPH_HXX
# define MLN_GRAPH_HXX

# include "graph.hh"

namespace mln
{

  namespace util
  {

    template<typename N, typename E>
    inline
    void
    Graph<N, E>::add_node ()
    {
      nodes_.push_back (new s_node);
      ++nb_node_;
    }

    template<typename N, typename E>
    inline
    void
    Graph<N, E>::add_edge (unsigned n1, unsigned n2)
    {
      mln_precondition(n1 < this->nb_node_);
      mln_precondition(n2 < this->nb_node_);

      s_edge*		edge;

      edge = new s_edge;
      edge->node1 = n1;
      edge->node2 = n2;
      links_.push_back (edge);
      ++nb_link_;
      nodes_[n1]->links.push_back (n2);
    }

    template<typename N, typename E>
    inline
    void
    Graph<N, E>::coherce ()
    {
      mln_precondition(nodes_.size () == this->nb_node_);
      mln_precondition(links_.size () == this->nb_link_);
      typename std::vector<s_node*>::const_iterator it = nodes_.begin ();
      for (; it != nodes_.end (); ++it)
	{
	  typename std::vector<unsigned>::const_iterator it2 = (*it)->links.begin ();
	  for (; it2 != (*it)->links.end (); ++it2)
	    mln_precondition((*it2) < nb_node_);
	}

      typename std::vector<s_edge*>::const_iterator it3 = links_.begin ();
      for (; it3 != links_.end (); ++it3)
	{
	  mln_precondition((*it3)->node1 < nb_node_);
	  mln_precondition((*it3)->node2 < nb_node_);
	}
    }

    template<typename N, typename E>
    inline
    void
    Graph<N, E>::print ()
    {
      std::cout << "nodes :"
		<< std::endl;

      typename std::vector<s_node*>::const_iterator it = nodes_.begin ();
      int i = 0;
      for (; it != nodes_.end (); ++it, ++i)
	{
	  std::cout << "node number = "
		    << i
		    << " nbh : ";
	  typename std::vector<unsigned>::const_iterator it2 = (*it)->links.begin ();
	  for (; it2 != (*it)->links.end (); ++it2, ++i)
	    {
	      std::cout << (*it2)
			<< " ";
	    }
	  std::cout << std::endl;
	}
      std::cout << std::endl;
    }



  } // end of util

} // end of mln

#endif // MLN_GRAPH_HXX
