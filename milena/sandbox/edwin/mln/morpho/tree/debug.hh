//Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)

// This file is part of Olena.

// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.

// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef DEBUG_HH
# define DEBUG_HH

# include <mln/core/image/image2d.hh>
# include <mln/morpho/tree/data.hh>

# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>

# include <mln/data/fill.hh>

# include <map>

namespace mln
{

  namespace global
  {
    image2d<value::rgb8> debug_img;
  }


  namespace morpho
  {

    namespace tree
    {

      namespace debug
      {


	/// Count the brothers of each tree node and return the result
	/// as an image.
	///
	/// \param tree The component tree.
	///
	/// \return The bro' count image.
	///
	template <typename T>
	inline
	mln_ch_value(typename T::function, value::int_u8)
	  tree_brother(const T& tree)
	{

	  typedef typename T::function F;

	  typedef mln_ch_value(F, value::int_u8) A;

	  A out;
	  initialize(out, tree.f());

	  mln::data::fill(out, 0);

	  {
	    mln_up_node_piter(T) n(tree);
	    for_all(n)
	      out(tree.parent(n))++;
	  }

	  {
	    mln_dn_node_piter(T) n(tree);
	    for_all(n)
	      out(n) = out(tree.parent(n)) - 1;
	  }
	  return out;
	}

	template <typename T>
	inline
	std::map< mln_value(T::function), unsigned>
	tree_height(const T& tree)
	{
	  std::map< mln_value(T::function), unsigned> m;

	  mln_leaf_piter(T) l(tree);
	  for_all(l)
	    m[tree.f(l)]++;

	  return m;
	}

      }

    }

  }

}





# ifndef MLN_INCLUDE_ONLY

# endif // ! MLN_INCLUDE_ONLY

#endif // ! DEBUG_HH
