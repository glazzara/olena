// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/labeling/colorize.cc
///
/// Test on mln::labeling::colorize.

# include <mln/make/image.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>
# include <mln/labeling/colorize.hh>
# include <mln/data/compare.hh>


int main()
{
  using namespace mln;

  value::int_u8 values[][2] = { { 3, 3 },
				{ 0, 0 } };

  typedef value::rgb8 rgb_t;
  value::rgb8 ref_data[][2] = { { rgb_t(89,92,86), rgb_t(89,92,86) },
				{ rgb_t(0,0,0),	   rgb_t(0,0,0)    } };

  image2d<value::int_u8> ima = make::image(values);
  image2d<rgb_t> ref = make::image(ref_data);

  image2d<value::rgb8> ima_color = labeling::colorize(value::rgb8(), ima);

  mln_assertion(ref == ima_color);
}
