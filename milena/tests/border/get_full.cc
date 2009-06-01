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

/// \file tests/border/get_full.cc
///
/// Tests on mln::border::get.

#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/border/get.hh>
#include <mln/literal/origin.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>


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



int main()
{
  using namespace mln;

  box2d b(literal::origin, point2d(1,1));
  f_box2d_t f_b(b);
  mln::fun::p2b::chess c_b;

  {
    typedef image2d<int> I;
    (std::cerr << "Tests border::get on int ... ").flush ();
    I ima(3,3, 51);

    mln_assertion(border::get(ima) == 51);
    sub_image<I, box2d> sub(ima, b);
    mln_assertion(border::get(sub) == 0);

    image_if<I, f_box2d_t> imaif(ima, f_b);
    mln_assertion(border::get(imaif) == 51);
    mln_assertion(border::get( (ima | b) ) == 0);
    mln_assertion(border::get( (ima | b) | f_b ) == 0);
    std::cerr << "OK" << std::endl;
  }


  {
    typedef image2d<value::int_u8> I;
    (std::cerr << "Tests border::get on int_u8 ... ").flush ();
    I ima(10, 10, 42);
    mln_assertion(border::get(ima) == 42);
    mln_assertion( ima.has(point2d(2,2)) == true );
    sub_image<I, box2d> sub(ima, b);
    mln_assertion(border::get(sub) == 0);

    image_if<I, mln::fun::p2b::chess > imaif(ima, c_b);
    mln_assertion(border::get(imaif) == 42);
    mln_assertion(border::get( (ima | b) ) == 0);
    mln_assertion(border::get( (ima | b) | c_b ) == 0);
    std::cerr << "OK" << std::endl;
  }

  {
    typedef image2d<value::rgb8> I;
    (std::cerr << "Tests border::get on rgb8 ... ").flush ();
    I ima(10, 10, 36);
    mln_assertion(border::get(ima) == 36);
    mln_assertion( ima.has(point2d(2,2)) == true );
    sub_image<I, box2d> sub(ima, b);
    mln_assertion(border::get(sub) == 0);

    image_if<I, mln::fun::p2b::chess > imaif(ima, c_b);
    mln_assertion(border::get(imaif) == 36);
    mln_assertion(border::get( (ima | b) ) == 0);
    mln_assertion(border::get( (ima | b) | c_b ) == 0);
    std::cerr << "OK" << std::endl;
  }

}
