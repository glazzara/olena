// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_PROPAGATE_HH_
# define MLN_MORPHO_TREE_PROPAGATE_HH_

/// \file mln/morpho/tree/propagate.hh
///
/// Functions to propagate a node value in the tree.

#include <mln/morpho/tree/data.hh>

namespace mln {
  namespace morpho {
    namespace tree {


      /// Propagate the representative point's value to
      /// non-representative points of the same node.
      template <typename T, typename A>
      void
      propagate_representant(const T& t, Image<A>& a_)
      {
	A a = exact(a_);
	mln_fwd_piter(T) p(t.domain());
	for_all(p)
	  if (! t.is_a_node(p))
	    {
	      mln_assertion(t.is_a_node(t.parent(p)));
	      a(p) = a(t.parent(p));
	    }
      }





      /// Dans le cas des images binaires...
      /// Propagate a tagged node's value to its subbranches.
	template <typename T, typename A>
	void
	propagate_to_childhood(const T& t, A& a)
	{
	  mln_bkd_piter(T::nodes_t) n(t.nodes());
	  for_all(n)
	  {
	    if (a(t.parent(n)))
	      {
		mln_assertion(t.is_a_node(t.parent(n)));
		a(n) = a(t.parent(n));
	    }
	  }
	}


      /// Propagate a tagged node's value to its direct children.
      template <typename T, typename A>
      void
      propagate_to_children(const T& t, A& a)
      {
	mln_fwd_piter(T::nodes_t) n(t.nodes());
	for_all(n)
	{
	  if (a(t.parent(n)))
	    {
	      mln_assertion(t.is_a_node(t.parent(n)));
	      a(n) = a(t.parent(n));
	    }
	}
      }

      /// Propagate a tagged node's value to its ancestors.
      template <typename T, typename A>
      void
      propagate_to_ancestors(const T& t, A& a)
      {
	mln_fwd_piter(T::nodes_t) n(t.nodes());
	for_all(n)
	{
	  if (a(n))
	    {
	      mln_assertion(t.is_a_node(t.parent(n)));
	      a(t.parent(n)) = a(n);
	    }
	}
      }

      /// Propagate a tagged node's value to its direct parents.
      template <typename T, typename A>
      void
      propagate_to_parent(const T& t, A& a)
      {
	mln_bkd_piter(T::nodes_t) n(t.nodes());
	for_all(n)
	{
	  mln_assertion(t.is_a_node(n));
	  if (a(n))
	    {
	      mln_assertion(t.is_a_node(t.parent(n)));
	      a(t.parent(n)) = a(n);
	    }
	}
      }

      /// Propagate a tagged leaf's value to its ancestors.
      /// In others words, tag the branch which the tagged leaf belongs to.
      /// At the end, the tree should get this property:
      /// for all n
      ///   a(n) is true iff there exits an l in tree's leaves such that
      ///       n <== l and a(l) is true.
      /// TODO: post-condition which checks this property.


      template <typename T, typename A>
      void
      propagate_leaf_to_ancestors(const T& t, A& a)
      {

	mln_fwd_piter(T::leaves_t) l(t.leaves());
	for_all(l)
	  a(t.parent(l)) = 0;


	mln_fwd_piter(T::nodes_t) n(t.nodes());
	for_all(n)
	{
	  mln_assertion(t.is_a_node(t.parent(n)));
	  a(t.parent(n)) = a(t.parent(n)) || a(n);
	}
      }



    } // end of namespace mln::morpho::tree
  } // end of namespace mln::morpho
} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_PROPAGATE_HH_ */


