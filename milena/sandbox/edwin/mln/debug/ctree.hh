// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_DEBUG_CTREE_HH
# define MLN_DEBUG_CTREE_HH

# include <mln/util/ctree/ctree.hh>
# include <mln/core/image/attribute_image.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/math/max.hh>
# include <vector>
# include <iostream>


namespace mln
{

  namespace debug
  {

    /// \brief Print a morphological tree.
    ///
    /// \param[in] tree The tree to be print.
    ///
    template <typename T>
    void
    println(const Tree<T>& tree);

    /// \brief Print an  attribute image.
    ///
    /// \param[in] a The attribute image.
    ///
    template <typename T, typename V>
    void
    println(const attribute_image<T, V>& a);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename T, typename V>
      void
      println(const T& tree, const std::vector<V>& values)
      {
	typedef std::vector< p_array<mln_psite(T)> > hset_t;
	hset_t hset(tree.nodes().nsites());

	// Make site set of each node.
	{
	  mln_piter(T::domain_t) p(tree.domain());
	  for_all(p)
	    hset[tree.node(p).index()].insert(p);
	}

	// Retrieve the height
	unsigned height = 0;
	{
	  attribute_image<T, unsigned> h(tree);
	  data::fill(h, 0);

	  mln_bkd_piter(T::nodes_t) n(tree.nodes());
	  for_all(n)
	    if (!tree.is_a_root(n))
	      h(tree.parent(n)) = math::max(h(tree.parent(n)), h(n) + 1);
	    else
	      height = math::max(height, h(n));
	}
	{
	  std::cout << "Node";
	  for (unsigned i = 0; i < height; i++)
	    std::cout << "  ";
	  std::cout << "Value\tSites" << std::endl;
	}

	mln_fwd_piter(T::nodes_t) n(tree.nodes());
	typename T::node_t old;
	unsigned depth = 0;
	for_all(n)
	{
	  if (tree.is_a_root(n))
	    depth = 0;
	  else
	    {
	      while (old != tree.parent(n))
		{
		  --depth;
		  old = tree.parent(old);
		}
	      ++depth;
	    }

	  for (unsigned i = 0; i < depth; i++)
	    std::cout << "  ";
	  std::cout << n;
	  for (unsigned i = depth; i < height + 1; i++)
	    std::cout << "  ";

	  old = n;
	  std::cout << values[old.index()] << "\t"
		    << hset[old.index()] << std::endl;
	}

      }

    } // end of namespace mln::debug::internal

    template <typename T>
    void
    println(const Tree<T>& tree_)
    {
      const T& tree = exact(tree_);

      mln_precondition(tree.is_valid());
      internal::println(tree, tree.data_hook_()->values_);
    }

    template <typename T, typename V>
    void
    println(const attribute_image<T, V>& a)
    {
      mln_precondition(a.is_valid());
      internal::println(a.tree(), a.hook_data_()->values_);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln

#endif // !MLN_DEBUG_CTREE_HH
