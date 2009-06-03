// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/data/stretch_full.cc
///
/// Tests on mln::data::stretch.
///
/// \todo Make it pass.

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s16.hh>

#include <mln/debug/iota.hh>
#include <mln/data/saturate.hh>
#include <mln/data/stretch.hh>



namespace mln
{
  template <typename I, typename J>
  void
  chck(Image<I>& input_, Image<J>& output_)
  {
    I& input  = exact(input_);
    J& output = exact(output_);
    output = data::stretch(mln_value(J)(), input);
  }

  template <typename I, typename J>
  void
  chk1d(unsigned cols)
  {
    {
      image1d<I> input(cols);
      debug::iota(input);
      image1d<J> output(cols);
      chck(input, output);
    }

  }

  template <typename I, typename J>
  void
  chk2d(unsigned rows, unsigned cols)
  {

    {
      image2d<I> input(rows, cols);
      debug::iota(input);
      image2d<J> output(rows, cols);
      chck(input, output);
    }

  }

  template <typename I, typename J>
  void
  chk3d(unsigned slis, unsigned rows, unsigned cols)
  {

    {
      image3d<I> input(slis, rows, cols);
      debug::iota(input);
      image3d<J> output(slis, rows, cols);
      chck(input, output);
    }

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
    std::cerr << "  into int_u8:" << std::endl;
    chk<I, value::int_u8>(slis, rows, cols);
    std::cerr << "  into int_u16:" << std::endl;
    chk<I, value::int_u16>(slis, rows, cols);
  }
}


int main()
{
  using namespace mln;

  unsigned slis =  4;
  unsigned rows =  4;
  unsigned cols = 16;

  std::cerr << "Tests data::stretch:" << std::endl;
  std::cerr << "on int:" << std::endl;
  ch<int>(slis, rows, cols);
  std::cerr << "on unsigned:" << std::endl;
  ch<unsigned>(slis, rows, cols);
  std::cerr << "on int_u8:" << std::endl;
  ch<value::int_u8>(slis, rows, cols);
  std::cerr << "on int_u16:" << std::endl;
  ch<value::int_u16>(slis, rows, cols);
  std::cerr << "on int_s8:" << std::endl;
  ch<value::int_s8>(slis, rows, cols);
  std::cerr << "on int_s16:" << std::endl;
  ch<value::int_s16>(slis, rows, cols);
}
