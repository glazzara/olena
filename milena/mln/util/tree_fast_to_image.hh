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

#ifndef MLN_UTIL_TREE_FAST_TO_IMAGE_HH
# define MLN_UTIL_TREE_FAST_TO_IMAGE_HH

/*!
 *  \file   mln/util/tree_fast_to_image.hh
 *
 *  \brief Definition of function which transform a tree_fast into an
 *  image.
 *
 */

# include <mln/util/tree_fast.hh>
# include <mln/core/set_p.hh>
# include <list>

namespace mln
{

  namespace util
  {

    template <typename T, typename I>
    void
    tree_fast_to_image (tree_fast<T>& tree, Image<I>& output_);

# ifndef MLN_INCLUDE_ONLY

    template <typename T, typename I>
    void
    tree_fast_to_image (tree_fast<T>& tree, Image<I>& output_)
    {
      I& output = exact(output_);
      std::list<unsigned> q;

      q.push_back (tree.root_);
      while (!(q.empty ()))
	{
	  unsigned current = q.front ();
	  for (unsigned i = 0; i < tree.child_[current].size (); ++i)
	    q.push_back (tree.child_[current][i]);

	  mln_piter(set_p<point2d>) p(tree.data_[current].points);

	  for_all(p)
	    {
	      output(p) = tree.data_[current].value;
	    }
	  q.pop_front ();
	}
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // !MLN_UTIL_TREE_FAST_TO_IMAGE_HH
