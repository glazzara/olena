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

#ifndef MLN_CORE_SPARSE_ENCODE_HH
# define MLN_CORE_SPARSE_ENCODE_HH

/*! \file mln/core/sparse_encode.hh
 *
 * \brief FIXME
 */

# include <mln/core/sparse_image.hh>
# include <vector>

namespace mln
{

  /*!
  ** encode a generic image to a sparse image format
  **
  ** @param input an Image
  **
  ** @return a sparse image
  */
  template <typename I>
  sparse_image<mln_point(I), mln_value(I)>
  sparse_encode(const Image<I>& input);

# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  sparse_image<mln_point(I), mln_value(I)>
  sparse_encode(const Image<I>& input)
  {
    sparse_image<mln_point(I), mln_value(I)> output;
    mln_piter(I) p(exact(input).domain());
    unsigned len = 1;
    // old point first dim coordinate
    typename I::coord old = 1;
    // range pointstart
    mln_point(I) rstart;
    // range value
    std::vector<mln_value(I)> values;

    p.start();
    if (not p.is_valid())
      return output;

    rstart = p;

    old = p[0];
    values.push_back(exact(input)(p));
    p.next_();
    while (p.is_valid())
    {
      if (p[0] - 1 == old)
      {
	++len;
	values.push_back(exact(input)(p));
      }
      else
      {
	output.insert(rstart, len, values);
	rstart = p;
	len = 1;
	values.clear();
	values.push_back(exact(input)(p));
      }
      old = p[0];
      p.next_();
    }
    output.insert(rstart, len, values);
    return output;
  }

#endif // ! MLN_INCLUDE_ONLY

}

#endif // ! MLN_CORE_SPARSE_ENCODE_HH
