//Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)

// This file is part of Olena.

// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.

// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

// From theo's sandbox ~theo/color/filter_meandist_rgb_pixels.cc


#ifndef COLOR_DISTANCE_HH
# define COLOR_DISTANCE_HH

#include <mln/value/rgb8.hh>
#include <mln/math/diff_abs.hh>

namespace mln
{

 value::int_u8 dist_mean(const value::rgb8& c1, const value::rgb8& c2)
  {
    unsigned d = 0;
    d += (math::diff_abs(c1.red(), c2.red()) + 2) / 3;
    d += (math::diff_abs(c1.green(), c2.green()) + 2) / 3;
    d += (math::diff_abs(c1.blue(), c2.blue()) + 2) / 3;
    if (d > 255)
      d = 255;
    return d;
  }

  value::int_u8 dist_sat(const value::rgb8& c1, const value::rgb8& c2)
  {
    unsigned d = 0;
    d += math::diff_abs(c1.red(), c2.red());
    d += math::diff_abs(c1.green(), c2.green());
    d += math::diff_abs(c1.blue(), c2.blue());
    if (d > 255)
      d = 255;
    return d;
  }

  value::int_u8 dist_max(const value::rgb8& c1, const value::rgb8& c2)
  {
    unsigned d = 0, d_;
    d_ = math::diff_abs(c1.red(), c2.red());
    if (d_ > d) d = d_;
    d_ = math::diff_abs(c1.green(), c2.green());
    if (d_ > d) d = d_;
    d_ = math::diff_abs(c1.blue(), c2.blue());
    if (d_ > d) d = d_;
    return d;
  }



# ifndef MLN_INCLUDE_ONLY

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln.

#endif // ! COLOR_DISTANCE_HH
