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

/// \file tests/border/find_full.cc
///
/// Tests on mln::border::find.

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/sub_image.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/fun/p2b/chess.hh>
#include <mln/border/find.hh>
#include <mln/literal/origin.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>


#include <mln/debug/iota.hh>
#include <mln/border/find.hh>
#include <mln/core/routine/duplicate.hh>


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


int main()
{
  using namespace mln;

  box1d b1(literal::origin, point1d(1));
  box2d b2(literal::origin, point2d(1,1));
  box3d b3(literal::origin, point3d(1,1,1));
  f_box1d_t f_b1(b1);
  f_box2d_t f_b2(b2);
  f_box3d_t f_b3(b3);
  mln::fun::p2b::chess c_b;

  {
    typedef image1d<int> I;
    (std::cerr << "Tests border::find on int in 1d ... ").flush ();
    I ima(3, 51);
    debug::iota(ima);
    mln_assertion(border::find(ima) == 51);
    sub_image<I, box1d> sub(ima, b1);
    mln_assertion(border::find(sub) == 51);

    image_if<I, f_box1d_t> imaif(ima, f_b1);
    mln_assertion(border::find(imaif) == 51);

    mln_assertion(border::find( (ima | b1) ) == 51);
    mln_assertion(border::find( (ima | b1) | f_b1 ) == 51);
    std::cerr << "OK" << std::endl;
  }

  {
    typedef image2d<value::int_u8> I;
    (std::cerr << "Tests border::find on int_u8 in 2d ... ").flush ();
    I ima(10, 10, 42);
    debug::iota(ima);
    mln_assertion(border::find(ima) == 42);
    sub_image<I, box2d> sub(ima, b2);
    mln_assertion(border::find(sub) == 42);

    image_if<I, f_box2d_t > imaif(ima, f_b2);
    mln_assertion(border::find(imaif) == 42);

    image_if<I, fun::p2b::chess > imaif_chess(ima, c_b);
    mln_assertion(border::find(imaif_chess) == 42);


    mln_assertion(border::find( (ima | b2) ) == 42);
    mln_assertion(border::find( (ima | b2) | f_b2 ) == 42);
    mln_assertion(border::find( (ima | b2) | c_b ) == 42);
    mln_assertion(border::find( (ima | b2) | c_b | f_b2 ) == 42);
    mln_assertion(border::find( (ima | b2) | f_b2 | c_b ) == 42);

    std::cerr << "OK" << std::endl;
  }

  {
    typedef image3d<value::rgb8> I;
    (std::cerr << "Tests border::find on rgb8 in 3d ... ").flush ();
    I ima(10, 10, 10, 36);
    mln_assertion(border::find(ima) == 36);
    mln_assertion( ima.has(point3d(2,2,2)) == true );
    sub_image<I, box3d> sub(ima, b3);
    mln_assertion(border::find(sub) == 36);

    image_if<I, f_box3d_t> imaif(ima, f_b3);
    mln_assertion(border::find(imaif) == 36);
    mln_assertion(border::find( (ima | b3) ) == 36);
    mln_assertion(border::find( (ima | b3) | f_b3 ) == 36);
    std::cerr << "OK" << std::endl;
  }
}
