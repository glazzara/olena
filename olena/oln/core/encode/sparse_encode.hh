// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
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

#ifndef SPARSE_ENCODE_HH_
# define SPARSE_ENCODE_HH_

# include <oln/core/concept/image.hh>

# include <oln/core/sparse/sparse_image.hh>

# include <vector>

namespace oln
{

  /*!
  ** encode a generic image to a sparse image format
  **
  ** @param input an Image
  **
  ** @return a sparse image
  */
  template <typename I>
  sparse_image<typename I::point, typename I::value>
  sparse_encode(const Image<I>& input)
  {
    sparse_image<typename I::point, typename I::value>	output;
    typename I::piter					p(input.points());
    unsigned						len = 1;
    typename I::coord					old = 1; /*!< old point first dim coordinate */
    typename I::point					rstart;	/*!< range pointstart */
    std::vector<typename I::value>			values;	/*!< range value */

    p.start();
    if (!p.is_valid())
      return output;

    rstart = p;
    //FIXME: is it generall ?
    old = (p.to_point())[0];
    values.push_back(input(p.to_point()));
    p.next();
    while (p.is_valid())
    {
      if ((p.to_point())[0] - 1 == old)
      {
	++len;
	values.push_back(input(p.to_point()));
      }
      else
      {
	output.insert(rstart, len, values);
	rstart = p;
	len = 1;
	values.clear();
	values.push_back(input(p.to_point()));
      }
      old = (p.to_point())[0];
      p.next();
    }
    output.insert(rstart, len, values);
    return output;
  }
}

#endif /* !SPARSE_ENCODE_HH_ */
