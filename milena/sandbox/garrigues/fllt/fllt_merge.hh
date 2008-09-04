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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


#ifndef MLN_FIXME_FLLT_MERGE_HH
# define MLN_FIXME_FLLT_MERGE_HH

/*! \file fllt_merge.hh
 *
 * \brief merge the upper and lower level set.
 *
 */

# include <mln/core/image/image2d.hh>

# include <mln/set/is_subset_of.hh>

# include "fllt_types.hh"

namespace mln
{
  namespace fllt
  {
    // Fwd declarations.
    template <typename P, typename V, typename F>
    void
    fill_a_shape(fllt_node(P, V)& node,
		 fllt_tree(P, V)& tree,
		 const image2d<fllt_node(P, V)*>& node_reg,
		 const image2d<fllt_node(P, V)*>& hole_reg);

    template <typename P, typename V, typename F>
    void
    move_shape(fllt_node(P, V)& node,
	       fllt_node(P, V)& hole,
	       fllt_tree(P, V)& tree,
	       const image2d<fllt_node(P, V)*>& hole_reg,
	       const image2d<fllt_node(P, V)*>& other_reg)
    {
      node.add_child(&hole);
      fill_a_shape<P,V,typename F::opposite>(hole, tree, hole_reg, other_reg);
    }

    template <typename P, typename V, typename F>
    fllt_node(P, V)*
    find_the_hole(fllt_node(P, V)& node,
		  const P p,
		  const image2d<fllt_node(P, V)*>& other_reg)
    {
      fllt_node(P, V)* s = other_reg(p);
      mln_assertion(s);
      while (s->parent() && F::opposite::compare(s->parent()->elt().value, node.elt().value))
      {
	mln_assertion(s);
	s = s->parent();
	mln_assertion(s);
      }
      return s;
    }

    template <typename P, typename V, typename F>
    void
    fill_a_shape(fllt_node(P, V)& node,
		 fllt_tree(P, V)& tree,
		 const image2d<fllt_node(P, V)*>& node_reg,
		 const image2d<fllt_node(P, V)*>& hole_reg)
    {
      if (node.elt().holes.npoints() == 0)
      {
	return;
      }
      mln_piter(p_set<P>) p(node.elt().holes);
      for_all(p)
	{
	  bool h = true;

	  fllt_node(P, V)* hole;
	  if (node.elt().brighter == F::parent_is_brighter)
	    hole = find_the_hole<P,V,F>(node, point2d(p), hole_reg);
	  else
	    hole = find_the_hole<P,V,typename F::opposite>(node, point2d(p), node_reg);

	  mln_assertion(hole);

	  typename fllt_node(P, V)::children_t::iterator it;
	  for (it = node.children().begin();
	       it != node.children().end();
	       it++)
	  {
	    // Browse the holes of each child.
	    mln_piter(p_set<P>) q((*it)->elt().holes);
	    for_all(q)
	      {
		fllt_node(P, V)* child_hole = find_the_hole<P,V,F>((**it), point2d(q), hole_reg);
		if (set::is_subset_of(hole->elt().points,
				      child_hole->elt().points))
		{
		  h = false;
		  break;
		}

	      }
	    if (!h)
	      break;
	  }
	  if (h)
	    move_shape<P,V,F>(node, *hole, tree, hole_reg, node_reg);
	}

      node.elt().holes.clear();
    }

    template <typename P, typename V>
    fllt_tree(P, V)
      merge_trees(fllt_tree(P, V)& lower_tree,
		  fllt_tree(P, V)& upper_tree,
		  const image2d<fllt_node(P, V)*>& low_reg,
		  const image2d<fllt_node(P, V)*>& upp_reg,
		  const image2d<V>& ima)
    {

      //   In order to merge the trees, we only have to find for each shape S
      //   with a hole H in it whether one of its children has a hole H´
      //   containing H. If it is the case, we do nothing. Otherwise, we
      //   put the shape of the hole H (and all its descendants) as child of
      //   the shape .
      {
	fllt_branch_iter(P, V) p(lower_tree.main_branch());
	for_all(p)
	  {
	    fllt_node(P, V)& n(p);
	    fill_a_shape< P, V, lower<V> >(n, lower_tree, low_reg, upp_reg);
	    mln_assertion(lower_tree.check_consistency());
	    mln_assertion(upper_tree.check_consistency());
	  }

      }

      {
	fllt_branch_iter(P, V) p(upper_tree.main_branch());
	for_all(p)
	  {
	    fllt_node(P, V)& n(p);
	    fill_a_shape< P, V, upper<V> >(n, upper_tree, upp_reg, low_reg);
	    mln_assertion(lower_tree.check_consistency());
	    mln_assertion(upper_tree.check_consistency());
	  }
      }

      // FIXME : this is a wrong way to choose the root of the result
      // tree. lower and upper root doesn't have the same level, we
      // want the right level for the background.
      fllt_tree(P, V)* main_tree = &lower_tree;
      fllt_tree(P, V)* other_tree = &upper_tree;

      if (lower_tree.root()->elt().points.npoints() >= ima.domain().npoints())
      {
	main_tree = &upper_tree;
	other_tree = &lower_tree;
      }

      typename fllt_node(P, V)::children_t::iterator it;
      for (it = other_tree->root()->children().begin();
      	   it != other_tree->root()->children().end(); )
      {
      	main_tree->root()->add_child(*it);
      }
      mln_assertion(main_tree->check_consistency());
      return *main_tree;
    }

  } // end of namespace mln::fllt

} // end of namespace mln

#endif // ! MLN_FIXME_FLLT_MERGE_HH
