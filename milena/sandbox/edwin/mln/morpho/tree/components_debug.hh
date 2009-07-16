// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_MORPHO_TREE_COMPONENTS_HH_
# define MLN_MORPHO_TREE_COMPONENTS_HH_

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/data/fill.hh>

# include <mln/morpho/tree/propagate_node.hh>
# include <mln/morpho/tree/data.hh>
# include <mln/trace/entering.hh>
# include <mln/trace/exiting.hh>

/* Debug */
# include <mln/morpho/tree/debug.hh>
# include <mln/morpho/tree/propagate_representative.hh>
# include <mln/core/image/image2d.hh>
# include <mln/literal/colors.hh>
# include <mln/io/ppm/save.hh>
# include <iostream>
# include <fstream>
# include <sstream>
# include <unistd.h>

/**
** \file mln/morpho/tree/components.hh
**
** Routines that permits to retrieve tree components.
** Tree components are nodes maximising the attribute.
** The code is instrumented for debugging.
*/



namespace mln {

  namespace morpho {

    namespace tree {

      /**
      ** Retrieve components from the tree until all leaves belong to
      ** components.
      **
      ** @param tree Component tree.
      ** @param attr_image Attribute image.
      **
      ** @return Array of components.
      */
      template <typename T, typename A>
      p_array< mln_psite(A) >
      get_components(const T& tree, const Image<A>& attr_image);

      /**
      ** Retrieve the \p n most important components from the tree. n
      ** should be lesser than the maximum number of components. If
      ** not, the functions stops when there's no more components.
      **
      ** @param tree Component tree.
      ** @param attr_image Attribute image.
      ** @param n Number of components to get.
      **
      ** @return Array of components.
      */
      template <typename T, typename A>
      p_array< mln_psite(A) >
      get_components(const T& tree, const Image<A>& attr_image, unsigned n);

      /**
      ** Retrieve the most important components that check
      ** predicate \p pred.
      **
      ** @param tree Component tree.
      ** @param attr_image Attribute image.
      ** @param pred Predicate that components must check.
      **
      ** @return Array of components.
      */
      template <typename T, typename A, typename P2B>
      p_array< mln_psite(A) >
      get_components(const T& tree, const Image<A>& attr_image, const Function_v2b<P2B>& pred);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {
	template <typename P, typename I>
	struct desc_sort
	{
	  bool operator()(const P& p1, const P& p2) const
	  {
	    return ima(p1) > ima(p2);
	  }
	  const I& ima;

	  desc_sort(const I& ima) : ima(ima) {}
	};

	template <typename P, typename I>
	inline
	void
	sort(p_array<P>& arr, const I& ima_)
	{
	  mlc_converts_to(P, mln_psite(I))::check();

	  const I& ima = exact(ima_);
	  std::vector<P>& v = arr.hook_std_vector_();
	  std::sort(v.begin(), v.end(), desc_sort<P,I>(ima));
	}

	template <typename A>
	inline
	bool
	get_max(const p_array< mln_psite(A) >& arr, const A& activity, unsigned& pos)
	{
	  while (pos < arr.nsites() && !activity(arr[pos]))
	    ++pos;
	  return (pos < arr.nsites());
	}


	//////////////////////////
	// Predifined predicate //
	//////////////////////////

	/// Predicate used to gets components until tree leaves last.
	template <typename T>
	class pred_tree_leaves_last
	{
	public:
	  pred_tree_leaves_last(const T& tree) :
	    n_ (tree.leaves().nsites())
	  {
	  }

	  template <typename P>
	  bool operator() (const P& p, unsigned n, unsigned* nb_leaves) const
	  {
	    (void)p;
	    (void)n;
	    mln_assertion(nb_leaves != 0);

	    return (*nb_leaves < n_);
	  }


	private:
	  unsigned n_;
	};

	/// Predicate used to get n components.
	class pred_n_components
	{
	public:
	  pred_n_components(unsigned n) :
	    n_(n)
	  {
	  }

	  template <typename P>
	  bool operator() (const P& p, unsigned n, unsigned* nb_leaves) const
	  {
	    (void)p;
	    (void)nb_leaves;

	    return (n < n_);
	  }


	private:
	  unsigned n_;
	};

	/// Predicate used to get components that check a p2b function predicate.
	template <typename P2B>
	class pred_p2b
	{
	public:
	  pred_p2b(const P2B& f) :
	    f_ (f)
	  {
	  }

	  template <typename P>
	  bool operator() (const P& p, unsigned n, unsigned* nb_leaves) const
	  {
	    (void)n;
	    (void)nb_leaves;

	    return (f_(p));
	  }


	private:
	  const P2B& f_;
	};

	template <typename T, typename A, class P>
	p_array< mln_psite(A) >
	get_components(const T& tree, const A& a, const P& pred, bool uses_leaves = 0)
	{

	  mln_psite(A) p;
	  p_array< mln_psite(A) > components;
	  mln_ch_value(A, bool) activity;
	  p_array< mln_psite(A) > max_arr = tree.nodes();
	  unsigned arr_pos = 0; // position in max_array
	  unsigned n = 0;
	  unsigned* nb_leaves = uses_leaves ? new unsigned(0) : 0;

	  initialize(activity, a);
	  mln::data::fill(activity, true);
	  internal::sort(max_arr, a);

	  unsigned cpt = 0;

	  do
	    {
	      if (!internal::get_max(max_arr, activity, arr_pos))
		break;
	      p = max_arr[arr_pos];

	      { // debug
		std::cout << "# " << cpt << " Retrieve " << p << " with attribute value: "
			  << a(p) << std::endl;
	      }


	      if (a(p) == 0)
		break;
	      components.insert(p);
	      morpho::tree::propagate_node_to_descendants(p, tree, activity, false, nb_leaves);
	      morpho::tree::propagate_node_to_ancestors(p, tree, activity, false);
	      activity(p) = false;
	      n++;
	      mln_assertion(nb_leaves == 0 || *nb_leaves < mln_max(unsigned));

	      // debug
	      {
		morpho::tree::propagate_node_to_descendants(p, tree, global::debug_img,
							    literal::green);
		morpho::tree::propagate_node_to_ancestors(p, tree, global::debug_img,
							  literal::red);

		global::debug_img(p) = literal::blue;

		morpho::tree::propagate_representative(tree, global::debug_img);

		std::stringstream fn;
		fn.fill('0');
		fn << "debug/log_";
		fn.width(3);
		fn << std::right << cpt;

		std::ofstream out((fn.str() + ".log").c_str());

		out << "# " << cpt << " Retrieve " << p << " with attribute value: "
		    << a(p) << std::endl;

		mln_psite(T) it = p;
		while (!tree.is_root(it))
		{
		  out << "\"" << it << "\" " << a(it) << std::endl;
		  it = tree.parent(it);
		}
		out.close();


		io::ppm::save(global::debug_img, (fn.str() + ".ppm").c_str());


// 		if (fork() == 0)
// 		  {
// 		    execlp("xv", "xv", "debug.ppm", 0);
// 		    exit(0);
// 		  }
		//std::cout << "Press to continue: "
		//	  << std::endl;
		//std::cin.ignore();
		cpt++;
	      }
	    }
	  while (pred(p, n, nb_leaves));

	  if (uses_leaves)
	    delete nb_leaves;
	  return components;
	}

      } // end of namespace mln::morpho::tree::internal


