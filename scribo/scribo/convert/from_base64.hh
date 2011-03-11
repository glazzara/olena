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



#ifndef SCRIBO_CONVERT_FROM_BASE64_HH
# define SCRIBO_CONVERT_FROM_BASE64_HH

/*! \file

  \brief Decode base64 raw data and convert it to an image.

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

#include <cstdio>
#include <cstdlib>

# ifdef HAVE_QT
#  include <QString>
# endif // ! HAVE_QT

# include <mln/border/resize.hh>
# include <mln/core/image/image2d.hh>
# include <mln/util/array.hh>



namespace scribo
{

  namespace convert
  {

    using namespace mln;

    template <typename I>
    void from_base64(const util::array<unsigned char>& data64,
		     Image<I>& output);


# ifdef HAVE_QT

    template <typename I>
    void from_base64(const QString& data64, Image<I>& output_);

# endif // ! HAVE_QT


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      /*
      ** Translation Table to decode
      */
      static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

      /*
      ** decodeblock
      **
      ** decode 4 '6-bit' characters into 3 8-bit binary bytes
      */
      inline
      void
      decodeblock(unsigned char in[4], unsigned char out[3])
      {
	out[ 0 ] = (unsigned char) (in[0] << 2 | in[1] >> 4);
	out[ 1 ] = (unsigned char) (in[1] << 4 | in[2] >> 2);
	out[ 2 ] = (unsigned char) (((in[2] << 6) & 0xc0) | in[3]);
      }


      template <typename V, typename I>
      void
      from_base64_(const V& data64, const unsigned length, Image<I>& output_)
      {
	trace::entering("scribo::convert::from_base64_");

	mln_precondition(exact(output_).is_valid());
	using namespace internal;

	I& output = exact(output_);

	unsigned char in[4], out[3], v;
	int i, len;

	border::resize(output, 0); // Make sure there is no border!

	unsigned char *ptr = (unsigned char *)output.buffer();
	unsigned char *end_ptr = (unsigned char *)(output.buffer() + output.nelements());

	for (unsigned idx = 0; idx < length;)
	{
	  for(len = 0, i = 0; i < 4 && idx < length; i++)
	  {
	    v = 0;
	    while(idx < length && v == 0)
	    {
	      v = (unsigned char) data64[idx++];
	      v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
	      if(v)
	      {
		v = (unsigned char) ((v == '$') ? 0 : v - 61);
	      }
	    }
	    if(idx < length)
	    {
	      len++;
	      if(v)
	      {
		in[ i ] = (unsigned char) (v - 1);
	      }
	    }
	    else
	    {
	      in[i] = 0;
	    }
	  }
	  if(len)
	  {
	    decodeblock(in, out);
	    for(i = 0; i < len - 1 && ptr != end_ptr; i++)
	    {
	      *ptr++ = out[i];
	    }
	  }
	}

	trace::exiting("scribo::convert::to_base64_");
      }


    } // end of namespace scribo::convert::internal


    template <typename I>
    void
    from_base64(const util::array<unsigned char>& data64, Image<I>& output_)
    {
      trace::entering("scribo::convert::from_base64");

      internal::from_base64_(data64, data64.nelements(), output_);

      trace::exiting("scribo::convert::to_base64");
    }


#  ifdef HAVE_QT

    template <typename I>
    void
    from_base64(const QString& data64, Image<I>& output_)
    {
      trace::entering("scribo::convert::from_base64");

      QByteArray data64_ = data64.toAscii();
      internal::from_base64_(data64_.constData(), data64_.size(), output_);

      trace::exiting("scribo::convert::to_base64");
    }

#  endif // ! HAVE_QT


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::convert

} // end of namespace scribo

#endif // ! SCRIBO_CONVERT_FROM_BASE64_HH
