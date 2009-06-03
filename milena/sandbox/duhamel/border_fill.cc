// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/border_fill.cc
 *
 * \brief Tests on mln::border::fill.
 */

#include "border_fill.hh"
#include <mln/core/image1d_b.hh>
#include <mln/core/image2d_b.hh>
#include <mln/core/image3d_b.hh>
#include <mln/value/int_u8.hh>
#include <mln/pw/all.hh>
#include <mln/debug/println_with_border.hh>

using namespace mln;

template <typename I>
void print_3d_with_border (const Fast_Image<I>& input_)
{
  const I& ima = exact(input_);
  mln_precondition(ima.is_valid());

  typedef mln_point(I) P;

  std::size_t len_s = exact(ima).bbox().len(P::dim - 3);
  std::size_t len_r = exact(ima).bbox().len(P::dim - 1);
  std::size_t len_c = exact(ima).bbox().len(P::dim - 2);
  std::size_t border = ima.border ();
  std::size_t real_len_s = len_s + 2 * border;
  std::size_t real_len_r = len_r + 2 * border;
  std::size_t real_len_c = len_c + 2 * border;

  for (std::size_t k = 0; k < real_len_s; ++k)
    {
      for (std::size_t j = 0; j < real_len_c; ++j)
	{
	  for (std::size_t i = 0; i < real_len_r; ++i)
	    std::cout << ima[k * (real_len_r * real_len_c) + j * real_len_r + i]
		      << " ";
	  std::cout << std::endl;
	}
      std::cout << std::endl;
    }
  std::cout << std::endl;
}

int
main (void)
{

  std::cout << "Test 2d size=2x3 in int"
	    << std::endl;
  image2d_b<int> i2(2, 3);
  border::fill (i2, 5);
  debug::println_with_border(i2);

  std::cout << "Test 3d size=2x3x1 in value::int_u8"
	    << std::endl;
  image3d_b<value::int_u8> i4(2, 3, 1, 2);
  border::fill (i4, 7);
  print_3d_with_border(i4);


  std::cout << "Test 3d size=2x3x1 with border=1 in int"
	    << std::endl;
  image3d_b<int> i3(2, 3, 1, 1);
  border::fill (i3, 6);
  print_3d_with_border(i3);



  std::cout << "Test 1d size=5 with border=3 in value::int_u8"
	    << std::endl;
  image1d_b<value::int_u8> i6(5);
  border::fill (i6, 9);
  debug::println_with_border(i6);


  std::cout << std::endl;

  std::cout << "Test 1d size=8 with border=3 in int"
	    << std::endl;
  image1d_b<int> i5(8);
  border::fill (i5, 1);
  debug::println_with_border(i5);


}
