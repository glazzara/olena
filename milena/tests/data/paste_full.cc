// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory
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

/// \file tests/data/paste_full.cc
///
/// Tests on mln::data::paste.



#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/sub_image.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/literal/origin.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>


#include <mln/debug/iota.hh>

#include <mln/data/saturate.hh>
#include <mln/data/paste.hh>



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
  template <typename I, typename J>
  void
  chck(Image<I>& input_, Image<J>& output_)
  {
    typedef mln_value(I) T;
    typedef mln_value(J) V;
    unsigned max_i = mln_max (T);
    unsigned max_j = mln_max (V);
    I& input = exact(input_);
    J& output = exact(output_);

    if (max_i > max_j)
      data::saturate_inplace(input, 0, (T)max_j);

    data::paste(input, output);

    mln_piter(I) p (input.domain ());
    for_all(p)
      mln_assertion ((V)(input(p)) == output(p));
  }

  template <typename I, typename J>
  void
  chk1d(unsigned cols)
  {
    box1d b1(literal::origin, point1d(1));
    f_box1d_t f_b1(b1);

    {
      image1d<I> input (cols);
      debug::iota(input);
      image1d<J> output (cols);
      chck(input, output);
    }

    {
      image1d<I> in (cols);
      sub_image<image1d<I>, box1d> input (in, b1);
      debug::iota(input);
      image1d<J> output (cols);
      chck(input, output);
    }

    /// FIXME : Uncomment this when it will be possible
//     {
//       image1d<I> in (cols);
//       image_if<image1d<I>, f_box1d_t> input(in, f_b1);
//       debug::iota(input);
//       image1d<J> output (cols);
//       chck(input, output);
//     }
  }

  template <typename I, typename J>
  void
  chk2d(unsigned rows, unsigned cols)
  {
    box2d b2(literal::origin, point2d(1, 1));
    f_box2d_t f_b2(b2);

    {
      image2d<I> input (rows, cols);
      debug::iota(input);
      image2d<J> output (rows, cols);
      chck(input, output);
    }

    {
      image2d<I> in (rows, cols);
      sub_image<image2d<I>, box2d> input (in, b2);
      debug::iota(input);
      image2d<J> output (rows, cols);
      chck(input, output);
    }

    /// FIXME : Uncomment this when it will be possible
//     {
//       image2d<I> in (rows, cols);
//       image_if<image2d<I>, f_box2d_t> input(in, f_b2);
//       debug::iota(input);
//       image2d<J> output (rows, cols);
//       chck(input, output);
//     }
  }

  template <typename I, typename J>
  void
  chk3d(unsigned slis, unsigned rows, unsigned cols)
  {
    box3d b3(literal::origin, point3d(1, 1, 1));
    f_box3d_t f_b3(b3);

    {
      image3d<I> input (slis, rows, cols);
      debug::iota(input);
      image3d<J> output (slis, rows, cols);
      chck(input, output);
    }

    {
      image3d<I> in (slis, rows, cols);
      sub_image<image3d<I>, box3d> input (in, b3);
      debug::iota(input);
      image3d<J> output (slis, rows, cols);
      chck(input, output);
    }

    /// FIXME : Uncomment this when it will be possible
//     {
//       image3d<I> in (slis, rows, cols);
//       image_if<image3d<I>, f_box3d_t> input(in, f_b3);
//       debug::iota(input);
//       image3d<J> output (slis, rows, cols);
//       chck(input, output);
//     }
  }


  template <typename I, typename J>
  void
  chk(unsigned slis, unsigned rows, unsigned cols)
  {
    (std::cerr << "    in 1d ... ").flush ();
    {
      for (unsigned i = 2; i < cols; ++i)
	chk1d<I, J>(i);
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "    in 2d ... ").flush ();
    {
      for (unsigned j = 2; j < rows; ++j)
	for (unsigned i = 2; i < cols; ++i)
	  chk2d<I, J>(j, i);
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "    in 3d ... ").flush ();
    {
      for (unsigned k = 2; k < slis; ++k)
	for (unsigned j = 2; j < rows; ++j)
	  for (unsigned i = 2; i < cols; ++i)
	    chk3d<I, J>(k, j, i);
    }
    std::cerr << "OK" << std::endl;
  }

  template <typename I>
  void
  ch(unsigned slis, unsigned rows, unsigned cols)
  {
    std::cerr << "  into int:" << std::endl;
    chk<I, int>(slis, rows, cols);
    std::cerr << "  into unsigned:" << std::endl;
    chk<I, unsigned>(slis, rows, cols);
    std::cerr << "  into int_u8:" << std::endl;
    chk<I, value::int_u8>(slis, rows, cols);
    std::cerr << "  into int_u16:" << std::endl;
    chk<I, value::int_u16>(slis, rows, cols);
    std::cerr << "  into int_s8:" << std::endl;
    chk<I, value::int_s8>(slis, rows, cols);
    std::cerr << "  into int_s16:" << std::endl;
    chk<I, value::int_s16>(slis, rows, cols);
  }
}


int main()
{
  using namespace mln;

  unsigned slis =  4;
  unsigned rows =  4;
  unsigned cols = 16;

  std::cerr << "Tests data::paste:" << std::endl;
  std::cerr << "on int:" << std::endl;
  ch<int>(slis, rows, cols);
  std::cerr << "on unsigned:" << std::endl;
  ch<unsigned>(slis, rows, cols);
}

