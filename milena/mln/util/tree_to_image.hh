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

# include <mln/core/concept/image.hh>
# include <mln/util/tree.hh>
# include <mln/core/p_set.hh>
# include <mln/level/fill.hh>

namespace mln
{

  namespace util
  {

    /*! \brief Convert a tree into an image.
     *
     * \param[in] tree The tree to convert.
     * \param[out] output_ The image containing tree informations.
     *
     */
    template <typename T, typename I>
    void
    tree_to_image (tree<T>& tree, Image<I>& output_);

    /*! Display a tree.
     *
     * \param[in] ima_ The domain of output image.
     * \param[in] tree The tree to display.
     *
     */
    template <typename I, typename J>
    void
    display_tree(const Image<J>& ima_, tree<I>& tree);


    /*! Display an arborescence from \p tree_node.
     *
     * \param[in] ima_ The domain of output image.
     * \param[in] tree_node The root tree_node to display.
     *
     */
    template <typename I, typename J>
    void
    display_branch(const Image<J>& ima_, tree_node<I>* tree_node);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename T, typename I>
      inline
      void
      tree_to_image_rec(tree_node<T>* tree_node, Image<I>& output_)
      {
	trace::entering("util::impl::tree_to_image_rec");

	I& output = exact(output_);

	mln_piter(p_set<point2d>) p(tree_node->elt().points);

	for_all(p)
	  output(p) = tree_node->elt().value;

	typename std::vector< util::tree_node<T>* >::const_iterator it = tree_node->children().begin();

	for (int i = 0;
	     it != tree_node->children().end();
	     ++it, ++i)
	  {
	    if (*it)
	      tree_to_image_rec((*it), output);
	  }
	trace::exiting("util::impl::tree_to_image_rec");
      }

      template <typename T, typename J>
      inline
      void
      display_tree_rec(const Image<J>& ima_, tree_node<T>* tree_node, int level)
      {
	trace::entering("util::impl::display_tree_rec");

	const J& ima = exact(ima_);
	display_set(ima, tree_node->elt().points);
	typename mln::util::tree_node<T>::children_t::iterator it = tree_node->children().begin();
	for (;
	     it != tree_node->children().end(); ++it)
	  display_tree_rec(ima, (*it), level + 1);

	trace::exiting("util::impl::display_tree_rec");
      }


      template <typename T, typename J, typename K>
      inline
      void
      display_branch_rec(const Image<J>& ima_, tree_node<T>* tree_node, Image<K>& output_)
      {
	trace::entering("util::impl::display_branch_rec");

	K& output = exact(output_);
	const J& ima = exact(ima_);

	mln_piter(p_set<point2d>) p(tree_node->elt().points);
	for_all (p)
	  output(p) = true;
	typename mln::util::tree_node<T>::children_t::iterator it = tree_node->children().begin();
	for (;
	     it != tree_node->children().end(); ++it)
	  display_branch_rec(ima, (*it), output);

	trace::exiting("util::impl::display_branch_rec");
      }


      template <typename P, typename J>
      inline
      void
      display_set(const Image<J>& ima_, p_set<P>& s)
      {
	trace::entering("util::impl::display_set");

	const J& ima = exact(ima_);
	image2d<bool> out (ima.bbox ());

	level::fill(out, false);
	mln_piter(p_set<P>) p (s);
	for_all (p)
	  out(p) = true;

	trace::exiting("util::impl::display_set");
      }


    } // end of namespace mln::util::impl



    template <typename T, typename I>
    inline
    void
    tree_to_image (tree<T>& tree, Image<I>& output_)
    {
      trace::entering("util::tree_to_image");

      I& output = exact(output_);
      impl::tree_to_image_rec(tree.root(), output);

      trace::exiting("util::tree_to_image");
    }


    template <typename I, typename J>
    inline
    void
    display_tree(const Image<J>& ima_, tree<I>& tree)
    {
      trace::entering("util::display_tree");

      mln_precondition(tree.root());

      const J& ima = exact(ima_);
      int level = 0;

      impl::display_tree_rec(ima, tree.root(), level);

      trace::exiting("util::display_tree");
    }


    template <typename I, typename J>
    inline
    void
    display_branch(const Image<J>& ima_, tree_node<I>* tree_node)
    {
      trace::entering("util::display_branch");

      mln_assertion(tree_node);

      const J& ima = exact(ima_);

      image2d<bool> output (ima.domain ());
      level::fill(output, false);
      impl::display_branch_rec(ima, tree_node, output);

      trace::exiting("util::display_branch");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // !MLN_UTIL_TREE_TO_IMAGE_HH
