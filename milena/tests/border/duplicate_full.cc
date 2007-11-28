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

/*! \file tests/border/duplicate_full.cc
 *
 * \brief Tests on mln::border::duplicate.
 */

#include <mln/core/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/border/duplicate.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_s8.hh>


using namespace mln;

int
main (void)
{
  {
    (std::cerr << "Test border::mirror on int with border = 3 ... ").flush ();

    typedef int T;
    int border = 3;
    int row = 4;
    int col = 5;

    int r = row + 2 * border;
    int c = col + 2 * border;

    image2d<T> ima(row, col, border);
    debug::iota (ima);
    border::duplicate (ima);

    T vs[110] =
      {
	1,  1,  1,  1,  2,  3,  4,  5,  5,  5,  5,
	1,  1,  1,  1,  2,  3,  4,  5,  5,  5,  5,
	1,  1,  1,  1,  2,  3,  4,  5,  5,  5,  5,
	1,  1,  1,  1,  2,  3,  4,  5,  5,  5,  5,
	6,  6,  6,  6,  7,  8,  9,  10, 10, 10, 10,
	11, 11, 11, 11, 12, 13, 14, 15, 15, 15, 15,
	16, 16, 16, 16, 17, 18, 19, 20, 20, 20, 20,
	16, 16, 16, 16, 17, 18, 19, 20, 20, 20, 20,
	16, 16, 16, 16, 17, 18, 19, 20, 20, 20, 20,
	16, 16, 16, 16, 17, 18, 19, 20, 20, 20, 20
      };

    for (int i = 0; i < c * r; ++i)
      mln_assertion(ima[i] == vs[i]);
    std::cerr << "OK" << std::endl;
  }

  {
    (std::cerr << "Test border::mirror on int_u8 with border = 2 ... ").flush ();

    typedef value::int_u8 T;
    int border = 2;
    int row = 4;
    int col = 5;

    int r = row + 2 * border;
    int c = col + 2 * border;

    image2d<T> ima(row, col, border);
    debug::iota (ima);
    border::duplicate (ima);

    T vs[72] =
      {
	 1,  1,  1,  2,  3,  4,  5,  5,  5,
	 1,  1,  1,  2,  3,  4,  5,  5,  5,
	 1,  1,  1,  2,  3,  4,  5,  5,  5,
	 6,  6,  6,  7,  8,  9,  10, 10, 10,
	 11, 11, 11, 12, 13, 14, 15, 15, 15,
	 16, 16, 16, 17, 18, 19, 20, 20, 20,
	 16, 16, 16, 17, 18, 19, 20, 20, 20,
	 16, 16, 16, 17, 18, 19, 20, 20, 20
      };

    for (int i = 0; i < c * r; ++i)
      mln_assertion(ima[i] == vs[i]);
    std::cerr << "OK" << std::endl;
  }

  {
    (std::cerr << "Test border::mirror on int_s8 with border = 1 ... ").flush ();

    typedef value::int_s8 T;
    int border = 1;
    int row = 4;
    int col = 5;

    int r = row + 2 * border;
    int c = col + 2 * border;

    image2d<T> ima(row, col, border);
    debug::iota (ima);
    border::duplicate (ima);

    T vs[49] =
      {
	1,  1,  2,  3,  4,  5,  5,
	1,  1,  2,  3,  4,  5,  5,
	6,  6,  7,  8,  9,  10, 10,
	11, 11, 12, 13, 14, 15, 15,
	16, 16, 17, 18, 19, 20, 20,
	16, 16, 17, 18, 19, 20, 20
      };

    for (int i = 0; i < c * r; ++i)
      mln_assertion(ima[i] == vs[i]);
    std::cerr << "OK" << std::endl;
  }


  {
    (std::cerr << "Test border::mirror on int with border = 0 ... ").flush ();

    typedef int T;
    int border = 0;
    int row = 4;
    int col = 5;

    int r = row + 2 * border;
    int c = col + 2 * border;

    image2d<T> ima(row, col, border);
    debug::iota (ima);
    border::duplicate (ima);

    T vs[20] =
      {
	1,  2,  3,  4,  5,
	6,  7,  8,  9,  10,
	11, 12, 13, 14, 15,
	16, 17, 18, 19, 20
      };

    for (int i = 0; i < c * r; ++i)
      mln_assertion(ima[i] == vs[i]);
    std::cerr << "OK" << std::endl;
  }


}
