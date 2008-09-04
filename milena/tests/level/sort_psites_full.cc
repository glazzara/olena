// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file tests/level/sort_points_full.cc
 *
 * \brief Tests on mln::level::sort_points.
 */

#include <mln/core/image1d.hh>
#include <mln/core/image2d.hh>
#include <mln/core/image3d.hh>
#include <mln/core/sub_image.hh>

#include <mln/core/image_if.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/literal/origin.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/debug/iota.hh>

#include <mln/level/saturate.hh>
#include <mln/level/paste.hh>
#include <mln/level/sort_psites.hh>



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
  chck(Image<I>& input_, p_array<J>& array_inc_ref, p_array<J>& array_dec_ref)
  {
    I& input = exact(input_);

    p_array<J> array_inc = level::sort_psites_increasing(input);
    p_array<J> array_dec = level::sort_psites_decreasing(input);

    mln_assertion(array_inc == array_inc_ref);
    mln_assertion(array_dec == array_dec_ref);
  }

  template <typename I>
  void
  chk1d(int cols)
  {
    box1d b1(literal::origin, point1d(1));
    f_box1d_t f_b1(b1);

    {
      image1d<I> input (cols);
      debug::iota(input);

      p_array<point1d> array_inc_ref;
      p_array<point1d> array_dec_ref;

      for (int i = 0; i < cols; ++i)
	array_inc_ref.append(point1d(i));

      for (int i = cols - 1; i >= 0; --i)
	array_dec_ref.append(point1d(i));

      chck(input, array_inc_ref, array_dec_ref);
    }

    p_array<point1d> array_inc_ref;
    p_array<point1d> array_dec_ref;

    for (int i = 0; i < 2; ++i)
      array_inc_ref.append(point1d(i));

    for (int i = 1; i >= 0; --i)
      array_dec_ref.append(point1d(i));

    {
      image1d<I> in (cols);
      sub_image<image1d<I>, box1d> input (in, b1);
      debug::iota(input);
      chck(input, array_inc_ref, array_dec_ref);
    }

    {
      image1d<I> in (cols);
      image_if<image1d<I>, f_box1d_t> input(in, f_b1);
      debug::iota(input);
      chck(input, array_inc_ref, array_dec_ref);
    }
  }

  template <typename I>
  void
  chk2d(int rows, int cols)
  {
    box2d b2(literal::origin, point2d(1, 1));
    f_box2d_t f_b2(b2);

    {
      image2d<I> input (rows, cols);
      debug::iota(input);

      p_array<point2d> array_inc_ref;
      p_array<point2d> array_dec_ref;

      for (int j = 0; j < rows; ++j)
	for (int i = 0; i < cols; ++i)
	  array_inc_ref.append(point2d(j, i));

      for (int j = rows - 1; j >= 0; --j)
	for (int i = cols - 1; i >= 0; --i)
	  array_dec_ref.append(point2d(j, i));

      chck(input, array_inc_ref, array_dec_ref);
    }

    p_array<point2d> array_inc_ref;
    p_array<point2d> array_dec_ref;

    for (int j = 0; j < 2; ++j)
      for (int i = 0; i < 2; ++i)
	array_inc_ref.append(point2d(j, i));

    for (int j = 1; j >= 0; --j)
      for (int i = 1; i >= 0; --i)
	array_dec_ref.append(point2d(j, i));

    {
      image2d<I> in (rows, cols);
      sub_image<image2d<I>, box2d> input (in, b2);
      debug::iota(input);
      chck(input, array_inc_ref, array_dec_ref);
    }

    {
      image2d<I> in (rows, cols);
      image_if<image2d<I>, f_box2d_t> input(in, f_b2);
      debug::iota(input);
      chck(input, array_inc_ref, array_dec_ref);
    }
  }

  template <typename I>
  void
  chk3d(int slis, int rows, int cols)
  {
    box3d b3(literal::origin, point3d(1, 1, 1));
    f_box3d_t f_b3(b3);

    {
      image3d<I> input (slis, rows, cols);
      debug::iota(input);

      p_array<point3d> array_inc_ref;
      p_array<point3d> array_dec_ref;

      for (int k = 0; k < slis; ++k)
	for (int j = 0; j < rows; ++j)
	  for (int i = 0; i < cols; ++i)
	    array_inc_ref.append(point3d(k, j, i));

      for (int k = slis - 1; k >= 0; --k)
	for (int j = rows - 1; j >= 0; --j)
	  for (int i = cols - 1; i >= 0; --i)
	    array_dec_ref.append(point3d(k, j, i));

      chck(input, array_inc_ref, array_dec_ref);
    }

    p_array<point3d> array_inc_ref;
    p_array<point3d> array_dec_ref;

    for (int k = 0; k < 2; ++k)
      for (int j = 0; j < 2; ++j)
	for (int i = 0; i < 2; ++i)
	  array_inc_ref.append(point3d(k, j, i));

    for (int k = 1; k >= 0; --k)
      for (int j = 1; j >= 0; --j)
	for (int i = 1; i >= 0; --i)
	  array_dec_ref.append(point3d(k, j, i));

    {
      image3d<I> in (slis, rows, cols);
      sub_image<image3d<I>, box3d> input (in, b3);
      debug::iota(input);
      chck(input, array_inc_ref, array_dec_ref);
    }

    {
      image3d<I> in (slis, rows, cols);
      image_if<image3d<I>, f_box3d_t> input(in, f_b3);
      debug::iota(input);
      chck(input, array_inc_ref, array_dec_ref);
    }

  }


  template <typename I>
  void
  chk(int slis, int rows, int cols)
  {
    (std::cerr << "  in 1d ... ").flush ();
    {
      for (int i = 2; i < cols; ++i)
	chk1d<I>(i);
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "  in 2d ... ").flush ();
    {
      for (int j = 2; j < rows; ++j)
	for (int i = 2; i < cols; ++i)
	  chk2d<I>(j, i);
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "  in 3d ... ").flush ();
    {
      for (int k = 2; k < slis; ++k)
	for (int j = 2; j < rows; ++j)
	  for (int i = 2; i < cols; ++i)
	    chk3d<I>(k, j, i);
    }
    std::cerr << "OK" << std::endl;
  }

}


int main()
{
  using namespace mln;

  int slis =  8;
  int rows =  8;
  int cols =  8;

  std::cerr << "Tests level::sort_psites:" << std::endl;
  std::cerr << "in int:" << std::endl;
  chk<int>(slis, rows, cols);
  std::cerr << "in unsigned:" << std::endl;
  chk<unsigned>(slis, rows, cols);
  std::cerr << "in int_u16:" << std::endl;
  chk<value::int_u16>(slis, rows, cols);
  std::cerr << "in int_s16:" << std::endl;
  chk<value::int_s16>(slis, rows, cols);
}
