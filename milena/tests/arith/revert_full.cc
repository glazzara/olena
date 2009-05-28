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

/// \file tests/arith/revert_full.cc
///
/// Tests on mln::arith::revert.
///
/// \todo Rewrite this non-sense test file!!!

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/fun/p2b/chess.hh>
#include <mln/literal/origin.hh>
#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>

#include <mln/debug/iota.hh>
#include <mln/arith/revert.hh>




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
  chck(const Image<I>& ref_, const Image<J>& ima_)
  {
    typedef mln_value(I) V;
    const I& ref = exact(ref_);
    const J& ima = exact(ima_);

    mln_piter(I) p (ima.domain ());
    for_all(p)
      mln_assertion (ima(p) == mln_value(J)(mln_min(V) + mln_max(V) - ref(p)));
  }

  template <typename V>
  void
  chk()
  {
    box1d b1(literal::origin, point1d(1));
    box2d b2(literal::origin, point2d(1,1));
    box3d b3(literal::origin, point3d(1,1,1));
    f_box1d_t f_b1(b1);
    f_box2d_t f_b2(b2);
    f_box3d_t f_b3(b3);
    unsigned sli = 5;
    unsigned row = 10;
    unsigned col = 100;


    {
      typedef image1d<V> I;


	for (unsigned i = 1; i < col; ++i)
	  {
	    I ref(i);
	    debug::iota(ref);
	    I ima(i);
	    debug::iota(ima);
	    arith::revert_inplace(ima);
	    chck (ref, ima);
	  }
    }
    {
      typedef image2d<V> I;


	for (unsigned i = 1; i < col; ++i)
	  for (unsigned j = 1; j < row; ++j)
	    {
	      I ref(j, i);
	      debug::iota(ref);
	      I ima(j, i);
	      debug::iota(ima);
	      arith::revert_inplace(ima);
	      chck (ref, ima);
	    }
    }
    {
      typedef image3d<V> I;


	for (unsigned i = 1; i < col; ++i)
	  for (unsigned j = 1; j < row; ++j)
	    for (unsigned k = 1; k < sli; ++k)
	    {
	      I ref(k, j, i);
	      debug::iota(ref);
	      I ima(k, j, i);
	      debug::iota(ima);
	      arith::revert_inplace(ima);
	      chck (ref, ima);
	    }
    }
    {
      typedef image1d<V> I;
      typedef sub_image<I, box1d> J;



	for (unsigned i = 1; i < col; ++i)
	  {
	    I refima(i);
	    J ref(refima, b1);
	    debug::iota(ref);

	    I image(i);
	    J ima(image, b1);
	    debug::iota(ima);
	    arith::revert_inplace(ima);
	    chck (ref, ima);
	  }
    }
    {
      typedef image2d<V> I;
      typedef sub_image<I, box2d> J;


	for (unsigned i = 1; i < col; ++i)
	  for (unsigned j = 1; j < row; ++j)
	  {
	    I refima(j, i);
	    J ref(refima, b2);
	    debug::iota(ref);

	    I image(j, i);
	    J ima(image, b2);
	    debug::iota(ima);
	    arith::revert_inplace(ima);
	    chck (ref, ima);
	  }
    }
    {
      typedef image3d<V> I;
      typedef sub_image<I, box3d> J;


	for (unsigned i = 1; i < col; ++i)
	  for (unsigned j = 1; j < row; ++j)
	    for (unsigned k = 1; k < sli; ++k)
	  {
	    I refima(k, j, i);
	    J ref(refima, b3);
	    debug::iota(ref);

	    I image(k, j, i);
	    J ima(image, b3);
	    debug::iota(ima);
	    arith::revert_inplace(ima);
	    chck (ref, ima);
	  }
    }
  }

}





int main()
{
  using namespace mln;

  chk<int>();
  chk<value::int_s8>();
  chk<value::int_s16>();
  chk<value::int_u8>();
  chk<unsigned>();
}

