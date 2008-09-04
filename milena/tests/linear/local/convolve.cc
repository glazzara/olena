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

/*! \file tests/linear/local/convolve.cc
 *
 * \brief Tests on mln::linear::local::convolve.
 */

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/core/w_window2d_int.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/core/pixel.hh>

#include <mln/debug/iota.hh>
#include <mln/pw/cst.hh>
#include <mln/border/thickness.hh>
#include <mln/linear/local/convolve.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  const unsigned size = 3;
  border::thickness = size - 1;

  image2d<int_u8> ima(size, size);
  debug::iota(ima);

  point2d p = make::point2d(1,1);
  w_window2d_int w_win = make::w_window(win::rectangle2d(size, size),
					pw::cst(1));

  int res;
  {
    linear::local::convolve(ima, p, w_win, res);
    mln_assertion(res == size*size * (size*size + 1) / 2);
  }

  {
    typedef image2d<int_u8> I;
    linear::local::convolve(pixel<const I>(ima, p), w_win, res);
    linear::local::convolve(pixel<I>(ima, p), w_win, res);
  }
  {
    linear::local::convolve(make::pixel(ima, p), w_win, res);
    mln_assertion(res == size*size * (size*size + 1) / 2);
  }

  res = 0;
}
