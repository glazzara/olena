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

/*! \file tests/border/mirror_full.cc
 *
 * \brief Tests on mln::border::mirror.
 */

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/border/mirror.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_s8.hh>


namespace mln
{

  template <typename T>
  void chck1d (int cols, int border, T ref[])
  {
    int c = cols + 2 * border;

    image1d<T> ima(cols, border);
    debug::iota(ima);
    border::mirror(ima);

    for (int i = 0; i < c; ++i)
      mln_assertion(ima[i] == ref[i]);
  }

  template <typename T>
  void chck2d (int rows, int cols, int border, T ref[])
  {
    int r = rows + 2 * border;
    int c = cols + 2 * border;

    image2d<T> ima(rows, cols, border);
    debug::iota(ima);
    border::mirror(ima);

    for (int i = 0; i < c * r; ++i)
      mln_assertion(ima[i] == ref[i]);
  }

}


int
main (void)
{
  using namespace mln;


  std::cerr << "Tests border::mirror:" << std::endl;

  {
    std::cerr << "  in 1d :" << std::endl;

    {
      (std::cerr << "    on int_u8 with border = 3 ... ").flush ();

      typedef value::int_u8 T;
      int border = 3;
      int cols = 2;
      T ref[8] = {2, 2, 1, 1, 2, 2, 1, 1};

      chck1d(cols, border, ref);
      std::cerr << "OK" << std::endl;
    }

    {
      (std::cerr << "    on    int with border = 2 ... ").flush ();

      typedef int T;
      int border = 2;
      int cols = 3;
      T ref[7] = {2, 1, 1, 2, 3, 3, 2};

      chck1d(cols, border, ref);
      std::cerr << "OK" << std::endl;
    }

    {
      (std::cerr << "    on int_s8 with border = 1 ... ").flush ();

      typedef value::int_s8 T;
      int border = 1;
      int cols = 2;
      T ref[4] = {1, 1, 2, 2};

      chck1d(cols, border, ref);
      std::cerr << "OK" << std::endl;
    }

    {
      (std::cerr << "    on    int with border = 0 ... ").flush ();
      typedef int T;
      int border = 0;
      int cols = 4;
      T ref[4] = {1, 2, 3, 4};

      chck1d(cols, border, ref);
      std::cerr << "OK" << std::endl;
    }

  } // end of 1d

  {
    std::cerr << "  in 2d :" << std::endl;

    {
      (std::cerr << "    on    int with border = 3 ... ").flush ();
      typedef int T;
      int border = 3;
      int rows = 4;
      int cols = 5;
      T ref[110] =
	{
	   1,  1,  1, 11, 12, 13, 14, 15,  5,  5,  5,
	   1,  1,  1,  6,  7,  8,  9, 10,  5,  5,  5,
	   1,  1,  1,  1,  2,  3,  4,  5,  5,  5,  5,
	   3,  2,  1,  1,  2,  3,  4,  5,  5,  4,  3,
	   8,  7,  6,  6,  7,  8,  9, 10, 10,  9,  8,
	  13, 12, 11, 11, 12, 13, 14, 15, 15, 14, 13,
	  18, 17, 16, 16, 17, 18, 19, 20, 20, 19, 18,
	  16, 16, 16, 16, 17, 18, 19, 20, 20, 20, 20,
	  16, 16, 16, 11, 12, 13, 14, 15, 20, 20, 20,
	  16, 16, 16,  6,  7,  8,  9, 10, 20, 20, 20
	};

      chck2d(rows, cols, border, ref);
      std::cerr << "OK" << std::endl;
    }

    {
      (std::cerr << "    on int_u8 with border = 3 ... ").flush ();
      typedef value::int_u8 T;
      int border = 2;
      int rows = 4;
      int cols = 5;
      T ref[72] =
	{
	   1,  1,  6,  7,  8,  9, 10,  5,  5,
	   1,  1,  1,  2,  3,  4,  5,  5,  5,
	   2,  1,  1,  2,  3,  4,  5,  5,  4,
	   7,  6,  6,  7,  8,  9, 10, 10,  9,
	  12, 11, 11, 12, 13, 14, 15, 15, 14,
	  17, 16, 16, 17, 18, 19, 20, 20, 19,
	  16, 16, 16, 17, 18, 19, 20, 20, 20,
	  16, 16, 11, 12, 13, 14, 15, 20, 20
	};

      chck2d(rows, cols, border, ref);
      std::cerr << "OK" << std::endl;
    }

    {
      (std::cerr << "    on int_s8 with border = 1 ... ").flush ();
      typedef value::int_s8 T;
      int border = 1;
      int rows = 4;
      int cols = 5;
      T ref[49] =
	{
	   1,  1,  2,  3,  4,  5,  5,
	   1,  1,  2,  3,  4,  5,  5,
	   6,  6,  7,  8,  9, 10, 10,
	  11, 11, 12, 13, 14, 15, 15,
	  16, 16, 17, 18, 19, 20, 20,
	  16, 16, 17, 18, 19, 20, 20
	};

      chck2d(rows, cols, border, ref);
      std::cerr << "OK" << std::endl;
    }

    {
      (std::cerr << "    on    int with border = 0 ... ").flush ();
      typedef int T;
      int border = 0;
      int rows = 4;
      int cols = 5;
      T ref[20] =
	{
	   1,  2,  3,  4,  5,
	   6,  7,  8,  9,  10,
	  11, 12, 13, 14, 15,
	  16, 17, 18, 19, 20
	};

      chck2d(rows, cols, border, ref);
      std::cerr << "OK" << std::endl;
    }

  } // end of 2d

}
