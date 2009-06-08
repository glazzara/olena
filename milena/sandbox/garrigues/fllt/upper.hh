// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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


#ifndef MLN_FIXME_FLLT_UPPER_HH
# define MLN_FIXME_FLLT_UPPER_HH

/*! \file upper.hh
 *
 * \brief Upper level set for Fast level line transform.
 *
 */

# include <mln/core/alias/neighb2d.hh>
# include <mln/core/clock_neighb2d.hh>
# include <mln/labeling/regional_maxima.hh>
# include <mln/accu/stat/max.hh>

# include "lower.hh"

namespace mln
{
  namespace fllt
  {

    //Fwd declarations.
    template <typename V> struct lower;

    //   UPPER LEVEL SET : region = c8, border = c4
    template <typename V>
    struct upper
    {
      typedef lower<V> opposite;

      static bool
      compare(const V& u, const V& v)
      {
	return u > v;
      }

      template <typename I, typename N, typename L>
      static mln_ch_value(I, L)
      regional_extremum(const Image<I>& input, const Neighborhood<N>& nbh, L& nlabels)
      {
	return labeling::regional_maxima(input, nbh, nlabels);
      }

      static const int inc = -1;
      static const bool parent_is_brighter = false;
      typedef accu::stat::max accu_for_gn;

      static const neighb2d& bdr_nbh() { return c4(); }
      static const neighb2d& reg_nbh() { return c8(); }

      static const clock_neighb2d bdr_c_nbh(dpoint2d& dp) { return cc4(dp); }
      static const clock_neighb2d reg_c_nbh(dpoint2d& dp) { return cc8(dp); }
    };

  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_UPPER_HH
