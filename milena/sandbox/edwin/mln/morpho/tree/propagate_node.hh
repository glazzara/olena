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

# include <mln/core/concept/image.hh>
# include <mln/core/macros.hh>
# include <mln/morpho/tree/data.hh>

/// \file mln/morpho/tree/propagate_node.hh
///
/// Functions to propagate node in the tree.

namespace mln {
  namespace morpho {
    namespace tree {

      /**
      ** Propagate a value \p v from a node \p n to its descendants.
      **
      ** @param n[in] Node to propagate.
      ** @param t[in] Component tree used for propagation.
      ** @param a_[in] Attribute image where values are propagated.
      ** @param v[in] Value to propagate.
      ** @param nb_leaves[out] Optional. Store the number of leaves in
      ** the component.
      */
      template <typename T, typename A>
      void
      propagate_node_to_descendants(mln_psite(A) n,
				    const T& t,
				    Image<A>& a_,
				    const mln_value(A)& v,
				    unsigned* nb_leaves = 0);

      /**
      ** Propagate the node's value to its descendants.
      **
      ** @param n[in] Node to propagate.
      ** @param t[in] Component tree used for propagation.
      ** @param a_[in] Attribute image where values are propagated.
      ** @param nb_leaves[out] Optional. Store the number of leaves in
      ** the component.
      */
      template <typename T, typename A>
      inline
      void
      propagate_node_to_descendants(mln_psite(A)& n,
				    const T& t,
				    Image<A>& a_,
				    unsigned* nb_leaves = 0);


      /**
      ** Propagate a value \v from a node \n to its ancestors.
      **
      ** @param n Node to propagate.
      ** @param t Component tree used for propagation.
      ** @param a_ Attribute image where values are propagated.
      ** @param v Value to propagate.
      */
      template <typename T, typename A>
      void
      propagate_node_to_ancestors(mln_psite(A) n,
				  const T& t,
				  Image<A>& a_,
				  const mln_value(A)& v);

      /**
      ** Propagate the node's value to its ancestors.
      **
      ** @param n Node to propagate.
      ** @param t Component tree used for propagation.
      ** @param a_ Attribute image where values are propagated.
      */
      template <typename T, typename A>
      inline
      void
      propagate_node_to_ancestors(mln_psite(A) n,
				  const T& t,
				  Image<A>& a_);


      # ifndef MLN_INCLUDE_ONLY

      /* Descendants propagation */


      template <typename T, typename A>
      inline
      void
      propagate_node_to_descendants(mln_psite(A) n,
				    const T& t,
				    Image<A>& a_,
				    const mln_value(A)& v,
				    unsigned* nb_leaves = 0)
      {
	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());
	mln_precondition(a.domain().has(n));


	if (!t.is_a_node(n)) // Get the representant.
	  n = t.parent(n);
	mln_assertion(t.is_a_node(n));

	typename T::preorder_piter pp(t, n);

	pp.start(); // We don't set n to v.

	if (nb_leaves)
	  *nb_leaves += t.is_a_leaf(pp);

	for (pp.next(); pp.is_valid(); pp.next())
	  {
	    a(pp) = v;
	    if (nb_leaves && t.is_a_leaf(pp))
	      ++(*nb_leaves);
	  }
      }


      template <typename T, typename A>
      inline
      void
      propagate_node_to_descendants(mln_psite(A) n,
				    const T& t,
				    Image<A>& a_,
				    unsigned* nb_leaves = 0)

      {
	A& a = exact(a_);
	propagate_node_to_descendants(n, t, a, a(n), nb_leaves);
      }


      /* Ancestors propagation */

      template <typename T, typename A>
      void
      propagate_node_to_ancestors(mln_psite(A) n,
				  const T& t,
				  Image<A>& a_,
				  const mln_value(A)& v)
      {
	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());
	mln_precondition(a.domain().has(n));

	if (!t.is_a_node(n)) // Get the representant.
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

      # endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree
  } // end of namespace mln::morpho
} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_PROPAGATE_NODE_HH_ */
