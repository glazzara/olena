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

/// \file tests/data/memset_.cc
///
/// Tests on mln::data::memset_.

#include <mln/core/image/image2d.hh>
#include <mln/geom/ncols.hh>
#include <mln/data/fill.hh>
#include <mln/data/memset_.hh>
#include <mln/opt/at.hh>


int main()
{
  using namespace mln;

  image2d<int> ima(3, 3);
  data::fill(ima, 0);
  int X = 9;
  data::memset_(ima, point2d(0,0),
		X,
		geom::ncols(ima) + 2 * ima.border() + 1);
  //                                                  ^
  //                                                  |
  mln_assertion(opt::at(ima, 1,0) == X); // <---------+
  mln_assertion(opt::at(ima, 1,1) != X);
}
