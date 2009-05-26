// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_MORPHO_TREE_PROPAGATE_IF_HH_
# define MLN_MORPHO_TREE_PROPAGATE_IF_HH_

/**
** @file   mln/morpho/tree/propagate_if.hh
**
** @brief Routines to handle propagation with predicate.
**
*/

# include <mln/morpho/tree/data.hh>
# include <mln/morpho/tree/propagate_node.hh>

# include <mln/data/fill.hh>
# include <mln/pw/all.hh>

namespace mln {
  namespace morpho {
    namespace tree {

      template <typename WP>
      struct way_of_propagation : Object< WP > { protected: way_of_propagation() {}; };
      struct desc_propagation : way_of_propagation <desc_propagation> {};
      struct asc_propagation : way_of_propagation <asc_propagation> {};

      /**
      ** Propagate nodes checking the predicate \p pred in the way
      ** defined by \p way_of_propagation.
      **
      ** @param tree Component tree used for propagation.
      ** @param a_ Attributed image where values are propagated.
      ** @param way_of_propagation Propagate node in acsendant or
      ** descendant way.
      ** @param pred Predicate that node must check to be propagated.
      ** @param v Value to be propagated. (By default \p v is the value
      ** at the node being propagated).
      */
      template <typename T, typename A, typename P2B, typename WP>
      inline
      void
      propagate_if(const T& tree,
		   Image<A>& a_,
		   const way_of_propagation<WP>&,
		   const Function_p2b<P2B>& pred,
		   const mln_value(A)& v);

      template <typename T, typename A, typename P2B>
      inline
      void
      propagate_if(const T& tree,
		   Image<A>& a_,
		   const desc_propagation&,
		   const Function_p2b<P2B>& pred);

      /**
      ** Propagate nodes having the value v in the way
      ** defined by \p way_of_propagation.
      **
      ** @param tree Component tree used for propagation.
      ** @param a_ Attributed image where values are propagated.
      ** @param way_of_propagation Propagate node in acsendant or
      ** descendant way.
      ** @param v Value that node must have to be propagated.
      ** @param v_prop Value to propagate (By default it is the value
      ** at the node being propagated).
      */
      template <typename T, typename A, typename WP>
      inline
      void
      propagate_if_value(const T& tree,
			 Image<A>& a_,
			 const way_of_propagation<WP>&,
			 const mln_value(A)& v,
			 const mln_value(A)& v_prop);

      template <typename T, typename A, typename WP>
      inline
      void
      propagate_if_value(const T& tree,
			 Image<A>& a_,
			 const way_of_propagation<WP>&,
			 const mln_value(A)& v);




# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {
	template <typename T, typename A, typename P2B>
	bool check_propagate_if(const T& t,
				const A& a,
				const asc_propagation& prop,
				const P2B& pred,
				const mln_value(A)& v)
	{
	  (void) prop;
	  mln_node_piter(T) n(t);
	  for_all(n)
	    if (pred(n) && a(t.parent(n)) != v)
	      return false;
	  return true;
	}

	template <typename T, typename A, typename P2B>
	bool check_propagate_if(const T& t,
				const A& a,
				const desc_propagation& prop,
				const P2B& pred,
				const mln_value(A)& v)
	{
	  (void) prop;
	  mln_node_piter(T) n(t);
	  for_all(n)
	    if (a(n) != v && pred(t.parent(n)))
	      return false;
	  return true;
	}

	template <typename T, typename A, typename P2B>
	bool check_propagate_if(const T& t,
				const A& a,
				const desc_propagation& prop,
				const P2B& pred)
	{
	  (void) prop;
	  mln_node_piter(T) n(t);
	  for_all(n)
	    if (a(n) != a(t.parent(n)) && pred(t.parent(n)))
	      return false;
	  return true;
	}

