// Copyright (C) 2010, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_AFP_REGROUP_HH
# define SCRIBO_AFP_REGROUP_HH

/// \file
///
/// Specific implementation for AFP use case.

#include <mln/geom/ncols.hh>
#include <mln/geom/nrows.hh>
#include <mln/util/couple.hh>

#include <scribo/core/object_groups.hh>
#include <scribo/core/component_set.hh>
#include <scribo/core/macros.hh>

// FIXME: Move the contents of this file in the right namespace and/or
// refactor.

namespace scribo
{

  namespace primitive
  {

    namespace group
    {

      template <typename L>
      object_groups<L>
      regroup_left(const component_set<L>& components,
		   const object_groups<L>& groups,
		   unsigned dmax)
      {
	mln_trace("scribo::primitive::group::regroup_left");

	mln_precondition(groups.is_valid());

	object_groups<L>
	  new_groups(components, static_cast<unsigned>(components.nlabels()) + 1, 0);

	unsigned ngroups = 0;
	for_all_comps(i, components)
	{
	  if (groups[i] == 0)
	    continue;

	  // We MUST set a group id here since the most left group
	  // component won't have any id otherwise.
	  if (new_groups[i] == 0)
	    new_groups[i] = ++ngroups;

	  const mln_site(L) c = components.mass_center(i);

	  int
	    midcol = (components.bbox(i).pmax().col()
		      - components.bbox(i).pmin().col()) / 2;
	  int
	    nleftima = geom::ncols(components),
	    nleft = std::min(static_cast<unsigned>(nleftima), midcol + dmax);

	  // Left
	  {
	    const mln_value(L)
	      *p = &components(c),
	      *pstop = p - nleft - 1;

	    for (; p != pstop; --p)
	    {
	      if (*p != literal::zero  // Not the background
		  && *p != i // Not the current component
//		  && new_groups[*p] != ngroups
		  && groups[*p] != 0)
	      {
		if (new_groups[*p] == 0)
		  new_groups[*p] = ngroups;
		else
		{
		  new_groups[i] = new_groups[*p];
		  --ngroups;
		}

		break;
	      }
	    }
	  }
	}

	return new_groups;
      }

    } // end of namespace scribo::primitive::group

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_AFP_REGROUP_HH
