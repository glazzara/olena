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

#ifndef SCRIBO_FILTER_SMALL_OBJECT_GROUPS_HH
# define SCRIBO_FILTER_SMALL_OBJECT_GROUPS_HH

/// \file
///
/// Remove small objects groups.


# include <mln/util/array.hh>

# include <scribo/core/object_groups.hh>

namespace scribo
{

  namespace filter
  {

    using namespace mln;


    /*!  \brief Remove objects within a group with less than \p n
      links.

      \param[in] groups Information about object groups.
      \param[in] n_links The minimum number of links per group.

      \return A function mapping an object id to a bool. It is set to
      true if an object is part of a group with more than \p n_links
      links.
    */
    template <typename L>
    mln::util::array<bool>
    small_object_groups(const object_groups<L>& groups,
			unsigned n_links);


# ifndef MLN_INCLUDE_ONLY


    template <typename L>
    inline
    mln::util::array<bool>
    small_object_groups(const object_groups<L>& parent_link,
			unsigned n_links)
    {
      // Counting the number of objects per group.
      mln::util::array<unsigned> group_size(parent_link.size(), 0);
      for (unsigned i = 1; i < group_size.size(); ++i)
	++group_size[parent_link[i]];

      mln::util::array<bool> f(parent_link.size());
      f(0) = true;
      for (unsigned i = 1; i < f.size(); ++i)
	f(i) = (group_size[parent_link[i]] >= n_links);

      return f;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::filter

} // end of namespace scribo

#endif // ! SCRIBO_FILTER_SMALL_OBJECT_GROUPS_HH

