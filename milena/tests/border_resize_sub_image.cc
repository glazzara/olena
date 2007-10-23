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

/*! \file tests/border_resize_sub_image.cc
 *
 * \brief Tests on mln::border::resize.
 */

#include <mln/core/image2d.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/image_if.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/border/get.hh>
#include <mln/literal/origin.hh>
#include <mln/border/resize.hh>

struct my_box2d : mln::Function_p2b< my_box2d >
{
  my_box2d(const mln::box2d& b)
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

  typedef image2d<int> I;
  unsigned border = 42;
  unsigned new_border = 51;

  box2d b(literal::origin, point2d(1,1));
  I ima(3,3, border);

  mln_assertion(border::get(ima) == border);
  sub_image<I, box2d> sub(ima, b);
  mln_assertion( sub.has  (point2d(2,2)) == false && 
		 sub.owns_(point2d(2,2)) == false );

  // A subimage has no border so it cannot be resized => we get a compilation error as expected.
  // border::resize(sub, new_border);
}
