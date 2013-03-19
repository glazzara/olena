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



#ifndef SCRIBO_CONVERT_TO_BASE64_HH
# define SCRIBO_CONVERT_TO_BASE64_HH

/*! \file

  \brief Encode an image into base64 raw data.

  Based on Bob Trower's code.
  http://base64.sourceforge.net/b64.c

  LICENCE: Copyright (c) 2001 Bob Trower, Trantor Standard Systems Inc.

           Permission is hereby granted, free of charge, to any person
           obtaining a copy of this software and associated
           documentation files (the "Software"), to deal in the
           Software without restriction, including without limitation
           the rights to use, copy, modify, merge, publish, distribute,
           sublicense, and/or sell copies of the Software, and to
           permit persons to whom the Software is furnished to do so,
           subject to the following conditions:

           The above copyright notice and this permission notice shall
           be included in all copies or substantial portions of the
           Software.

           THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
           KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
           WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
           PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
           OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
           OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
           OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
           SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

# include <cstdio>
# include <cstdlib>

# include <mln/border/resize.hh>
# include <mln/core/image/image2d.hh>
# include <mln/util/array.hh>


# define B64_DEF_LINE_SIZE   72

namespace scribo
{

  namespace convert
  {

    using namespace mln;


    template <typename I>
    void
    to_base64(const Image<I>& input, mln::util::array<unsigned char>& output);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      /*
      ** Translation Table as described in RFC1113
      */
      static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

      /*
      ** encodeblock
      **
      ** encode 3 8-bit binary bytes as 4 '6-bit' characters
      */
      inline
      void
      encodeblock(unsigned char in[3], unsigned char out[4], int len)
      {
	out[0] = cb64[ in[0] >> 2 ];
	out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
	out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
	out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
      }

    } // end of namespace scribo::convert::internal



    template <typename I>
    void
    to_base64(const Image<I>& input_, mln::util::array<unsigned char>& output)
    {
      mln_trace("scribo::convert::to_base64");

      mln_precondition(exact(input_).is_valid());
      using namespace internal;

      const I& input = exact(input_);

      unsigned char in[3], out[4];
      int i, len, blocksout = 0;

      // FIXME: Take border into account while moving pointer and
      // remove that call.
      border::resize(input, 0);

      const unsigned char
    	*end_ptr = (unsigned char *) (input.buffer() + input.nelements()),
    	*ptr = (unsigned char *)input.buffer();

      while(ptr != end_ptr)
      {
    	len = 0;
    	for(i = 0; i < 3; ++i)
    	{
	  if (ptr != end_ptr)
	  {
	    in[i] = (unsigned char) *ptr++;
	    ++len;
	  }
    	  else
	  {
    	    in[i] = 0;
	    for(++i; i < 3; ++i)
	      in[i] = 0;
	  }
    	}
    	if(len)
    	{
    	  encodeblock(in, out, len);
    	  for(i = 0; i < 4; ++i)
    	    output.append(out[i]);
    	  ++blocksout;
    	}
    	if(blocksout >= (B64_DEF_LINE_SIZE/4) || ptr == end_ptr)
    	{
    	  if(blocksout)
    	  {
    	    output.append('\r');
    	    output.append('\n');
    	  }
    	  blocksout = 0;
    	}
      }

    }

# endif // ! MLN_INCLUDE_ONLY

# undef B64_DEF_LINE_SIZE

  } // end of namespace scribo::convert

} // end of namespace scribo

#endif // ! SCRIBO_CONVERT_TO_BASE64_HH
