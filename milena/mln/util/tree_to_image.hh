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

#ifndef MLN_UTIL_TREE_TO_IMAGE_HH
# define MLN_UTIL_TREE_TO_IMAGE_HH

/*!
 *  \file   mln/util/tree_to_image.hh
 *
 *  \brief Definition of function which transform a tree into an
 *  image.
 *
 */

# include <mln/util/tree.hh>
# include <mln/core/set_p.hh>

namespace mln
{

  namespace util
  {

    template <typename T, typename I>
    void
    tree_to_image_rec(node<T>* node, Image<I>& output_);

    template <typename T, typename I>
    void
    tree_to_image (tree<T>& tree, Image<I>& output_);

    template <typename P, typename J>
    void
    display_set(const Image<J>& ima_, set_p<P>& s);

    template <typename T, typename J>
    void
    display_tree_rec(const Image<J>& ima_, node<T>* node, int level);

    template <typename I, typename J>
    void
    display_tree(const Image<J>& ima_, tree<I>& tree);

# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename I>
    void
    tree_to_image_rec(node<T>* node, Image<I>& output_)
    {
      I& output = exact(output_);

      mln_piter(set_p<point2d>) p(node->elt().points);

      for_all(p)
	output(p) = node->elt().value;

      typename std::vector< util::node<T>* >::const_iterator it = node->children().begin();

      for (int i = 0;
	   it != node->children().end();
	   ++it, ++i)
	{
	  if (*it)
	    tree_to_image_rec((*it), output);
	}
    }

    template <typename T, typename I>
    void
    tree_to_image (tree<T>& tree, Image<I>& output_)
    {
      I& output = exact(output_);
      tree_to_image_rec(tree.root(), output);
    }


    template <typename P, typename J>
    void
    display_set(const Image<J>& ima_, set_p<P>& s)
    {
      const J& ima = exact(ima_);
      image2d<bool> out (ima.bbox ());

      level::fill(out, false);
      mln_piter(set_p<P>) p (s);
      for_all (p)
	out(p) = true;
      debug::println(out);
    }


    template <typename T, typename J>
    void
    display_tree_rec(const Image<J>& ima_, node<T>* node, int level)
    {
      const J& ima = exact(ima_);
      std::cout << level << std::endl;
      std::cout << std::endl;
      display_set(ima, node->elt().points);
      typename mln::util::node<T>::children_t::iterator it = node->children().begin();
      for (;
	   it != node->children().end(); ++it)
	display_tree_rec(ima, (*it), level + 1);
      std::cout << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;
    }

    template <typename I, typename J>
    void
    display_tree(const Image<J>& ima_, tree<I>& tree)
    {
      const J& ima = exact(ima_);
      int level = 0;

      mln_assertion(tree.root());
      display_tree_rec(ima, tree.root(), level);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // !MLN_UTIL_TREE_TO_IMAGE_HH
