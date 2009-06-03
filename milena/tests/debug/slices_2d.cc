// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/debug/slices_2d.cc
///
/// Tests on mln::debug::slices_2d.
///
/// \todo Add extra tests...

#include <mln/debug/slices_2d.hh>
#include <mln/debug/iota.hh>


int main()
{
  using namespace mln;

  {
    point3d p_orig(-1, -2, -3);
    box3d b(p_orig, p_orig + dpoint3d(4, 1, 2));
    image3d<int> vol(b); // 5 x 2 x 3
    debug::iota(vol);

    //   1  2  3 
    //  4  5  6 
    //
    //   7  8  9 
    //  10 11 12 
    //
    //   13 14 15 
    //  16 17 18 
    //
    //   19 20 21 
    //  22 23 24 
    //
    //   25 26 27 
    //  28 29 30 

    image2d<int> ima = debug::slices_2d(vol, 2, 3,
					-1);

    // 1  2  3  7  8  9  
    // 4  5  6  10 11 12 
    // 13 14 15 19 20 21 
    // 16 17 18 22 23 24 
    // 25 26 27 -1 -1 -1 
    // 28 29 30 -1 -1 -1 

    mln_assertion(ima(ima.domain().pmin()) ==  1);
    mln_assertion(ima(ima.domain().pmax()) == -1);

  }


  // Tests with ratio.

  {
    int bg = 0;
    image3d<int> vol(1, 1, 1);
    image2d<int> ima;
    float ratio;

    ratio = 1.f;
    ima = debug::slices_2d(vol, ratio, bg);
    mln_assertion(ima.nrows() == 1 && ima.ncols() == 1);

    ratio = 0.00001f;
    ima = debug::slices_2d(vol, ratio, bg);
    mln_assertion(ima.nrows() == 1 && ima.ncols() == 1);

    ratio = 100000.f;
    ima = debug::slices_2d(vol, ratio, bg);
    mln_assertion(ima.nrows() == 1 && ima.ncols() == 1);
  }

}
