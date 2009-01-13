#ifndef MLN_FLLT_CANONIZE_TREE_HH
# define MLN_FLLT_CANONIZE_TREE_HH

#include "fllt.svg.7.hh"
#include "fllt_tree_to_image.hh"
#include "area_filter.hh"

namespace mln
{

  template <typename P, typename V>
  void
  merge_node(fllt_node(P, V)* src, fllt_node(P, V)* dest)
  {
    for (typename std::vector<fllt_node(P, V)* >::iterator it = src->parent()->children().begin();
	 it != src->parent()->children().end(); ++it)
      if ((*it) == src)
      {
	src->parent()->children().erase(it);
	break;
      }

    dest->elt().points.append(src->elt().points);
    while(!src->children().empty())
      dest->add_child(*src->children().begin());
    delete src;
  }

  template <typename P, typename V>
  bool
  canonize_tree(fllt_node(P, V)* node)
  {
    if (node->parent() && node->parent()->elt().value == node->elt().value)
    {
      merge_node(node, node->parent());
      return true;
    }

    bool change = true;
    while (change)
    {
      change = false;
      for (int i = 0; !change && i < node->children().size(); i++)
	change = canonize_tree(node->children()[i]);
    }
    return false;
  }

}

#endif // ! MLN_FLLT_CANONIZE_TREE_HH
