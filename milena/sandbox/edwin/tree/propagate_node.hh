// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_MORPHO_TREE_PROPAGATE_NODE_HH_
# define MLN_MORPHO_TREE_PROPAGATE_NODE_HH_

#include <mln/morpho/tree/data.hh>
#include <mln/core/site_set/p_array.hh>

/// \file mln/morpho/tree/propagate_node.hh
///
/// Functions to propagate node in the tree.

namespace mln {
  namespace morpho {
    namespace tree {


      /**
      ** Propagate a value to a node and its descendants.
      **
      ** @param n Forward iterator related to the node.
      ** @param t Reference to components tree.
      ** @param a_ Reference to image.
      ** @param v Value to propagate. Default is a_(n).
      */
      template <typename T, typename A>
      void
      propagate_node_to_descendants(mln_bkd_piter(T::nodes_t)& n,
				    const T& t,
				    Image<A>& a_,
				    mln_value(A) v);

      /**
      ** Propagate the value of a node to its descendants.
      **
      ** @param n Forward iterator related to the node.
      ** @param t Reference to components tree.
      ** @param a_ Reference to image.
      */
      template <typename T, typename A>
      void
      propagate_node_to_descendants(mln_bkd_piter(T::nodes_t)& n,
				    const T& t,
				    Image<A>& a_);


      /**
      ** Propagate a value to a node and its descendants.
      **
      ** @param n The root of subtree which value propagates in.
      ** @param t The reference to components tree.
      ** @param a_ The reference to image.
      ** @param v The value to propagate. Default is a_(n).
      */
      template <typename T, typename A>
      void
      propagate_node_to_descendants(mln_psite(A) n,
				    const T& t,
				    Image<A>& a_,
				    mln_value(A) v);

      /**
      ** Propagate the node's value to its descendants.
      **
      ** @param n The root of subtree which value propagates in.
      ** @param t The reference to components tree.
      ** @param a_ The reference to image.
      */
      template <typename T, typename A>
      void
      propagate_node_to_descendants(mln_psite(A)& n,
				    const T& t,
				    Image<A>& a_);

      /**
      ** Propagate a value from a node to its ancestors.
      **
      ** @param n Forward iterator related to the node.
      ** @param t Reference to components tree.
      ** @param a_ Reference to image.
      ** @param v Value to propagate.
      */
      template <typename T, typename A>
      void
      propagate_node_to_ancestors(mln_psite(A) n,
				  const T& t,
				  Image<A>& a_,
				  mln_value(A) v);

      /**
      ** Propagate the node's value to its ancestors.
      **
      ** @param n Forward iterator related to the node.
      ** @param t Reference to components tree.
      ** @param a_ Reference to image.
      */
      template <typename T, typename A>
      void
      propagate_node_to_ancestors(mln_psite(A) n,
				  const T& t,
				  Image<A>& a_);






      /* Descendants propagation */

      template <typename T, typename A>
      void
      propagate_node_to_descendants(mln_bkd_piter(T::nodes_t)& n,
				    const T& t,
				    Image<A>& a_,
				    mln_value(A) v)
      {
	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());
	mln_precondition(n.is_valid());
	mln_precondition(t.is_a_node(n));

	mln_ch_value(A, bool) ancestors;
	initialize(ancestors, a);
	data::fill(ancestors, false);
	ancestors(n) = true;

	mln_bkd_piter(T::nodes_t) it(n);
	for (it.next(); it.is_valid(); it.next())
	  {
	    if (ancestors(t.parent(it)))
	      {
		ancestors(it) = true;
		a(it) = v;
	      }
	  }
      }

      template <typename T, typename A>
      inline
      void
      propagate_node_to_descendants(mln_bkd_piter(T::nodes_t)& n,
				    const T& t,
				    Image<A>& a_)
      {
	A& a = exact(a_);
	propagate_node_to_descendants(n, t, a, a(n));
      }

      template <typename T, typename A>
      void
      propagate_node_to_descendants(mln_psite(A) n,
				    const T& t,
				    Image<A>& a_,
				    mln_value(A) v)
      {
	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());
	mln_precondition(a.domain().has(n));


	if (!t.is_a_node(n)) // Get the representant.
	  n = t.parent(n);

	mln_bkd_piter(T::nodes_t) it = find_bkd(t.nodes(), n);
	propagate_node_to_descendants(it, t, a, v);
      }

      template <typename T, typename A>
      inline
      void
      propagate_node_to_descendants(mln_psite(A) n,
				    const T& t,
				    Image<A>& a_)

      {
	A& a = exact(a_);
	propagate_node_to_descendants(n, t, a, a(n));
      }


      /* Ancestors propagation */

      template <typename T, typename A>
      void
      propagate_node_to_ancestors(mln_psite(A) n,
				  const T& t,
				  Image<A>& a_,
				  mln_value(A) v)
      {
	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());
	mln_precondition(a.domain().has(n));

	if (!t.is_a_node(n))
	  n = t.parent(n);
	mln_assertion(t.is_a_node(n));

	if (t.is_root(n))
	  return;

	do {
	  n = t.parent(n);
	  a(n) = v;
	} while (!t.is_root(n));

      }

      template <typename T, typename A>
      inline
      void
      propagate_node_to_ancestors(mln_psite(A) n,
				  const T& t,
				  Image<A>& a_)
      {
	A& a = exact(a_);
	propagate_node_to_ancestors(n, t, a, a(n));
      }


    } // end of namespace mln::morpho::tree
  } // end of namespace mln::morpho
} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_PROPAGATE_NODE_HH_ */
