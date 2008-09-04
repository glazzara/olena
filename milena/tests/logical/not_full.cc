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

/*! \file tests/logical/not_full.cc
 *
 * \brief Tests on mln::logical::not.
 */

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/logical/not.hh>
#include <mln/level/compare.hh>
#include <mln/level/apply.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/core/concept/function.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>

#include <mln/literal/zero.hh>

namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      template <typename V, typename T = V, typename R = T>
      struct chess_value_t : public Function_v2v< chess_value_t<V,T,R> >
      {
	chess_value_t (V mod) : mod_ (mod) {}

	typedef R result;
	R operator()(const V& v) const
	{
	  return v % mod_ ? v : literal::zero;
	}

	V mod_;
      };

    }

  }

  template <typename I>
  void
  chck(Image<I>& ima_)
  {
    const I& ima = exact(ima_);
    I& ima2 = exact(ima_);
    mln_concrete(I) log = logical::not_(ima);

    mln_piter(I) p (ima.domain ());
    for_all(p)
      mln_assertion (!ima(p) == log(p));
    logical::not_inplace(ima2);
    for_all(p)
      mln_assertion (ima2(p) == log(p));
  }


  template <typename I>
  void
  chk1d(unsigned col, unsigned mod)
  {
    image1d<I> ima (col);
    debug::iota(ima);
    level::apply(ima, fun::v2v::chess_value_t<I>(mod) );

    chck(ima);
  }


  template <typename I>
  void
  chk2d(unsigned row, unsigned col, unsigned mod)
  {
    image2d<I> ima (row, col);
    debug::iota(ima);
    level::apply(ima, fun::v2v::chess_value_t<I>(mod) );

    chck(ima);
  }


  template <typename I>
  void
  chk3d(unsigned sli, unsigned row, unsigned col,
	unsigned mod)
  {
    image3d<I> ima (sli, row, col);
    debug::iota(ima);
    level::apply(ima, fun::v2v::chess_value_t<I>(mod) );

    chck(ima);
  }

}



int main()
{
  using namespace mln;

  unsigned max_mod = 64;
  unsigned max_cols = 128;
  unsigned max_rows = 4;
  unsigned max_slis = 2;

  std::cerr << "Tests logical::not" << std::endl;

  (std::cerr << "in 1d ... ").flush ();
  {
    for (unsigned i = 1; i < max_mod; ++i)
      for (unsigned j = 1; j < max_cols; ++j)
	{
	  chk1d<int>(j, i);
	  chk1d<unsigned>(j, i);
	  chk1d<value::int_s8>(j, i);
	  chk1d<value::int_s16>(j, i);
	}
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 2d ... ").flush ();
  {
    for (unsigned i = 1; i < max_mod; ++i)
      for (unsigned j = 1; j < max_rows; ++j)
	for (unsigned k = 1; k < max_cols; ++k)
	  {
	    chk2d<int>(j, k, i);
	    chk2d<unsigned>(j, k, i);
	    chk2d<value::int_s8>(j, k, i);
	    chk2d<value::int_s16>(j, k, i);
	  }
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 3d ... ").flush ();
  {
    for (unsigned i = 1; i < max_mod; ++i)
      for (unsigned j = 1; j < max_slis; ++j)
	for (unsigned k = 1; k < max_rows; ++k)
	  for (unsigned l = 1; l < max_cols; ++l)
	    {
	      chk3d<int>(j, k, l, i);
	      chk3d<unsigned>(j, k, l, i);
	      chk3d<value::int_s8>(j, k, l, i);
	      chk3d<value::int_s16>(j, k, l, i);
	    }
  }
  std::cerr << "OK" << std::endl;

}
