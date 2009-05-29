// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/data/saturate_full.cc
///
/// Tests on mln::data::saturate.

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>


#include <mln/core/routine/duplicate.hh>
#include <mln/data/saturate.hh>
#include <mln/debug/iota.hh>

#include <mln/debug/println.hh>


namespace mln
{

  /*

  template <typename I, typename J>
  void
  chck(I& ref, J& out, mln_value(I) min, mln_value(I) max)
  {
    mln_value(J) min2 = min;
    mln_value(J) max2 = max;

    mln_piter(I) p (ref.domain ());

    {
      data::saturate(ref, min, max, out);

      for_all(p)
	{
	  if (ref(p) <= min)
	    {
	      mln_assertion(out(p) == min2);
	      continue;
	    }
	  if (ref(p) >= max)
	    {
	      mln_assertion(out(p) == max2);
	      continue;
	    }
	  mln_assertion(ref(p) == (mln_value(I)) out(p));
	}
    }

    {
      data::saturate_inplace(ref, min, max);

      for_all(p)
	{
	  mln_assertion(ref(p) == (mln_value(I)) out(p));
	}
    }
  }

  template <typename I, typename J>
  void
  chk1d(unsigned cols,
	int min,
	int max)
  {
    image1d<I> ima (cols);
    image1d<J> out (cols);
    debug::iota (ima);
    chck(ima, out, min, max);
  }

  template <typename I, typename J>
  void
  chk2d(unsigned rows,
	unsigned cols,
	int min,
	int max)
  {
    image2d<I> ima (rows, cols);
    image2d<J> out (rows, cols);
    debug::iota (ima);
    chck(ima, out, min, max);
  }

  template <typename I, typename J>
  void
  chk3d(unsigned slis,
	unsigned rows,
	unsigned cols,
	int min,
	int max)
  {
    image3d<I> ima (slis, rows, cols);
    image3d<J> out (slis, rows, cols);
    debug::iota (ima);
    chck(ima, out, min, max);
  }

  */

}


int main()
{
  using namespace mln;

  /*

  unsigned slis_start = 1;
  unsigned slis_end   = 3;

  unsigned rows_start = 1;
  unsigned rows_end   = 5;

  unsigned cols_start = 2;
  unsigned cols_end = 6;


  std::cerr << "Tests data::saturate" << std::endl;

  (std::cerr << "in 1d ... ").flush ();
  {
    for (unsigned i = cols_start; i < cols_end; ++i)
      for (unsigned j = 1; j < i; ++j)
	for (unsigned k = j + 1; k <= i; ++k)
	  {
 	    chk1d<int, int>(i, j, k);
	    chk1d<unsigned, unsigned>(i, j, k);
	    chk1d<int, unsigned>(i, j, k);
	    chk1d<unsigned, int>(i, j, k);

 	    chk1d<value::int_u8, value::int_u8>(i, j, k);
 	    chk1d<value::int_u16, value::int_u16>(i, j, k);
 	    chk1d<value::int_s8, value::int_s8>(i, j, k);
 	    chk1d<value::int_s16, value::int_s16>(i, j, k);

 	    chk1d<unsigned, value::int_u8>(i, j, k);
 	    chk1d<unsigned, value::int_u16>(i, j, k);
 	    chk1d<int, value::int_s8>(i, j, k);
 	    chk1d<int, value::int_s16>(i, j, k);

 	    chk1d<value::int_u8, unsigned>(i, j, k);
 	    chk1d<value::int_u16, unsigned>(i, j, k);
 	    chk1d<value::int_s8, int>(i, j, k);
 	    chk1d<value::int_s16, int>(i, j, k);
	  }
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 2d ... ").flush ();
  {
    for (unsigned h = rows_start; h < rows_end; ++h)
      for (unsigned i = cols_start; i < cols_end; ++i)
	for (unsigned j = 1; j < i; ++j)
	  for (unsigned k = j + 1; k <= i; ++k)
	    {
	      chk2d<int, int>(h, i, j, k);
	      chk2d<unsigned, unsigned>(h, i, j, k);
	      chk2d<int, unsigned>(h, i, j, k);
	      chk2d<unsigned, int>(h, i, j, k);

	      chk2d<value::int_u8, value::int_u8>(h, i, j, k);
	      chk2d<value::int_u16, value::int_u16>(h, i, j, k);
	      chk2d<value::int_s8, value::int_s8>(h, i, j, k);
	      chk2d<value::int_s16, value::int_s16>(h, i, j, k);

	      chk2d<unsigned, value::int_u8>(h, i, j, k);
	      chk2d<unsigned, value::int_u16>(h, i, j, k);
	      chk2d<int, value::int_s8>(h, i, j, k);
	      chk2d<int, value::int_s16>(h, i, j, k);

	      chk2d<value::int_u8, unsigned>(h, i, j, k);
	      chk2d<value::int_u16, unsigned>(h, i, j, k);
	      chk2d<value::int_s8, int>(h, i, j, k);
	      chk2d<value::int_s16, int>(h, i, j, k);
	  }
  }
  std::cerr << "OK" << std::endl;


  (std::cerr << "in 3d ... ").flush ();
  {
    for (unsigned g = slis_start; g < slis_end; ++g)
      for (unsigned h = rows_start; h < rows_end; ++h)
	for (unsigned i = cols_start; i < cols_end; ++i)
	  for (unsigned j = 1; j < i; ++j)
	    for (unsigned k = j + 1; k <= i; ++k)
	      {
		chk3d<int, int>(g, h, i, j, k);
		chk3d<unsigned, unsigned>(g, h, i, j, k);
		chk3d<int, unsigned>(g, h, i, j, k);
		chk3d<unsigned, int>(g, h, i, j, k);

		chk3d<value::int_u8, value::int_u8>(g, h, i, j, k);
		chk3d<value::int_u16, value::int_u16>(g, h, i, j, k);
		chk3d<value::int_s8, value::int_s8>(g, h, i, j, k);
		chk3d<value::int_s16, value::int_s16>(g, h, i, j, k);

		chk3d<unsigned, value::int_u8>(g, h, i, j, k);
		chk3d<unsigned, value::int_u16>(g, h, i, j, k);
		chk3d<int, value::int_s8>(g, h, i, j, k);
		chk3d<int, value::int_s16>(g, h, i, j, k);

		chk3d<value::int_u8, unsigned>(g, h, i, j, k);
		chk3d<value::int_u16, unsigned>(g, h, i, j, k);
		chk3d<value::int_s8, int>(g, h, i, j, k);
		chk3d<value::int_s16, int>(g, h, i, j, k);
	  }
  }
  std::cerr << "OK" << std::endl;

  */

}
