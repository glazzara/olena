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

#ifndef EXPORT_LEAVES_HH_
# define EXPORT_LEAVES_HH_

# include <mln/core/concept/image.hh>

# include <mln/morpho/tree/data.hh>
# include "../tree/propagate.hh"


# include <mln/data/fill.hh>

# include <fstream>
# include <sstream>

namespace mln {

  template <typename T, typename A>
  inline
  void
  export_leaves_to_file(const T& tree, const Image<A>& attr_image);

  template <typename T, typename A>
  inline
  mln_ch_value(A, bool)
  export_leaves_to_img(const T& tree, const Image<A>& attr_image);


# ifndef MLN_INCLUDE_ONLY

  template <typename T, typename A>
  inline
  void
  export_leaves_to_file(const T& tree, const Image<A>& attr_image)
  {
    mln_precondition(tree.f().domain() == attr_image.domain());

    const A& a = exact(attr_image);
    mln_leaf_piter(T) leaf(tree);

    for_all(leaf)
    {
      std::stringstream fn;
      for (int i = 0; i < leaf.dim; i++)
	fn << "[" << leaf[i] << "]";
      fn << ".txt";

      std::ofstream out(fn.str().c_str());
      mln_psite(T) n = leaf;
      int i = 0;
      while (!tree.is_root(n))
	{
	  out << i++ << " " << a(n) << std::endl;
	  n = tree.parent(n);
	}
      out << i << " " << a(n) << std::endl;
    }
  }

  template <typename T, typename A>
  inline
  mln_ch_value(A, bool)
  export_leaves_to_img(const T& tree, const Image<A>& attr_image)
  {
    mln_precondition(tree.f().domain() == attr_image.domain());

    typedef mln_ch_value(A, bool) I;
    const A& a = exact(attr_image);

    I output;
    initialize(output, a);
    data::fill(output, false);

    mln_leaf_piter(T) l(tree);
    for_all(l)
      output(l) = true;

    morpho::tree::propagate_representant(tree, output);
    return output;
  }



# endif /* !MLN_INCLUDE_ONLY */

} // end of namespace mln

#endif /* !EXPORT_LEAVES_HH_ */
