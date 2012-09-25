// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

/// \file
/// \brief Computation of the influence zones of the objects of an
/// image.

// FIXME: Refactor and move into the library.

#include <mln/core/routine/duplicate.hh>

#include <mln/core/site_set/p_queue_fast.hh>


namespace mln
{

  // FIXME: Add Image and Neighborhood concept constraints.
  template <typename I, typename N>
  inline
  mln_concrete(I)
  influence_zones(const I& input, const N& nbh)
  {
    mln_concrete(I) output = duplicate(input);

    p_queue_fast<mln_site(I)> q;

    {
      // Initialization.
      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
	if (input(p) != 0)
	  for_all(n) if (input.has(n))
	    if (input(n) == 0)
	      {
		q.push(p);
		break;
	      }
    }
    {
      // Body.
      mln_site(I) p;
      mln_niter(N) n(nbh, p);
      while (! q.is_empty())
	{
	  p = q.pop_front();
	  mln_invariant(output(p) != 0);
	  for_all(n) if (input.has(n))
	    if (output(n) == 0)
	      {
		output(n) = output(p);
		q.push(n);
	      }
	}
    }

    return output;
  }

} // end of namespace mln
