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

/*! \file tests/level/compare_full.cc
 *
 * \brief Tests on mln::level::compare.
 */

#include <mln/core/image1d.hh>
#include <mln/core/image2d.hh>
#include <mln/core/image3d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>

#include <mln/level/compare.hh>

#include <mln/debug/iota.hh>

#include <mln/arith/plus.hh>
#include <mln/arith/minus.hh>


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
  template<typename I, typename J>
  void
  test_less(I& lhs, J& rhs)
  {
    mln_assertion(! (lhs == rhs));
    mln_assertion(  (lhs <= rhs));
    mln_assertion(  (lhs <  rhs));
    mln_assertion(! (lhs >= rhs));
    mln_assertion(! (lhs >  rhs));

    mln_assertion(! (rhs == lhs));
    mln_assertion(! (rhs <= lhs));
    mln_assertion(! (rhs <  lhs));
    mln_assertion(  (rhs >= lhs));
    mln_assertion(  (rhs >  lhs));
  }

  template<typename I, typename J>
  void
  test_more(I& lhs, J& rhs)
  {
    mln_assertion(! (lhs == rhs));
    mln_assertion(  (lhs <= rhs));
    mln_assertion(  (lhs <  rhs));
    mln_assertion(! (lhs >= rhs));
    mln_assertion(! (lhs >  rhs));

    mln_assertion(! (rhs == lhs));
    mln_assertion(! (rhs <= lhs));
    mln_assertion(! (rhs <  lhs));
    mln_assertion(  (rhs >= lhs));
    mln_assertion(  (rhs >  lhs));
  }

  template<typename I, typename J>
  void
  test_equal(I& lhs, J& rhs)
  {
    mln_assertion(  (lhs == rhs));
    mln_assertion(  (lhs <= rhs));
    mln_assertion(! (lhs <  rhs));
    mln_assertion(  (lhs >= rhs));
    mln_assertion(! (lhs >  rhs));

    mln_assertion(  (rhs == lhs));
    mln_assertion(  (rhs <= lhs));
    mln_assertion(! (rhs <  lhs));
    mln_assertion(  (rhs >= lhs));
    mln_assertion(! (rhs >  lhs));
  }

  template <typename I>
  void
  chk1d(unsigned cols)
  {
    image1d<I> lhs (cols);
    image1d<I> rhs (cols);
    debug::iota (lhs);
    debug::iota (rhs);

    test_equal(lhs, rhs);
    image1d<int> lhs2 = lhs + 0;
    image1d<int> rhs2 = rhs + 1;
    test_less(lhs2, rhs2);
    image1d<int> lhs3 = lhs - 1;
    image1d<int> rhs3 = rhs + 0;
    test_more(lhs3, rhs3);
  }

  template <typename I>
  void
  chk2d(unsigned rows, unsigned cols)
  {
    image2d<I> lhs (rows, cols);
    image2d<I> rhs (rows, cols);
    debug::iota (lhs);
    debug::iota (rhs);

    test_equal(lhs, rhs);
    image2d<int> lhs2 = lhs + 0;
    image2d<int> rhs2 = rhs + 1;
    test_less(lhs2, rhs2);
    image2d<int> lhs3 = lhs - 1;
    image2d<int> rhs3 = rhs + 0;
    test_more(lhs3, rhs3);
  }

  template <typename I>
  void
  chk3d(unsigned slis, unsigned rows, unsigned cols)
  {
    image3d<I> lhs (slis, rows, cols);
    image3d<I> rhs (slis, rows, cols);
    debug::iota (lhs);
    debug::iota (rhs);

    test_equal(lhs, rhs);
    image3d<int> lhs2 = lhs + 0;
    image3d<int> rhs2 = rhs + 1;
    test_less(lhs2, rhs2);
    image3d<int> lhs3 = lhs - 1;
    image3d<int> rhs3 = rhs + 0;
    test_more(lhs3, rhs3);
  }
}




int main()
{
  using namespace mln;

  unsigned slis_start = 1;
  unsigned slis_end   = 3;

  unsigned rows_start = 1;
  unsigned rows_end   = 16;

  unsigned cols_start = 1;
  unsigned cols_end = 32;

  std::cerr << "Tests level::compare" << std::endl;

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
	    chk3d<value::int_s16>(k, j, i);
	  }
  }
  std::cerr << "OK" << std::endl;
}