      template <typename T, typename A>
      p_array< mln_psite(A) >
      get_components(const T& tree, const Image<A>& attr_image)
      {
	trace::entering("mln::morpho::tree::get_components");

	const A& a = exact(attr_image);
	mln_precondition(tree.f().domain() == a.domain()); // May be to strong.
	mln_precondition(a.is_valid());

	p_array< mln_psite(A) > components =
	  internal::get_components<T, A>(tree, a, internal::pred_tree_leaves_last<T>(tree), true);

	trace::exiting("mln::morpho::tree::get_components");
	return components;
      }


      template <typename T, typename A>
      p_array< mln_psite(A) >
      get_components(const T& tree, const Image<A>& attr_image, unsigned n)
      {
	trace::entering("mln::morpho::tree::get_components");

	const A& a = exact(attr_image);
	mln_precondition(tree.f().domain() == a.domain()); // May be to strong.
	mln_precondition(a.is_valid());

	p_array< mln_psite(A) > components;

	if (n > 0)
	  components = internal::get_components(tree, a, internal::pred_n_components(n), false);

	trace::exiting("mln::morpho::tree::get_components");
	return components;
      }

      template <typename T, typename A, typename P2B>
      p_array< mln_psite(A) >
      get_components(const T& tree, const Image<A>& attr_image, const Function_v2b<P2B>& pred)
      {
	trace::entering("mln::morpho::tree::get_components");

	const A& a = exact(attr_image);
	const P2B& predicate = exact(pred);
	mln_precondition(tree.f().domain() == a.domain()); // May be to strong.
	mln_precondition(a.is_valid());

	p_array< mln_psite(A) > components =
	  internal::get_components(tree, a, internal::pred_p2b<P2B>(predicate), false);

	trace::exiting("mln::morpho::tree::get_components");
	return components;
      }

# endif /* !MLN_INCLUDE_ONLY */

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif /* !MLN_MORPHO_TREE_COMPONENTS_HH_ */
