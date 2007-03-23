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

#ifndef OLN_CORE_ENCODE_RLE_ENCODE_HH
# define OLN_CORE_ENCODE_RLE_ENCODE_HH

# include <oln/core/concept/image.hh>

# include <oln/core/rle/rle_image.hh>


namespace oln
{

  /*!
  ** encode an image class to a rle_image
  **
  ** @param input has to respect the Image concept
  **
  ** @return rle_image
  */
  template <typename I>
  rle_image<typename Image<I>::point, typename I::value>
  rle_encode(const Image<I>& input)
  {
    rle_image<typename I::point, typename I::value> output;
    typename I::piter p (input.points());
    unsigned len = 1;
    /// range point start
    typename I::point rstart;
    /// range value
    typename I::value rvalue;

    p.start();
    if (!p.is_valid())
      return output;

    rstart = p;
    rvalue = input(p);
    p.next();
    while (p.is_valid())
    {
      if (rvalue == input(p))
	++len;
      else
      {
	output.insert(rstart, len, rvalue);
	len = 1;
	rstart = p;
	rvalue = input(p);
      }
      p.next();
    }
    output.insert(rstart, len, rvalue);
    return output;
  }
} // end of namespace oln

#endif /* !OLN_CORE_ENCODE_RLE_ENCODE_HH */
