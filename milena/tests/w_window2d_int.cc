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

/*! \file tests/w_window2d_int.cc
 *
 * \brief Tests on mln::w_window2d_int.
 */

#include <mln/core/w_window2d_int.hh>
#include <mln/core/win/rectangle2d.hh>

#include <mln/convert/to_image.hh>
#include <mln/convert/to_w_window.hh>
#include <mln/debug/println.hh>


struct my_f : mln::Function_p2v< my_f >
{
  typedef int result;
  int operator()(const mln::point2d& p) const
  {
    return p.row() + p.col();
  }
};


int main()
{
  using namespace mln;

  int ws[] = { -1, 0, 1,
	       -2, 0, 2,
	       -1, 0, 1 };
  w_window2d_int w_win = make::w_window2d(ws);

  image2d_b<int> ima = convert::to_image(w_win);
  w_window2d_int w_win_2 = convert::to_w_window(ima);
  mln_assertion(w_win_2 == w_win);

  w_window2d_int tmp = make::w_window(win::rectangle2d(3, 5), my_f());
  debug::println(convert::to_image(tmp));
}
