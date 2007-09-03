// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_RLE_ENCODE_HH
# define MLN_CORE_RLE_ENCODE_HH

/*! \file mln/core/rle_encode.hh
 *
 * \brief FIXME
 */

# include <mln/core/sparse_image.hh>
# include <vector>

namespace mln
{

  /*!
  ** encode an image class to a rle_image
  **
  ** @param input has to respect the Image concept
  **
  ** @return rle_image
  */
  template <typename I>
  rle_image<mln_point(I), mln_value(I)>
  rle_encode(const Image<I>& input);

# ifndef MLN_INCLUDE_ONLY
  /*!
  ** test if Point p1 and p2 are on the same line
  */
  template <typename P>
  bool
  on_the_same_line(const P& p1, const P& p2)
  {
    unsigned dim = P::dim;
    bool same_line = true;

    for (int n = dim - 1; same_line and n > 0; --n)
      same_line = (p1[n] == p2[n]);
    return same_line;
  }

  template <typename I>
  rle_image<mln_point(I), mln_value(I)>
  rle_encode(const Image<I>& input)
  {
    rle_image<mln_point(I), mln_value(I)> output;
    mln_piter(I) p (exact(input).domain());
    unsigned len = 1;
    /// range point start
    mln_point(I) rstart;
    /// range value
    mln_value(I) rvalue;

    p.start();
    if (!p.is_valid())
      return output;

    rstart = p;
    rvalue = exact(input)(p);
    p.next_();
    while (p.is_valid())
    {
      if (rvalue == exact(input)(p) and
	  on_the_same_line(rstart, mln_point(I)(p)))
	++len;
      else
      {
	output.insert(rstart, len, rvalue);
	len = 1;
	rstart = p;
	rvalue = exact(input)(p);
      }
      p.next_();
    }
    output.insert(rstart, len, rvalue);
    return output;
  }

#endif // ! MLN_INCLUDE_ONLY

}

#endif // ! MLN_CORE_RLE_ENCODE_HH
