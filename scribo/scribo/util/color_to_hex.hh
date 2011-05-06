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

#ifndef SCRIBO_UTIL_COLOR_TO_HEX_HH
# define SCRIBO_UTIL_COLOR_TO_HEX_HH

/// \file
///
/// Convert hexadecimal encoded colors to value::rgb8.


#include <cstdio>
#include <iostream>
#include <string.h>
#include <mln/value/rgb8.hh>

namespace scribo
{

  namespace util
  {
    using namespace mln;

    std::string color_to_hex(const value::rgb8& v);


# ifndef MLN_INCLUDE_ONLY

    std::string color_to_hex(const value::rgb8& v)
    {
      std::string result = "#";

      char buf[3];

      int c = v.red();
      sprintf(buf, "%.2X", c);
      result.append(buf);

      c = v.green();
      sprintf(buf, "%.2X", c);
      result.append(buf);

      c = v.blue();
      sprintf(buf, "%.2X", c);
      result.append(buf);

      return result;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::util

} // end of namespace scribo

#endif // ! SCRIBO_UTIL_COLOR_TO_HEX_HH
