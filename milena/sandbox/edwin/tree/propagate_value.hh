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

#ifndef MLN_MORPHO_TREE_PROPAGATE_VALUE_HH_
# define MLN_MORPHO_TREE_PROPAGATE_VALUE_HH_


/// \file mln/morpho/tree/propagate_value.hh
///
/// Functions to propagate a value in the tree.

namespace mln {
  namespace morpho {
    namespace tree {

     /**
      ** Propagate the value \p v2 from node \p n having the value \p v
      ** to its ancestors.
      **
      ** @param v Value that node must have to be propagated.
      ** @param t Reference to components tree.
      ** @param a_ Reference to image.
      ** @param v2 Value to propagate (\p v by default).
      */
      template <typename T, typename A>
      void
      propagate_value_to_ancestors(mln_value(A) v,
				   const T& t,
				   Image<A>& a_,
				   mln_value(A) v2);

      /**
      ** Propagate the value \p v2 from node \p n having the value \p v
      ** to its descendants.
      **
      ** @param v Value that node must have to be propagated.
      ** @param t Reference to components tree.
      ** @param a_ Reference to image.
      ** @param v2 Value to propagate (\p v by default).
      */
      template <typename T, typename A>
      void
      propagate_value_to_descendants(mln_value(A) v,
				     const T& t,
				     Image<A>& a_,
				     mln_value(A) v2);









      namespace internal
      {
	template <typename T, typename A>
	bool check_propagate_ancestors(const T& t, const A& a, mln_value(A) v)
	{
	  mln_up_node_piter(T) n(t.nodes());
	  for_all(n)
	    if (a(n) == v && a(t.parent(n)) != v)
	      return false;
	  return true;
	}

	template <typename T, typename A>
	bool check_propagate_descendants(const T& t, const A& a, mln_value(A) v)
	{
	  mln_up_node_piter(T) n(t.nodes());
	  for_all(n)
	    if (a(n) != v && a(t.parent(n)) == v)
	      return false;
	  return true;
	}

      } // end of namespace mln::morpho::tree::internal

      template <typename T, typename A>
      void
      propagate_value_to_ancestors(mln_value(A) v,
				   const T& t,
				   Image<A>& a_,
				   mln_value(A) v2)
      {
	if (v == v2)
	  {
	    propagate_value_to_ancestors(v, t, a_);
	    return;
	  }

	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());

	mln_ch_value(A, bool) deja_vu;
	initialize(deja_vu, a);
	data::fill(deja_vu, false);

	mln_up_node_piter(T) n(t.nodes());
	for_all(n)
	{
	  if (a(n) == v || deja_vu(n))
	    {
	      mln_assertion(t.is_a_node(t.parent(n)));
	      a(t.parent(n)) = v2;
	      deja_vu(t.parent(n)) = true;
	    }
	}

	mln_postcondition(internal::check_propagate_ancestors(t, a, v2));
      }


      template <typename T, typename A>
      void
      propagate_value_to_ancestors(mln_value(A) v,
				   const T& t,
				   Image<A>& a_)
      {
	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());

	mln_up_node_piter(T) n(t.nodes());
	for_all(n)
	{
	  if (a(n) == v)
	    {
	      mln_assertion(t.is_a_node(t.parent(n)));
	      a(t.parent(n)) = v;
	    }
	}

	mln_postcondition(internal::check_propagate_ancestors(t, a, v));
      }


      template <typename T, typename A>
      void
      propagate_value_to_descendants(mln_value(A) v,
				     const T& t,
				     Image<A>& a_,
				     mln_value(A) v2)
      {
	if (v == v2)
	  {
	    propagate_value_to_descendants(v, t, a_);
	    return;
	  }

	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());

	mln_ch_value(A, bool) deja_vu;
	initialize(deja_vu, a);
	data::fill(deja_vu, false);

	mln_dn_node_piter(T) n(t.nodes());
	for_all(n)
	{
	  if (a(n) == v)
	    {
	      mln_assertion(t.is_a_node(t.parent(n)));
	      deja_vu(n) = true;
	    }
	  else if (deja_vu(t.parent(n)))
	    {
	      mln_assertion(t.is_a_node(t.parent(n)));
	      a(n) = v2;
	      deja_vu(n) = true;
	    }
	}
	mln_postcondition(internal::check_propagate_descendants(t, a, v2));
      }

      template <typename T, typename A>
      void
      propagate_value_to_descendants(mln_value(A) v,
				     const T& t,
				     Image<A>& a_)
      {
     	A& a = exact(a_);
	mln_precondition(a.is_valid());
	mln_precondition(a.domain() == t.f().domain());

	mln_dn_node_piter(T) n(t.nodes());
	for_all(n)
	{
	  if (a(t.parent(n)) == v)
	    {
	      mln_assertion(t.is_a_node(t.parent(n)));
	      a(n) = v;
	    }
	}
	mln_postcondition(internal::check_propagate_descendants(t, a, v));
      }


    } // end of namespace mln::morpho::tree
  } // end of namespace mln::morpho
} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_PROPAGATE_VALUE_HH_ */
