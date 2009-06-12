// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TEXT_GROUPING_INTERNAL_FIND_ROOT_LINK_ARRAY_HH
# define SCRIBO_TEXT_GROUPING_INTERNAL_FIND_ROOT_LINK_ARRAY_HH

/// \file
///
/// Find root in a parent array arrays.


# include <mln/util/array.hh>


namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      namespace internal
      {

	/// Find root in a parent array arrays.
	unsigned
        find_root(mln::util::array<unsigned>& parent, unsigned x);

# ifndef MLN_INCLUDE_ONLY

	inline
	unsigned
        find_root(mln::util::array<unsigned>& parent, unsigned x)
        {
          if (parent[x] == x)
	    return x;
	  else
	    return parent[x] = find_root(parent, parent[x]);
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::text::grouping::internal

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo


#endif // ! SCRIBO_TEXT_GROUPING_INTERNAL_FIND_ROOT_LINK_ARRAY_HH
