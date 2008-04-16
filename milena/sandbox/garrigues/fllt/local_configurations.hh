// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
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


#ifndef MLN_FIXME_FLLT_LOCAL_CONFIGURATIONS_HH
# define MLN_FIXME_FLLT_LOCAL_CONFIGURATIONS_HH

/*! \file local_configurations.hh
 *
 * \brief Stuffs related to local configurations for the FLLT.
 *
 */

#include <mln/core/clock_neighb2d.hh>
#include "upper.hh"
#include "lower.hh"

namespace mln
{
  namespace fllt
  {

    template <typename V>
    struct lower;

    template <typename V>
    struct upper;

    template <typename F>
    struct added_holes;

    template <typename V>
    struct added_holes<upper<V> >
    {
      // For each possible configuration (of the c8-neighbourghood) of the
      // added pixel, stock the number of added holes.
      // ==> region with c8, holes with c4.
      static char ret[256];
    };

    template <typename V>
    char added_holes<upper<V> >::ret[256] =
    {
      0,  0,  0,  0,   0,  0,  0,  0,   0,  1,  1,  1,   0,  0,  0,  0,
      0,  1,  1,  1,   0,  0,  0,  0,   0,  1,  1,  1,   0,  0,  0,  0,
      0,  1,  1,  1,   1,  1,  1,  1,   1,  2,  2,  2,   1,  1,  1,  1,
      0,  1,  1,  1,   0,  0,  0,  0,   0,  1,  1,  1,   0,  0,  0,  0,

      0,  0,  1,  0,   1,  0,  1,  0,   1,  1,  2,  1,   1,  0,  1,  0,
      0,  0,  1,  0,   0, -1,  0, -1,   0,  0,  1,  0,   0, -1,  0, -1,
      0,  0,  1,  0,   1,  0,  1,  0,   1,  1,  2,  1,   1,  0,  1,  0,
      0,  0,  1,  0,   0, -1,  0, -1,   0,  0,  1,  0,   0, -1,  0, -1,

      0,  0,  1,  0,   1,  0,  1,  0,   1,  1,  2,  1,   1,  0,  1,  0,
      1,  1,  2,  1,   1,  0,  1,  0,   1,  1,  2,  1,   1,  0,  1,  0,
      1,  1,  2,  1,   2,  1,  2,  1,   2,  2,  3,  2,   2,  1,  2,  1,
      1,  1,  2,  1,   1,  0,  1,  0,   1,  1,  2,  1,   1,  0,  1,  0,

      0,  0,  1,  0,   1,  0,  1,  0,   1,  1,  2,  1,   1,  0,  1,  0,
      0,  0,  1,  0,   0, -1,  0, -1,   0,  0,  1,  0,   0, -1,  0, -1,
      0,  0,  1,  0,   1,  0,  1,  0,   1,  1,  2,  1,   1,  0,  1,  0,
      0,  0,  1,  0,   0, -1,  0, -1,   0,  0,  1,  0,   0, -1,  0, -1
    };

    template <typename V>
    struct added_holes<lower<V> >
    {
      // For each possible configuration (of the c4-neighbourghood) of the
      // added pixel, stock the number of added holes.
      // ==> region with c4, holes with c8.
      static char ret[256];
    };

    template <typename V>
    char added_holes<lower<V> >::ret[256] =
    {
      0,  0,  0,  0,   0,  1,  0,  0,   0,  0,  0,  0,   0,  1,  0,  0,
      0,  1,  0,  1,   1,  2,  1,  1,   0,  1,  0,  1,   0,  1,  0,  0,
      0,  0,  0,  0,   0,  1,  0,  0,   0,  0,  0,  0,   0,  1,  0,  0,
      0,  1,  0,  1,   1,  2,  1,  1,   0,  1,  0,  1,   0,  1,  0,  0,

      0,  1,  0,  1,   1,  2,  1,  1,   0,  1,  0,  1,   1,  2,  1,  1,
      1,  2,  1,  2,   2,  3,  2,  2,   1,  2,  1,  2,   1,  2,  1,  1,
      0,  1,  0,  1,   1,  2,  1,  1,   0,  1,  0,  1,   1,  2,  1,  1,
      0,  1,  0,  1,   1,  2,  1,  1,   0,  1,  0,  1,   0,  1,  0,  0,

      0,  0,  0,  0,   0,  1,  0,  0,   0,  0,  0,  0,   0,  1,  0,  0,
      0,  1,  0,  1,   1,  2,  1,  1,   0,  1,  0,  1,   0,  1,  0,  0,
      0,  0,  0,  0,   0,  1,  0,  0,   0,  0,  0,  0,   0,  1,  0,  0,
      0,  1,  0,  1,   1,  2,  1,  1,   0,  1,  0,  1,   0,  1,  0,  0,

      0,  0,  0,  0,   1,  1,  1,  0,   0,  0,  0,  0,   1,  1,  1,  0,
      1,  1,  1,  1,   2,  2,  2,  1,   1,  1,  1,  1,   1,  1,  1,  0,
      0,  0,  0,  0,   1,  1,  1,  0,   0,  0,  0,  0,   1,  1,  1,  0,
      0,  0,  0,  0,   1,  1,  1,  0,   0,  0,  0,  0,   0,  0,  0, -1
    };

    template <typename P, typename F>
    char n_added_holes(const p_image2d<P>& set, const point2d& p)
    {
      unsigned char res = 0;
      dpoint2d dp(1,1);
      clock_neighb2d nbh = F::reg_c_nbh(dp);
      mln_fwd_niter_(clock_neighb2d) n(nbh , p);
      for_all(n)
      {
	res = (res << 1);
	if (set.has(n))
	  res = res | 1;
      }
//       std::cout << int(res) << ":" << int(added_holes<F>::ret[res]) << " hole added." << std::endl;
      return added_holes<F>::ret[res];
    }
  } // end of namespace mln::fllt

} // end of namespace mln



#endif // ! MLN_FIXME_FLLT_HH
