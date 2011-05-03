// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_CORE_INTERNAL_SORT_COMP_IDS_HH
# define SCRIBO_CORE_INTERNAL_SORT_COMP_IDS_HH

/// \file
///
/// Functor ordering Components by location, from left to right.


# include <scribo/core/component_set.hh>


namespace scribo
{

  namespace internal
  {

    template <typename L>
    struct sort_comp_ids
    {
      sort_comp_ids(const component_set<L>& comp_set);
      bool operator()(const component_id_t& l, const component_id_t& r) const;

      component_set<L> comps_;
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename L>
    sort_comp_ids<L>::sort_comp_ids(const component_set<L>& comp_set)
      : comps_(comp_set)
    {
    }


    template <typename L>
    bool
    sort_comp_ids<L>::operator()(const component_id_t& l,
				 const component_id_t& r) const
    {
      return comps_(l).bbox().pmin().col() < comps_(r).bbox().pmin().col()
	&& comps_(l).bbox().pmax().col() < comps_(r).bbox().pmax().col();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::internal

} // end of namespace scribo

#endif // ! SCRIBO_CORE_INTERNAL_SORT_COMP_IDS_HH
