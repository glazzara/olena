// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_CONVERT_TO_DPOINT_HH
# define MLN_CONVERT_TO_DPOINT_HH

/*! \file
 *
 * \brief Conversions to mln::Dpoint.
 */

# include <mln/core/concept/point_site.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a point site \p p into a delta-point.
    template <typename P>
    mln_dpoint(P) to_dpoint(const Point_Site<P>& p);


# ifndef MLN_INCLUDE_ONLY

    template <typename P>
    inline
    mln_dpoint(P) to_dpoint(const Point_Site<P>& p_)
    {
      const P& p = exact(p_);
      // FIXME HERE: Add ctors from vec in point_ and dpoint_.
      // FIXME HERE: Then change to "return mln_dpoint(P)(p.to_vec());"
      mln_dpoint(P) dp;
      for (unsigned i = 0; i < P::dim; ++i)
	dp[i] = p[i];
      typedef mln_point(P) P_;
      mln_postcondition(dp == p - P_::origin);
      return dp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_DPOINT_HH
