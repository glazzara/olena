// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_UTIL_TREE_TO_FAST_HH
# define MLN_UTIL_TREE_TO_FAST_HH

/*!
 * \file
 *
 * \brief Definition of function which converts a tree into tree_fast.
 *
 */

# include <mln/util/tree.hh>
# include <mln/util/tree_fast.hh>
# include <mln/debug/trace.hh>


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
      mln_trace("util::tree_to_fast");

      unsigned i = 0;
      tree_fast<T> tree (input.root ()->elt ());

      impl::tree_to_fast_(input.root (), tree, 0, i);

      return tree;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_TREE_TO_FAST_HH
