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

/*! \file tests/level/assign_full.cc
 *
 * \brief Tests on mln::level::assign.
 */


#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>


#include <mln/core/image/sub_image.hh>

#include <mln/core/image/image_if.hh>
#include <mln/fun/p2b/chess.hh>


#include <mln/core/clone.hh>
#include <mln/level/assign.hh>
#include <mln/level/compare.hh>
#include <mln/debug/iota.hh>

#include <mln/debug/println.hh>

struct f_box1d_t : mln::Function_p2b< f_box1d_t >
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

struct f_box2d_t : mln::Function_p2b< f_box2d_t >
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

struct f_box3d_t : mln::Function_p2b< f_box3d_t >
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
  template <typename I, typename J>
  void
  chck(I& lhs, J& rhs)
  {
    level::assign(lhs, rhs);
    mln_piter (I) p (lhs.domain ());
    for_all (p)
      mln_assertion (lhs(p) == (mln_value(I))rhs(p));

  }

  template <typename I, typename J>
  void
  chk1d(unsigned cols)
  {
    image1d<I> lhs (cols);
    image1d<J> rhs (cols);
    debug::iota (rhs);
    chck(lhs, rhs);

    level::fill(lhs, 0);
    box1d b1(literal::origin, point1d(1));
    sub_image<image1d<I>, box1d> sub_lhs (lhs, b1);
    sub_image<image1d<J>, box1d> sub_rhs (rhs, b1);
    chck(sub_lhs, sub_rhs);

    level::fill(lhs, 0);
    f_box1d_t f_b(b1);
    image_if<image1d<I>, f_box1d_t> if_lhs(lhs, f_b);
    image_if<image1d<J>, f_box1d_t> if_rhs(rhs, f_b);
    chck(if_lhs, if_rhs);
  }

  template <typename I, typename J>
  void
  chk2d(unsigned rows, unsigned cols)
  {
    image2d<I> lhs (rows, cols);
    image2d<J> rhs (rows, cols);
    debug::iota (rhs);
    chck(lhs, rhs);

    level::fill(lhs, 0);
    box2d b2(literal::origin, point2d(1, 1));
    sub_image<image2d<I>, box2d> sub_lhs (lhs, b2);
    sub_image<image2d<J>, box2d> sub_rhs (rhs, b2);
    chck(sub_lhs, sub_rhs);

    level::fill(lhs, 0);
    f_box2d_t f_b(b2);
    image_if<image2d<I>, f_box2d_t> if_lhs(lhs, f_b);
    image_if<image2d<J>, f_box2d_t> if_rhs(rhs, f_b);
    chck(if_lhs, if_rhs);
  }

  template <typename I, typename J>
  void
  chk3d(unsigned slis, unsigned rows, unsigned cols)
  {
    image3d<I> lhs (slis, rows, cols);
    image3d<J> rhs (slis, rows, cols);
    debug::iota (rhs);
    chck(lhs, rhs);

    level::fill(lhs, 0);
    box3d b3(literal::origin, point3d(1, 1, 1));
    sub_image<image3d<I>, box3d> sub_lhs (lhs, b3);
    sub_image<image3d<J>, box3d> sub_rhs (rhs, b3);
    chck(sub_lhs, sub_rhs);

    level::fill(lhs, 0);
    f_box3d_t f_b(b3);
    image_if<image3d<I>, f_box3d_t> if_lhs(lhs, f_b);
    image_if<image3d<J>, f_box3d_t> if_rhs(rhs, f_b);
    chck(if_lhs, if_rhs);
  }
}


int main()
{
  using namespace mln;

  unsigned slis_start = 1;
  unsigned slis_end   = 4;

  unsigned rows_start = 1;
  unsigned rows_end   = 16;

  unsigned cols_start = 1;
  unsigned cols_end = 32;


  std::cerr << "Tests level::assign" << std::endl;

  (std::cerr << "in 1d ... ").flush ();
  {
    for (unsigned i = cols_start; i < cols_end; ++i)
      {
	chk1d<int, int>(i);
	chk1d<unsigned, unsigned>(i);
	chk1d<int, unsigned>(i);
	chk1d<unsigned, int>(i);

	chk1d<value::int_u8, value::int_u8>(i);
	chk1d<value::int_u16, value::int_u16>(i);
	chk1d<value::int_s8, value::int_s8>(i);
	chk1d<value::int_s16, value::int_s16>(i);

	chk1d<unsigned, value::int_u8>(i);
	chk1d<unsigned, value::int_u16>(i);
 	chk1d<int, value::int_s8>(i);
  	chk1d<int, value::int_s16>(i);

      }
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 2d ... ").flush ();
  {
    for (unsigned j = rows_start; j < rows_end; ++j)
      for (unsigned i = cols_start; i < cols_end; ++i)
	{
	  chk2d<int, int>(j, i);
	  chk2d<unsigned, unsigned>(j, i);
	  chk2d<int, unsigned>(j, i);
	  chk2d<unsigned, int>(j, i);

	  chk2d<value::int_u8, value::int_u8>(j, i);
	  chk2d<value::int_u16, value::int_u16>(j, i);
	  chk2d<value::int_s8, value::int_s8>(j, i);
	  chk2d<value::int_s16, value::int_s16>(j, i);

	  chk2d<unsigned, value::int_u8>(j, i);
	  chk2d<unsigned, value::int_u16>(j, i);
	  chk2d<int, value::int_s8>(j, i);
	  chk2d<int, value::int_s16>(j, i);
	}
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 3d ... ").flush ();
  {
    for (unsigned k = slis_start; k < slis_end; ++k)
      for (unsigned j = rows_start; j < rows_end; ++j)
	for (unsigned i = cols_start; i < cols_end; ++i)
	  {
	    chk3d<int, int>(k, j, i);
	    chk3d<unsigned, unsigned>(k, j, i);
	    chk3d<int, unsigned>(k, j, i);
	    chk3d<unsigned, int>(k, j, i);

	    chk3d<value::int_u8, value::int_u8>(k, j, i);
	    chk3d<value::int_u16, value::int_u16>(k, j, i);
	    chk3d<value::int_s8, value::int_s8>(k, j, i);
	    chk3d<value::int_s16, value::int_s16>(k, j, i);

	    chk3d<unsigned, value::int_u8>(k, j, i);
	    chk3d<unsigned, value::int_u16>(k, j, i);
	    chk3d<int, value::int_s8>(k, j, i);
	    chk3d<int, value::int_s16>(k, j, i);
	  }
  }
  std::cerr << "OK" << std::endl;
}
