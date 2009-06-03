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

#ifndef MLN_SIMPLE_POINT_HH
# define MLN_SIMPLE_POINT_HH

/*! \file simple_point.hh
 *
 * \brief is_simple_point tell if a point is simple or not (Cf
 * bertrand.07.chap).
 *
 */

#include <mln/core/alias/point2d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

namespace mln
{

/*! Tell if a point is simple or not. A point of an object is simple
 *  if in its c8 neiborhood, there is exactly one connected component of the
 *  object, and only one connected component of the background
 *  Examples : ( | == object, - = background)
 *
 *           - - |
 *           | P | Here p is simple in the c4 and c8 case.
 *           | | |
 *
 *           - | -
 *           | P | Here p is never simple.
 *           | | |
 *
 */

  bool is_simple_point(const image2d<bool>& ima, const neighb2d& nbh, const point2d& p);

  unsigned nb_connexity2d(const image2d<bool>& ima, const neighb2d& nbh, const point2d& p);

  bool is_curve_extremum(const image2d<bool>& ima, unsigned nbh, const point2d& p);

# ifndef MLN_INCLUDE_ONLY

    static const unsigned char nb_connexity_c8[256] =
  {
    0,  1,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   2,  2,  1,  1,
    1,  2,  1,  1,   1,  2,  1,  1,   2,  2,  1,  1,   2,  2,  1,  1,
    1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
    2,  3,  2,  2,   2,  3,  2,  2,   2,  2,  1,  1,   2,  2,  1,  1,

    1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
    1,  2,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   1,  1,  1,  1,
    1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
    1,  2,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   1,  1,  1,  1,

    1,  2,  2,  2,   2,  3,  2,  2,   2,  2,  2,  2,   3,  3,  2,  2,
    1,  2,  1,  1,   1,  2,  1,  1,   2,  2,  1,  1,   2,  2,  1,  1,
    2,  3,  3,  3,   3,  4,  3,  3,   2,  2,  2,  2,   3,  3,  2,  2,
    2,  3,  2,  2,   2,  3,  2,  2,   2,  2,  1,  1,   2,  2,  1,  1,

    1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
    1,  2,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   1,  1,  1,  1,
    1,  2,  2,  2,   2,  3,  2,  2,   1,  1,  1,  1,   2,  2,  1,  1,
    1,  2,  1,  1,   1,  2,  1,  1,   1,  1,  1,  1,   1,  1,  1,  1
  };

  static const unsigned char nb_connexity_c4[256] =
  {
    0,  0,  1,  1,   0,  0,  1,  1,   1,  1,  2,  1,   1,  1,  2,  1,
    1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,
    0,  0,  1,  1,   0,  0,  1,  1,   1,  1,  2,  1,   1,  1,  2,  1,
    1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,

    1,  1,  2,  2,   1,  1,  2,  2,   2,  2,  3,  2,   2,  2,  3,  2,
    2,  2,  3,  3,   2,  2,  2,  2,   3,  3,  4,  3,   3,  3,  3,  2,
    1,  1,  2,  2,   1,  1,  2,  2,   1,  1,  2,  1,   1,  1,  2,  1,
    2,  2,  3,  3,   2,  2,  2,  2,   2,  2,  3,  2,   2,  2,  2,  1,

    0,  0,  1,  1,   0,  0,  1,  1,   1,  1,  2,  1,   1,  1,  2,  1,
    1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,
    0,  0,  1,  1,   0,  0,  1,  1,   1,  1,  2,  1,   1,  1,  2,  1,
    1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,

    1,  1,  2,  2,   1,  1,  2,  2,   2,  2,  3,  2,   2,  2,  3,  2,
    1,  1,  2,  2,   1,  1,  1,  1,   2,  2,  3,  2,   2,  2,  2,  1,
    1,  1,  2,  2,   1,  1,  2,  2,   1,  1,  2,  1,   1,  1,  2,  1,
    1,  1,  2,  2,   1,  1,  1,  1,   1,  1,  2,  1,   1,  1,  1,  1
  };

  neighb2d int_to_neighb(unsigned i)
  {
    if (i == 8)
      return c8();
    if (i == 4)
      return c4();
    mln_assertion(0);
    return c4();
  }

  unsigned complement_neighb(unsigned i)
  {
    if (i == 8)
      return 4;
    if (i == 4)
      return 8;
    mln_assertion(0);
    return 0;
  }

  unsigned nb_connexity2d(const image2d<bool>& ima, unsigned nbh, const point2d& p, bool object)
  {
    unsigned res = 0;

    mln_bkd_niter_(neighb2d) n(c8() , p);
    for_all(n)
    {
      res = (res << 1);
      if (ima.domain().has(n) && ima(n) == object)
	res = res | 1;
    }

    if (nbh == 8)
      return nb_connexity_c8[res];
    else
    {
      mln_assertion(nbh == 4);
      return nb_connexity_c4[res];
    }
  }

  bool is_curve_extremum(const image2d<bool>& ima, unsigned nbh_i, const point2d& p_, unsigned deep)
  {
//     return false;
    unsigned cpt = 0;
    mln_site_(image2d<bool>) next = p_;
    mln_site_(image2d<bool>) p = next;
    mln_niter_(neighb2d) n(int_to_neighb(nbh_i) , p);

    p = next;
    for_all(n)
    {
      if (ima.domain().has(n) && ima(n) == true)
      {
	next = n;
	cpt++;
      }
    }
    if (cpt != 1)
      return false;

    for (unsigned i = 0; i < deep - 1; i++)
    {
      cpt = 0;
      p = next;
      for_all(n)
      {
	if (ima.domain().has(n) && ima(n) == true)
	{
	  next = n;
	  cpt++;
	}
      }
      if (cpt != 2)
	return false;
    }

    return true;
  }

  bool is_simple_point2d(const image2d<bool>& ima, unsigned nbh, const point2d& p)
  {
    mln_assertion(nbh == 4 || nbh == 8);

    return (nb_connexity2d(ima, nbh, p, true) == 1) &&
      (nb_connexity2d(ima, complement_neighb(nbh), p, false) == 1);
  }

# endif // MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_SIMPLE_POINT_HH
