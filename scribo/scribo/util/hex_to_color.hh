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

#ifndef SCRIBO_UTIL_HEX_TO_COLOR_HH
# define SCRIBO_UTIL_HEX_TO_COLOR_HH

/// \file
///
/// Convert hexadecimal encoded colors to value::rgb8.

#include <iostream>
#include <string>
#include <vector>
#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>

namespace scribo
{

  namespace util
  {
    using namespace mln;

    // \brief Convert hexadecimal encoded colors to value::rgb8.
    value::rgb8 hex_to_color(const std::string& hex);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      value::int_u8 convert_from_hex(const std::string& hex)
      {
	value::int_u8 value = 0;
	int a = 0;
	int b = hex.length() - 1;

	for (; b >= 0; a++, b--)
	{
	  if (hex[b] >= '0' && hex[b] <= '9')
	  {
	    value += (hex[b] - '0') * (1 << (a * 4));
	  }
	  else
	  {
	    switch (hex[b])
	    {
	      case 'A':
	      case 'a':
		value += 10 * (1 << (a * 4));
		break;
	      case 'B':
	      case 'b':
		value += 11 * (1 << (a * 4));
		break;
	      case 'C':
	      case 'c':
		value += 12 * (1 << (a * 4));
		break;
	      case 'D':
	      case 'd':
		value += 13 * (1 << (a * 4));
		break;
	      case 'E':
	      case 'e':
		value += 14 * (1 << (a * 4));
		break;
	      case 'F':
	      case 'f':
		value += 15 * (1 << (a * 4));
		break;
	      default:
		std::cerr << "Error, invalid character '"
			  << hex[a] << "' in hex number" << std::endl;
		break;
	    }
	  }
	}

	return value;
      }

    } // end of namespace scribo::util::internal



    value::rgb8 hex_to_color(const std::string& hex)
    {
      mln_trace("scribo::util::hex_to_color");

      mln_precondition(!hex.empty());

      std::string red, green, blue;

      if (hex[0] == '#')
      {
	red = hex.substr(1, 2);
	green = hex.substr(3, 2);
	blue = hex.substr(5, 2);
      }
      else
      {
	red = hex.substr(0, 2);
	green = hex.substr(2, 2);
	blue = hex.substr(4, 2);
      }

      value::rgb8 v(internal::convert_from_hex(red),
		    internal::convert_from_hex(green),
		    internal::convert_from_hex(blue));

      return v;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_HEX_TO_COLOR_HH
