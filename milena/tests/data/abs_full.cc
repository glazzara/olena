// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

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

#include <mln/arith/times.hh>
#include <mln/data/abs.hh>




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
  chck(const Image<I>& ref_)
  {
    const I& ref = exact(ref_);
    I out (ref.domain ());

    data::abs(ref, out);
    mln_piter(I) p (ref.domain ());
    for_all(p)
      if (ref(p) > 0)
	mln_assertion (ref(p) == out(p));
      else
	mln_assertion (ref(p) == -out(p));
  }

  template <typename V>
  void
  chk(unsigned sli, unsigned row, unsigned col)
  {
    box1d b1(literal::origin, point1d(1));
    box2d b2(literal::origin, point2d(1,1));
    box3d b3(literal::origin, point3d(1,1,1));
    f_box1d_t f_b1(b1);
    f_box2d_t f_b2(b2);
    f_box3d_t f_b3(b3);

    (std::cerr << "in 1d ... ").flush ();
    {
      typedef image1d<V> I;

      for (unsigned i = 1; i < col; ++i)
	{
	  I ima(i);
	  debug::iota(ima);
	  chck (ima * -1);
	}
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in 2d ... ").flush ();
    {
      typedef image2d<V> I;

      for (unsigned i = 1; i < col; ++i)
	for (unsigned j = 1; j < row; ++j)
	  {
	    I ima(j, i);
	    debug::iota(ima);
	    chck (ima * -1);
	  }
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in 3d ... ").flush ();
    {
      typedef image3d<V> I;

      for (unsigned i = 1; i < col; ++i)
	for (unsigned j = 1; j < row; ++j)
	  for (unsigned k = 1; k < sli; ++k)
	    {
	      I ima(k, j, i);
	      debug::iota(ima);
	      chck (ima * -1);
	    }
    }
    std::cerr << "OK" << std::endl;
  }
}





int main()
{
  using namespace mln;

  std::cerr << "Tests data::abs:" << std::endl;
  std::cerr << "on int:" << std::endl;
  chk<int>(4, 16, 64);
  std::cerr << "on int_s8:" << std::endl;
  chk<value::int_s8>(2, 2, 2);
  std::cerr << "on int_s16:" << std::endl;
  chk<value::int_s16>(4, 16, 64);
}
