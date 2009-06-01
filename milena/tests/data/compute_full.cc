// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file tests/data/compute_full.cc
///
/// Tests on mln::data::compute.

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>

#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/accu/min.hh>
#include <mln/accu/max.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/data/compute.hh>



struct f_box1d_t : mln::Function_v2b< f_box1d_t >
{
  f_box1d_t(const mln::box1d& b)
    : b_(b)
  {
  }
  mln::box1d b_;
  bool operator()(const mln::point1d& p) const
  {
    return b_.has(p);
  }
};

struct f_box2d_t : mln::Function_v2b< f_box2d_t >
{
  f_box2d_t(const mln::box2d& b)
    : b_(b)
  {
  }
  mln::box2d b_;
  bool operator()(const mln::point2d& p) const
  {
    return b_.has(p);
  }
};

struct f_box3d_t : mln::Function_v2b< f_box3d_t >
{
  f_box3d_t(const mln::box3d& b)
    : b_(b)
  {
  }
  mln::box3d b_;
  bool operator()(const mln::point3d& p) const
  {
    return b_.has(p);
  }
};


namespace mln
{
  template <typename I>
  void
  chk1d(unsigned cols)
  {
    box1d b1(literal::origin, point1d(1));

    image1d<I> ima (cols);
    debug::iota (ima);
    I real_min = 1;
    I real_min2 = 1;
    I real_max = (I)(cols % (unsigned) mln_max(I));
    I real_max2 = 2;
    if (cols >= (unsigned)mln_max(I))
      {
	real_min = 0;
	real_max = mln_max(I) - 1;
      }


    {
      accu::min<I> acu_min;
      accu::max<I> acu_max;

      I min = data::compute(acu_min, ima);
      I max = data::compute(acu_max, ima);
      mln_assertion(min == real_min);
      mln_assertion(max == real_max);
    }

    {
      sub_image<image1d<I>, box1d> sub_ima (ima, b1);

      accu::min<I> acu_min;
      accu::max<I> acu_max;

      I min = data::compute(acu_min, sub_ima);
      I max = data::compute(acu_max, sub_ima);
      mln_assertion(min == real_min2);
      mln_assertion(max == real_max2);
    }

    {
      f_box1d_t f_b(b1);
      image_if<image1d<I>, f_box1d_t> if_ima(ima, f_b);

      accu::min<I> acu_min;
      accu::max<I> acu_max;

      I min = data::compute(acu_min, if_ima);
      I max = data::compute(acu_max, if_ima);
      mln_assertion(min == real_min2);
      mln_assertion(max == real_max2);
    }
  }

  template <typename I>
  void
  chk2d(unsigned rows, unsigned cols)
  {
    box2d b2(literal::origin, point2d(1, 1));

    image2d<I> ima (rows, cols);
    debug::iota (ima);
    I real_min = 1;
    I real_min2 = 1;
    I real_max;
    if (rows * cols >= (unsigned)mln_max(I))
      {
	real_min = 0;
	real_max = mln_max(I) - 1;
      }
    else
      {
	real_max = (I)(rows * cols);
	if ((cols == (unsigned)mln_max(I) - 2) ||
	    (cols == (unsigned)mln_max(I) - 1))
	  real_min2 = 0;
      }

    {
      accu::min<I> acu_min;
      accu::max<I> acu_max;

      I min = data::compute(acu_min, ima);
      I max = data::compute(acu_max, ima);

      mln_assertion(min == real_min);
      mln_assertion(max == real_max);
    }

  }

  template <typename I>
  void
  chk3d(unsigned slis, unsigned rows, unsigned cols)
  {
    box3d b3(literal::origin, point3d(1, 1, 1));

    image3d<I> ima (slis, rows, cols);
    debug::iota (ima);
    I real_min = 1;
    I real_max;
    if (slis * rows * cols >= (unsigned)mln_max(I))
      {
	real_min = 0;
	real_max = mln_max(I) - 1;
      }
    else
      real_max = (I)(slis * rows * cols);
    {
      accu::min<I> acu_min;
      accu::max<I> acu_max;

      I min = data::compute(acu_min, ima);
      I max = data::compute(acu_max, ima);

      mln_assertion(min == real_min);
      mln_assertion(max == real_max);
    }

  }
}


int main()
{
  using namespace mln;

  unsigned slis_start = 2;
  unsigned slis_end   = 3;

  unsigned rows_start = 2;
  unsigned rows_end   = 16;

  unsigned cols_start = 2;
  unsigned cols_end = 256;


  std::cerr << "Tests data::compute:" << std::endl;

  (std::cerr << "in 1d ... ").flush ();
  {
    for (unsigned i = cols_start; i < cols_end; ++i)
      {
  	chk1d<int>(i);
 	chk1d<unsigned>(i);
	chk1d<value::int_u8>(i);
	chk1d<value::int_u16>(i);
  	chk1d<value::int_s8>(i);
	chk1d<value::int_s16>(i);
      }
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 2d ... ").flush ();
  {
    for (unsigned j = rows_start; j < rows_end; ++j)
      for (unsigned i = cols_start; i < cols_end; ++i)
	{
	  chk2d<int>(j, i);
	  chk2d<unsigned>(j, i);
	  chk2d<value::int_u8>(j, i);
	  chk2d<value::int_u16>(j, i);
	  chk2d<value::int_s8>(j, i);
	  chk2d<value::int_s16>(j, i);
	}
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 3d ... ").flush ();
  {
    for (unsigned k = slis_start; k < slis_end; ++k)
      for (unsigned j = rows_start; j < rows_end; ++j)
	for (unsigned i = cols_start; i < cols_end; ++i)
	  {
	    chk3d<int>(k, j, i);
	    chk3d<unsigned>(k, j, i);
	    chk3d<value::int_u8>(k, j, i);
	    chk3d<value::int_u16>(k, j, i);
	    chk3d<value::int_s8>(k, j, i);
	  }
  }
  std::cerr << "OK" << std::endl;
}
