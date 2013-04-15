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

#ifndef SCRIBO_UTIL_BOX_IS_INCLUDED_HH
# define SCRIBO_UTIL_BOX_IS_INCLUDED_HH

/// \file
///
/// Check whether a box is included in another one.


#include <mln/core/site_set/box.hh>

namespace scribo
{

  namespace util
  {
    using namespace mln;

    /// \brief Check whether a box is included in another one.
    ///
    /// \return true if \p lhs is included in \p rhs.
    //
    template <typename P>
    bool
    box_is_included(const box<P>& lhs, const box<P>& rhs);


# ifndef MLN_INCLUDE_ONLY

    template <typename P>
    bool
    box_is_included(const box<P>& lhs, const box<P>& rhs)
    {
      mln_trace("scribo::util::box_is_included");

      for (unsigned i = 0; i < P::dim; ++i)
	if (!(lhs.pmin()[i] >= rhs.pmin()[i] && lhs.pmax()[i] <= rhs.pmax()[i]))
	  return false;

      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_BOX_IS_INCLUDED_HH
