// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file tests/core/alias/w_window1d_int.cc
/// Tests on mln::w_window1d_int.

#include <mln/core/alias/w_window1d_int.hh>
#include <mln/win/segment1d.hh>
#include <mln/convert/to.hh>

#include <mln/convert/to.hh>
#include <mln/convert/to_fun.hh>
#include <mln/convert/to_image.hh>

#include <mln/estim/sum.hh>


int f(mln::point1d p)
{
  return p.ind ();
}

#include <mln/convert/to.hh>
int main()
{
  using namespace mln;

  {
    int ws[] = { -1, 0, 1 };
    w_window1d_int w_win = make::w_window1d(ws);

    image1d<int> ima = convert::to_image(w_win);

    w_window1d_int w_win_2 = convert::to<w_window1d_int>(ima);
    mln_assertion(w_win_2 == w_win);
  }

  {
    w_window1d_int w_win = make::w_window(win::segment1d(3),
					  convert::to_fun(f));
    //  -1  0 +1
    image1d<int> ima = convert::to_image(w_win);
    mln_assertion(estim::sum(ima) == 0);
  }

}
