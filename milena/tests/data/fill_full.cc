// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory
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

/// \file tests/data/fill_full.cc
///
/// Tests on mln::data::fill



#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/sub_image.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/literal/origin.hh>

#include <mln/value/int_s8.hh>
#include <mln/value/int_s16.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/rgb16.hh>


#include <mln/debug/iota.hh>

#include <mln/data/fill.hh>

#include <mln/fun/p2v/iota.hh>






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
  chck(Image<I>& ima_, const mln_value(I) v)
  {
    I& ima = exact(ima_);

    data::fill(ima, v);

    {
      mln_piter(I) p (ima.domain ());
      for_all(p)
	mln_assertion (ima(p) == v);
    }
  }

  template <typename V>
  void
  chk(const V max)
  {
    box1d b1(literal::origin, point1d(1));
    box2d b2(literal::origin, point2d(1,1));
    box3d b3(literal::origin, point3d(1,1,1));
    f_box1d_t f_b1(b1);
    f_box2d_t f_b2(b2);
    f_box3d_t f_b3(b3);
    unsigned sli = 2;
    unsigned row = 3;
    unsigned col = 16;


    (std::cerr << "in 1d ... ").flush ();
    {
      typedef image1d<V> I;

      for (V v = 0; v < max; ++v)
	for (unsigned i = 1; i < col; ++i)
	  {
	    I ima(i);
	    chck (ima, v);
	  }
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in 2d ... ").flush ();
    {
      typedef image2d<V> I;

      for (V v = 0; v < max; ++v)
	for (unsigned i = 1; i < col; ++i)
	  for (unsigned j = 1; j < row; ++j)
	    {
	      I ima(j, i);
	      chck (ima, v);
	    }
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in 3d ... ").flush ();
    {
      typedef image3d<V> I;

      for (V v = 0; v < max; ++v)
	for (unsigned i = 1; i < col; ++i)
	  for (unsigned j = 1; j < row; ++j)
	    for (unsigned k = 1; k < sli; ++k)
	    {
	      I ima(k, j, i);
	      chck (ima, v);
	    }
    }
    std::cerr << "OK" << std::endl;


    (std::cerr << "in subimage 1d ... ").flush ();
    {
      typedef image1d<V> I;
      typedef sub_image<I, box1d> J;


      for (V v = 0; v < max; ++v)
	for (unsigned i = 1; i < col; ++i)
	  {
	    I image(i);
	    J ima(image, b1);
	    chck (ima, v);
	  }
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in subimage 2d ... ").flush ();
    {
      typedef image2d<V> I;
      typedef sub_image<I, box2d> J;


      for (V v = 0; v < max; ++v)
	for (unsigned i = 1; i < col; ++i)
	  for (unsigned j = 1; j < row; ++j)
	  {
	    I image(j, i);
	    J ima(image, b2);
	    chck (ima, v);
	  }
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in subimage 3d ... ").flush ();
    {
      typedef image3d<V> I;
      typedef sub_image<I, box3d> J;


      for (V v = 0; v < max; ++v)
	for (unsigned i = 1; i < col; ++i)
	  for (unsigned j = 1; j < row; ++j)
	    for (unsigned k = 1; k < sli; ++k)
	  {
	    I image(k, j, i);
	    J ima(image, b3);
	    chck (ima, v);
	  }
    }
    std::cerr << "OK" << std::endl;
  }

  template <typename V>
  void
  chk_rgb(const unsigned min, const unsigned max)
  {
    box1d b1(literal::origin, point1d(1));
    box2d b2(literal::origin, point2d(1,1));
    box3d b3(literal::origin, point3d(1,1,1));
    f_box1d_t f_b1(b1);
    f_box2d_t f_b2(b2);
    f_box3d_t f_b3(b3);
    unsigned sli = 2;
    unsigned row = 3;
    unsigned col = 16;


    (std::cerr << "in 1d ... ").flush ();
    {
      typedef image1d<V> I;

      for (unsigned r = min; r < max; ++r)
	for (unsigned g = min; g < max; ++g)
	  for (unsigned b = min; b < max; ++b)
	    for (unsigned i = 1; i < col; ++i)
	      {
		I ima(i);
		chck (ima, V(r,g,b));
	      }
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in 2d ... ").flush ();
    {
      typedef image2d<V> I;

      for (unsigned r = min; r < max; ++r)
	for (unsigned g = min; g < max; ++g)
	  for (unsigned b = min; b < max; ++b)
	    for (unsigned i = 1; i < col; ++i)
	      for (unsigned j = 1; j < row; ++j)
		{
		  I ima(j, i);
		  chck (ima, V(r,g,b));
		}
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in 3d ... ").flush ();
    {
      typedef image3d<V> I;

      for (unsigned r = min; r < max; ++r)
	for (unsigned g = min; g < max; ++g)
	  for (unsigned b = min; b < max; ++b)
	    for (unsigned i = 1; i < col; ++i)
	      for (unsigned j = 1; j < row; ++j)
		for (unsigned k = 1; k < sli; ++k)
		  {
		    I ima(k, j, i);
		    chck (ima, V(r,g,b));
		  }
    }
    std::cerr << "OK" << std::endl;


    (std::cerr << "in subimage 1d ... ").flush ();
    {
      typedef image1d<V> I;
      typedef sub_image<I, box1d> J;


      for (unsigned r = min; r < max; ++r)
	for (unsigned g = min; g < max; ++g)
	  for (unsigned b = min; b < max; ++b)
	    for (unsigned i = 1; i < col; ++i)
	      {
		I image(i);
		J ima(image, b1);
		chck (ima, V(r,g,b));
	      }
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in subimage 2d ... ").flush ();
    {
      typedef image2d<V> I;
      typedef sub_image<I, box2d> J;


      for (unsigned r = min; r < max; ++r)
	for (unsigned g = min; g < max; ++g)
	  for (unsigned b = min; b < max; ++b)
	    for (unsigned i = 1; i < col; ++i)
	      for (unsigned j = 1; j < row; ++j)
		{
		  I image(j, i);
		  J ima(image, b2);
		  chck (ima, V(r,g,b));
		}
    }
    std::cerr << "OK" << std::endl;

    (std::cerr << "in subimage 3d ... ").flush ();
    {
      typedef image3d<V> I;
      typedef sub_image<I, box3d> J;


      for (unsigned r = min; r < max; ++r)
	for (unsigned g = min; g < max; ++g)
	  for (unsigned b = min; b < max; ++b)
	    for (unsigned i = 1; i < col; ++i)
	      for (unsigned j = 1; j < row; ++j)
		for (unsigned k = 1; k < sli; ++k)
		  {
		    I image(k, j, i);
		    J ima(image, b3);
		    chck (ima, V(r,g,b));
		  }
    }
    std::cerr << "OK" << std::endl;
  }

}


int main()
{
  using namespace mln;

  std::cerr << "Tests data::fill:" << std::endl;
  std::cerr << "on int:" << std::endl;
  chk<int>(1000);
  std::cerr << "on unsigned:" << std::endl;
  chk<unsigned>(1000);
  std::cerr << "on int_u8:" << std::endl;
  chk<value::int_u8>(16);
  std::cerr << "on int_u16:" << std::endl;
  chk<value::int_u16>(1000);
  std::cerr << "on int_s8:" << std::endl;
  chk<value::int_s8>(16);
  std::cerr << "on int_s16:" << std::endl;
  chk<value::int_s16>(1000);
  std::cerr << "on rgb8:" << std::endl;
  chk_rgb<value::rgb8>(140, 150);
  std::cerr << "on rgb16:" << std::endl;
  chk_rgb<value::rgb16>(60000, 60015);
}
