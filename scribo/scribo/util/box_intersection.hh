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

#ifndef SCRIBO_UTIL_BOX_INTERSECTION_HH
# define SCRIBO_UTIL_BOX_INTERSECTION_HH

/// \file
///
/// Return the box corresponding to the intersection of two boxes.


#include <mln/core/site_set/box.hh>
#include <mln/math/min.hh>
#include <mln/math/max.hh>

namespace scribo
{

  namespace util
  {
    using namespace mln;

    /// \brief Return the box corresponding to the intersection of two
    /// boxes.
    ///
    /// \return An invalid box if there is no intersection. The box
    /// corresponding to the intersection, otherwise.
    template <typename P>
    box<P>
    box_intersection(const box<P>& lhs, const box<P>& rhs);


# ifndef MLN_INCLUDE_ONLY

    template <typename P>
    box<P>
    box_intersection(const box<P>& lhs, const box<P>& rhs)
    {
      trace::entering("scribo::util::box_intersection");

      P pmin, pmax;
      for (unsigned dim = 0; dim < P::dim; ++dim)
      {
	pmin[dim] = math::max(lhs.pmin()[dim], rhs.pmin()[dim]);
	pmax[dim] = math::min(lhs.pmax()[dim], rhs.pmax()[dim]);

	if (pmin[dim] > pmax[dim]) // No intersection.
	  return box<P>();
      }

      box<P> output(pmin, pmax);

      trace::exiting("scribo::util::box_intersection");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_BOX_INTERSECTION_HH
