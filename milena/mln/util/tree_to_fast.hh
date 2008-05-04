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

#ifndef MLN_UTIL_TREE_TO_FAST_HH
# define MLN_UTIL_TREE_TO_FAST_HH

/*!
 * \file   mln/util/tree_to_fast.hh
 *
 * \brief Definition of function which converts a tree into tree_fast.
 *
 */

# include <mln/util/tree.hh>
# include <mln/util/tree_fast.hh>
# include <mln/trace/all.hh>


namespace mln
{

  namespace util
  {


    /*! Convert a tree into an tree_fast.
     *
     * \param[in] input The tree to convert.
     *
     * \return The tree_fast containing tree informations.
     *
     */
    template<typename T>
    tree_fast<T>
    tree_to_fast(tree<T>& input);


# ifndef MLN_INCLUDE_ONLY


    namespace impl
    {

      template<typename T>
      inline
      void
      tree_to_fast_(tree_node<T>* input, tree_fast<T>& tree,
		    unsigned p, unsigned& i)
      {
	typename tree_node<T>::children_t child = input->children ();
	typename tree_node<T>::children_t::iterator it = child.begin ();

	for (; it != child.end (); ++it)
	  {
	    tree.add_child(p, (*it)->elt ());
	    ++i;
	    impl::tree_to_fast_((*it), tree, i, i);
	  }
      }

    }

    /// Facade.

    template<typename T>
    inline
    tree_fast<T>
    tree_to_fast(tree<T>& input)
    {
      trace::entering("util::tree_to_fast");

      unsigned i = 0;
      tree_fast<T> tree (input.root ()->elt ());

      impl::tree_to_fast_(input.root (), tree, 0, i);

      trace::exiting("util::tree_to_fast");
      return tree;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // !MLN_UTIL_TREE_TO_FAST_HH

