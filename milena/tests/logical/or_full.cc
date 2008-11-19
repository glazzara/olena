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

/*! \file tests/logical/or_full.cc
 *
 * \brief Tests on mln::logical::or.
 */

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/logical/or.hh>
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

  template <typename I, typename J>
  void
  chck(Image<I>& ima1_, const Image<J>& ima2_)
  {
    const I& ima1 = exact(ima1_);
    I& ima3 = exact(ima1_);
    const J& ima2 = exact(ima2_);
    mln_concrete(I) log = logical::or_(ima1, ima2);

    mln_assertion (ima2.domain () >= ima1.domain ());
    mln_piter(I) p (ima1.domain ());
    for_all(p)
      mln_assertion ((ima1(p) || ima2(p)) == log(p));
    logical::or_inplace(ima3, ima2);
    for_all(p)
      mln_assertion (ima3(p) == log(p));
  }


  template <typename I, typename J>
  void
  chk1d(unsigned col,
	unsigned mod1, unsigned mod2)
  {
    image1d<I> ima1 (col);
    image1d<J> ima2 (col);

    debug::iota(ima1);
    debug::iota(ima2);

    level::apply(ima1, fun::v2v::chess_value_t<I>(mod1) );
    level::apply(ima2, fun::v2v::chess_value_t<J>(mod2) );

    chck(ima1, ima2);
  }


  template <typename I, typename J>
  void
  chk2d(unsigned row, unsigned col,
	unsigned mod1, unsigned mod2)
  {
    image2d<I> ima1 (row, col);
    image2d<J> ima2 (row, col);

    debug::iota(ima1);
    debug::iota(ima2);

    level::apply(ima1, fun::v2v::chess_value_t<I>(mod1) );
    level::apply(ima2, fun::v2v::chess_value_t<J>(mod2) );

    chck(ima1, ima2);
  }


  template <typename I, typename J>
  void
  chk3d(unsigned sli, unsigned row, unsigned col,
	unsigned mod1, unsigned mod2)
  {
    image3d<I> ima1 (sli, row, col);
    image3d<J> ima2 (sli, row, col);

    debug::iota(ima1);
    debug::iota(ima2);

    level::apply(ima1, fun::v2v::chess_value_t<I>(mod1) );
    level::apply(ima2, fun::v2v::chess_value_t<J>(mod2) );

    chck(ima1, ima2);
  }


}





int main()
{
  using namespace mln;

  unsigned max_mod1 = 8;
  unsigned max_mod2 = 8;
  unsigned max_cols = 16;
  unsigned max_rows = 8;
  unsigned max_slis = 2;

  std::cerr << "Tests logical::or" << std::endl;

  (std::cerr << "in 1d ... ").flush ();
  {
    for (unsigned i = 1; i < max_mod1; ++i)
      for (unsigned j = 1; j < max_mod2; ++j)
	for (unsigned k = 1; k < max_cols; ++k)
	  {
	    chk1d<int, int>(k, i, j);
	    chk1d<unsigned, unsigned>(k, i, j);
	    chk1d<value::int_s8, value::int_s8>(k, i, j);
	    chk1d<int, unsigned>(k, i, j);
 	    chk1d<unsigned, value::int_u8>(k, i, j);
	    chk1d<int, value::int_s8>(k, i, j);
	    chk1d<unsigned, value::int_u16>(k, i, j);
	    chk1d<int, value::int_s16>(k, i, j);
	  }
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 2d ... ").flush ();
  {
    for (unsigned i = 1; i < max_mod1; ++i)
      for (unsigned j = 1; j < max_mod2; ++j)
	for (unsigned k = 1; k < max_cols; ++k)
	  for (unsigned l = 1; l < max_rows; ++l)
	  {
	    chk2d<int, int>(l ,k, i, j);
	    chk2d<unsigned, unsigned>(l ,k, i, j);
	    chk2d<value::int_s8, value::int_s8>(l ,k, i, j);
	    chk2d<int, unsigned>(l ,k, i, j);
 	    chk2d<unsigned, value::int_u8>(l ,k, i, j);
	    chk2d<int, value::int_s8>(l ,k, i, j);
	    chk2d<unsigned, value::int_u16>(l ,k, i, j);
	    chk2d<int, value::int_s16>(l ,k, i, j);
	  }
  }
  std::cerr << "OK" << std::endl;

  (std::cerr << "in 3d ... ").flush ();
  {
    for (unsigned i = 1; i < max_mod1; ++i)
      for (unsigned j = 1; j < max_mod2; ++j)
	for (unsigned k = 1; k < max_cols; ++k)
	  for (unsigned l = 1; l < max_rows; ++l)
	    for (unsigned m = 1; m < max_slis; ++m)
	  {
	    chk3d<int, int>(m, l ,k, i, j);
	    chk3d<unsigned, unsigned>(m, l ,k, i, j);
	    chk3d<value::int_s8, value::int_s8>(m, l ,k, i, j);
	    chk3d<int, unsigned>(m, l ,k, i, j);
 	    chk3d<unsigned, value::int_u8>(m, l ,k, i, j);
	    chk3d<int, value::int_s8>(m, l ,k, i, j);
	    chk3d<unsigned, value::int_u16>(m, l ,k, i, j);
	    chk3d<int, value::int_s16>(m, l ,k, i, j);
	  }
  }
  std::cerr << "OK" << std::endl;

}
