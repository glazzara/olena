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


/// \file
///
/// Compute a canonized (parenthood) component tree from an image.


#ifndef MLN_MORPHO_TREE_COMPONENT_TREE_HH
# define MLN_MORPHO_TREE_COMPONENT_TREE_HH

# include <mln/tag/tree.hh>
# include <mln/util/ctree/ctree.hh>
# include <mln/morpho/tree/impl/union_find.hh>
# include <mln/morpho/tree/impl/hqueue.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      /// Compute a canonized min-tree.
      ///
      /// \param[in] f The input image.
      /// \param[in] nbh The neighborhood.
      ///
      /// \return The corresponding min-tree structure.
      ///
      template <typename I, typename N>
      inline
      util::ctree::ctree<I>
      min_tree(const Image<I>& f, const Neighborhood<N>& nbh);

      /// Compute a canonized max-tree.
      ///
      /// \param[in] f The input image.
      /// \param[in] nbh The neighborhood.
      ///
      /// \return The corresponding max-tree structure.
      ///
      template <typename I, typename N>
      inline
      util::ctree::ctree<I>
      max_tree(const Image<I>& f, const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename N>
      inline
      util::ctree::ctree<I>
      min_tree(const Image<I>& f_, const Neighborhood<N>& nbh_)
      {
	trace::entering("morpho::tree::min_tree");

	const I& f = exact(f_);
	const N& nbh = exact(nbh_);

	mln_precondition(f.is_valid());
	mln_precondition(nbh.is_valid());

	util::ctree::ctree<I> tree =
	  morpho::tree::impl::generic::union_find(tag::tree::min, f, nbh);

	trace::exiting("morpho::tree::min_tree");
	return tree;
      }

      template <typename I, typename N>
      inline
      util::ctree::ctree<I>
      max_tree(const Image<I>& f_, const Neighborhood<N>& nbh_)
      {
	trace::entering("morpho::tree::max_tree");

	const I& f = exact(f_);
	const N& nbh = exact(nbh_);

	mln_precondition(f.is_valid());
	mln_precondition(nbh.is_valid());

	util::ctree::ctree<I> tree =
	  morpho::tree::impl::generic::union_find(tag::tree::max, f, nbh);

	trace::exiting("morpho::tree::max_tree");
	return tree;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_MORPHO_TREE_COMPONENT_TREE_HH
