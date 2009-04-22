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

#ifndef GRANULOMETRY_HH_
# define GRANULOMETRY_HH_

# include <mln/core/concept/image.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/morpho/tree/data.hh>

# include <queue>
# include <map>

namespace mln {
  namespace morpho {
    namespace tree {


      template <typename T, typename A>
      inline
      std::map<mln_value(A), unsigned>
      compute_attribute_curve_beta(const T& tree, const Image<A>& attr_img);


      template <typename T, typename A>
      inline
      std::map<mln_value(A), unsigned>
      compute_attribute_curve(const T& tree, const Image<A>& attr_img);


# ifndef MLN_INCLUDE_ONLY

      namespace internal {

	template <typename A>
	struct attr_less
	{
	  attr_less(const A& f)
	    : f_ (f)
	  {
	  }

	  bool operator () (const mln_psite(A)& a, const mln_psite(A)& b) const
	  {
	    return (f_(a) < f_(b));
	  }

	private:
	  const A& f_;
	};


      }

      template <typename T, typename A>
      inline
      std::map<mln_value(A), unsigned>
      compute_attribute_curve(const T& tree, const Image<A>& attr_img)
      {
	const A& a = exact(attr_img);

	std::map< mln_value(A), unsigned > f;
	mln_node_piter(T) n(tree);

	for_all(n)
	  f[a(n)]++;


	return f;
      }


      template <typename T, typename A>
      inline
      std::map<mln_value(A), unsigned>
      compute_attribute_curve_beta(const T& tree, const Image<A>& attr_img)
      {
	const A& a = exact(attr_img);

	// TODO: precondition attribut croissant


	typedef std::priority_queue< mln_psite(A), std::vector< mln_psite(A) >, internal::attr_less<A> > q_type;
	std::map< mln_value(A), unsigned > f;
 	internal::attr_less<A> cmp(a);
  	q_type pqueue(cmp);
	unsigned active_components = 1;

	pqueue.push(tree.nodes()[tree.nodes().nsites() - 1]);

	// TODO: push other roots

	while (!pqueue.empty())
	  {
	    mln_value(A) node_v = a(pqueue.top());
	    mln_psite(A) node;
	    std::cout << pqueue.top() << " : " << node_v << std::endl;
	    f[node_v] = pqueue.size();
	    do {
	      ++active_components;
	      node = pqueue.top();
	      pqueue.pop();

	      mln_piter(p_array< mln_psite(A) >) child(tree.children(node));
	      for_all(child)
	      {
		pqueue.push(child);
	      }
 	    } while (!pqueue.empty() && a(pqueue.top()) == node_v);
	  }
	// TODO: postcondition decroissant
	return f;
      }


# endif /* !MLN_INCLUDE_ONLY */


    }  // end of namespace mln::morpho::tree
  }  // end of namespace mln::morpho
} // end of namespace mln


#endif /* !GRANULOMETRY_HH_ */
