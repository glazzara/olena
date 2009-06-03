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

/*! \file tests/image2d_b.cc
 *
 * \brief Tests on mln::image2d_b.
 */

#include <mln/core/image2d_b.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/println.hh>


#include <mln/geom/nrows.hh>
#include <mln/geom/ncols.hh>

#include <mln/data/fill.hh>

#include <mln/debug/println_with_border.hh>


namespace mln
{

  // 1.08
  template <typename I>
  void fill_naive(I& ima, const mln_value(I)& value)
  {
    mln_piter(I) p(ima.domain());
    for_all(p)
      ima(p) = value;
  }

  // 0.08
  template <typename I>
  void fill_memset(I& ima, const mln_value(I)& value)
  {
    mln_precondition(sizeof(mln_value(I)) == 1);
    std::memset((void*)(ima.buffer()),
		*(const int*)(& value), // violent cast
		sizeof(mln_value(I)) * ima.ncells());
  }

  // 0.26
  template <typename I>
  void fill_pixel(I& ima, const mln_value(I)& value)
  {
    int n = 0;
    mln_pixter(I) pix(ima);
    for_all(pix)
      {
	++n;
	pix.val() = value;
      }
    std::cout << "n = "
	      << n
	      << std::endl;
  }

  // 0.10
  template <typename I>
  void fill_ptr(I& ima, const mln_value(I)& value)
  {
    mln_value(I)* ptr = & ima[0];
    std::size_t n = ima.ncells();

    for (std::size_t i = 0; i < n; ++i)
      *ptr++ = value;

    std::cout << "n = "
	      << n
	      << std::endl;
  }

  template <typename I>
  void fill_opt1(I& ima, const mln_value(I)& value)
  {
    mln_pixter(I) pix(ima);
    mln_pixter(I) s(ima);

    pix.start();
    s.start ();

    mln_coord(I) min_row = geom::min_row(exact(ima));
    mln_coord(I) min_col = geom::min_col(exact(ima));
    mln_coord(I) max_row = geom::max_row(exact(ima));
    mln_coord(I) max_col = geom::max_col(exact(ima));

    mln_coord(I) nb_cols = max_col - min_col;
    mln_coord(I) nb_rows = max_row - min_row;

    std::cerr << "min_row : "
	      << min_row
	      << std::endl
	      << "max_row : "
	      << max_row
	      << std::endl
	      << "min_col : "
	      << min_col
	      << std::endl
	      << "max_col : "
	      << max_col
	      << std::endl;

    // fill row
    for (std::size_t i = 0; i < nb_cols; ++i)
      {
	pix.val () = value;
   	pix.next ();
      }

    for (std::size_t i = 1; i < nb_rows; ++i)
      {
	// copy row into current row
	std::memcpy (&(pix.val ()),
		     &(s.val ()),
		     sizeof(mln_value(I)) * nb_cols);

	// Jump to next row
	for (std::size_t j = 0; j < nb_cols; ++j)
	  pix.next ();
      }

    std::memcpy (&(pix.val ()),
		 &(s.val ()),
		 sizeof(mln_value(I)) * nb_cols);
  }

  template <typename I>
  void fill_opt2(I& ima, const mln_value(I)& value)
  {
    mln_value(I)* ptr = & ima[0];

    mln_pixter(I) pix(ima);
    mln_pixter(I) s(ima);
    point2d min = ima.bbox ().pmin ();
    point2d u = min;
    point2d v = min + down;
    std::size_t k = ima.offset (down);
    std::size_t max = ima.ncells() / k;

    for (std::size_t i = 0; i < k; ++i)
      *ptr++ = value;

    ptr = & ima[0];

    for (std::size_t i = 0;
	 i < max - 1;
	 ++i)
      {
	u = u + down;
	std::memcpy (&ima[ima.offset (u - min)],
		     ptr,
		     sizeof (mln_value(I)) * k);
      }
  }

  template <typename I>
  void imaset_(I& ima, const mln_value(I)& value, std::size_t n)
  {
    // FIXME Preconditions
    mln_value(I)* ptr = &ima[0];

    for (std::size_t i = 0; i < n; ++i)
      *ptr++ = value;
  }

  template <typename I>
  void imacpy_(I& dst, I& src, std::size_t n)
  {
    // FIXME Preconditions
    mln_value(I)* u = &dst[0];
    mln_value(I)* v = &src[0];

    for (std::size_t i = 0; i < n; ++i)
      *u++ = *v++;
  }
}



int main()
{
  using namespace mln;
  image2d_b<value::int_u8> ima(3, 3);

  image2d_b<value::int_u8> i1(3, 3);
  image2d_b<value::int_u8> i2(3, 3);

  //  fill_ptr(ima, 5);
  fill_opt2(ima, 5);
  debug::println(ima);

  imaset_(i1, 3, 50);
  debug::println_with_border(i1);
  mln_assertion (i1[42] == 3);
  imacpy_(i2, i1, 50);
  debug::println_with_border(i2);

  for (int i = 0; i < 50; ++i)
    mln_assertion (i1[i] == i2[i]);
}
