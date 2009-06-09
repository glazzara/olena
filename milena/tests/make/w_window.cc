// Copyright (C) 2008 EPITA Research and Development Laboratory
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

/*! \file
 *
 * \brief Tests on mln::make::w_window.
 *
 * \todo Remove std::cout.
 */

#include <mln/core/alias/window2d.hh>
// #include <mln/core/w_window.hh>
#include <mln/core/alias/w_window2d_int.hh>


struct test : mln::Function_v2v<test>
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

  bool vals[] = { 1, 1, 0, 0, 1,
		  1, 1, 1, 1, 0,
		  0, 1, 1, 1, 0,
		  0, 1, 1, 1, 0,
		  0, 1, 0, 0, 0 };
  window2d win;
  convert::from_to(vals, win);

  w_window2d_int w_win = make::w_window(win, test());
  std::cout << w_win << std::endl;


//   mln_assertion( count(ima, make::w_window(ima, c4(), c8()))
// 		 == c4().size() + c8().size() );
}