	template <typename T, typename A, typename P2B>
	inline
	void
	propagate_if(const T& tree,
		     A& a,
		     const desc_propagation& prop,
		     const P2B& pred,
		     const mln_value(A)& v)
	{
	  (void) prop;

	  mln_precondition(a.is_valid());
	  mln_precondition(tree.f().domain() == a.domain());

	  mln_ch_value(typename T::function, bool) mark;
	  initialize(mark, tree.f());
	  mln::data::fill(mark, false);

	  mln_dn_node_piter(T) n(tree);
	  for_all(n)
	    if (mark(tree.parent(n)))
	      {
		a(n) = v;
		mark(n) = true;
	      }
	    else if (pred(n))
	      mark(n) = true;
	  mln_postcondition(check_propagate_if(tree, a, prop, pred, v));
	}

	template <typename T, typename A, typename P2B>
	inline
	void
	propagate_if(const T& tree,
		     A& a,
		     const desc_propagation& prop,
		     const P2B& pred)
	{
	  (void) prop;

	  mln_precondition(a.is_valid());
	  mln_precondition(tree.f().domain() == a.domain());

	  mln_ch_value(typename T::function, bool) mark;
	  initialize(mark, tree.f());
	  mln::data::fill(mark, false);

	  mln_dn_node_piter(T) n(tree);
	  for_all(n)
	    if (mark(tree.parent(n)))
	      {
		a(n) = a(tree.parent(n));
		mark(n) = true;
	      }
	    else if (pred(n))
	      mark(n) = true;
	  mln_postcondition(check_propagate_if(tree, a, prop, pred));
	}


	template <typename T, typename A, typename P2B>
	inline
	void
	propagate_if(const T& tree,
		     A& a,
		     const asc_propagation& prop,
		     const P2B& pred,
		     const mln_value(A)& v)
	{
	  (void) prop;

	  mln_precondition(a.is_valid());
	  mln_precondition(tree.f().domain() == a.domain());

	  mln_ch_value(typename T::function, bool) mark;
	  initialize(mark, tree.f());
	  mln::data::fill(mark, false);

	  mln_up_node_piter(T) n(tree);
	  for_all(n)
	    if (mark(n))
	      {
		a(n) = v;
		mark(tree.parent(n)) = true;
	      }
	    else if (pred(n))
	      mark(tree.parent(n)) = true;

	  mln_postcondition(check_propagate_if(tree, a, prop, pred, v));
	}

      } // end of namespace mln::morpho::tree::internal


      /* Facades */

      template <typename T, typename A, typename WP>
      inline
      void
      propagate_if_value(const T& tree,
			 Image<A>& a_,
			 const way_of_propagation<WP>& prop_,
			 const mln_value(A)& v,
			 const mln_value(A)& v_prop)
      {
	A& a = exact(a_);
	const WP& prop = exact(prop_);

	internal::propagate_if(tree, a, prop, pw::value(a) == pw::cst(v), v_prop);
      }


      template <typename T, typename A, typename WP>
      inline
      void
      propagate_if_value(const T& tree,
			 Image<A>& a_,
			 const way_of_propagation<WP>& prop_,
			 const mln_value(A)& v)
      {
	A& a = exact(a_);
	const WP& prop = exact(prop_);

	internal::propagate_if(tree, a, prop, pw::value(a) == pw::cst(v), v);
      }

      template <typename T, typename A, typename P2B, typename WP>
      inline
      void
      propagate_if(const T& tree,
		   Image<A>& a_,
		   const way_of_propagation<WP>& prop_,
		   const Function_p2b<P2B>& pred_,
		   const mln_value(A)& v)
      {
	A& a = exact(a_);
	const WP& prop = exact(prop_);
	const P2B& pred = exact(pred_);

	internal::propagate_if(tree, a, prop, pred, v);
      }

      template <typename T, typename A, typename P2B>
      inline
      void
      propagate_if(const T& tree,
		   Image<A>& a_,
		   const desc_propagation& prop,
		   const Function_p2b<P2B>& pred_)
      {
	A& a = exact(a_);
	const P2B& pred = exact(pred_);

	internal::propagate_if(tree, a, prop, pred);
      }

#endif /* !MLN_INCLUDE_ONLY */


    } // end of namespace mln::morpho::tree
  } // end of namespace mln::morpho
} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_PROPAGATE_IF_HH_ */
