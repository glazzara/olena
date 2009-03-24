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

#ifndef MLN_MORPHO_TREE_PROPAGATE_RUN_HH_
# define MLN_MORPHO_TREE_PROPAGATE_RUN_HH_

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>

# include <mln/core/site_set/p_array.hh>
# include <mln/util/pix.hh>

# include "propagate_node.hh"

namespace mln {
  namespace morpho {
    namespace tree {


      /**
      ** Traverse component tree \p tree and provide tree nodes values
      ** to accumulator \p accu_. (Alias to "map")
      **
      ** @param tree Component tree used to perform accumulation.
      ** @param a_ Attributed image.
      ** @accu_ Accumulator that will be applied on tree nodes.
      **
      ** @return The result of accumulation.
      */
      template <typename T, typename A, typename I>
      mln_result(A)
      run(const T& tree,
	  const Image<I>& a_,
	  Accumulator<A>& accu_);

      /**
      ** Apply accumulator \accu on tree nodes. If the resulting node of
      ** accumulation checks predicate \p pred, the node is inserted
      ** into resulting array, then ascendant and descendant zero-fill
      ** propagations are performed from the node. These operations
      ** are repeated until the node won't check predicate.
      **
      ** @param tree Component tree used for propagation.
      ** @param a Attributed image where values are propagated.
      ** @param accu_ Accumulator to apply on tree.
      ** @param pred Predicate that must check the result of
      ** accumulator to propagate the node.
      **
      ** @return Array of propagated nodes.
      */
      template <typename T, typename A, typename ACC, typename P2B>
      p_array< mln_psite(A) >
      run_while(const T& tree,
		Image<A>& a,
		Accumulator<ACC>& accu_,
		Function_p2b<P2B>& pred);


      /**
      ** Apply accumulator \accu on tree nodes values n times.
      ** Each time, the result of accumulator is inserted
      ** into the returned array, then ascendant and descendant zero-fill
      ** propagations are performed from the node.
      ** (This function is a shorcut of run_while with a cardinality predicate).
      **
      ** @param tree Component tree used for propagation.
      ** @param a Attributed image where values are propagated.
      ** @param acc Accumulator to apply on tree.
      ** @param n The repetition number.
      **
      ** @return Array of propagated nodes.
      */
      template <typename T, typename A, typename ACC>
      inline
      p_array< mln_psite(A) >
      run_ntimes(const T& tree,
		 Image<A>& a,
		 Accumulator<ACC>& acc,
		 unsigned n);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename T, typename A, typename ACC, typename P2B>
	p_array< mln_psite(A) >
	run_while(const T& tree,
		  A& a,
		  ACC& accu,
		  P2B& pred)
	{
	  mln_psite(A) p;
	  p_array< mln_psite(A) > arr_sites;
	  util::array< mln_value(A) > arr_values;

	  p = morpho::tree::run(tree, a, accu);
	  while (pred(p) && a(p) != 0)
	    {
	      arr_sites.insert(p);
	      arr_values.append(a(p));
	      morpho::tree::propagate_node_to_descendants(p, tree, a, 0);
	      morpho::tree::propagate_node_to_ancestors(p, tree, a, 0);
	      a(p) = 0;
	      p = morpho::tree::run(tree, a, accu);
	    }
	  for (unsigned i = 0; i < arr_sites.nsites(); i++)
	    a(arr_sites[i]) = arr_values[i];
	  return arr_sites;
	}

	struct ncard : Function_p2b< ncard >
	{
	  typedef bool result;

	  ncard(unsigned n)
	    : n_ (n)
	  {
	  }

	  template <typename P>
	  bool operator()(const P& p)
	  {
	    (void)p;
	    return (n_-- > 0);
	  }

	private:
	  unsigned n_;
	};


      } // end of namespace mln::morpho::tree::internal

      template <typename T, typename A, typename ACC, typename P2B>
      inline
      p_array< mln_psite(A) >
      run_while(const T& tree,
		Image<A>& a_,
		Accumulator<ACC>& acc,
		Function_p2b<P2B>& pred)
      {
	A& a = exact(a_);

	mln_precondition(tree.f().domain() == a.domain());
	mln_precondition(a.is_valid());

	return internal::run_while(tree, a, exact(acc), exact(pred));
      }

      template <typename T, typename A, typename ACC>
      inline
      p_array< mln_psite(A) >
      run_ntimes(const T& tree,
		 Image<A>& a,
		 Accumulator<ACC>& acc,
		 unsigned n)
      {
	internal::ncard predicate(n - 1);
	return run_while(tree, a, acc, predicate);
      }

      template <typename T, typename A, typename I>
      mln_result(A)
      run(const T& tree,
	  const Image<I>& a_,
	  Accumulator<A>& accu_)
      {
	A& accu = exact(accu_);
	const I& a = exact(a_);

	mln_precondition(tree.f().domain() == a.domain());
	mln_precondition(a.is_valid());

	mln_up_node_piter(T) n(tree);
	for_all(n)
	  accu.take(make::pix(a, n));
	return (accu.to_result());
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace mln::morpho::tree
  } // end of namespace mln::morpho
} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_PROPAGATE_RUN_HH_ */
