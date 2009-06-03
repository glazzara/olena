// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/world/inter_pixel/immerse.cc
///
/// Tests on mln::world::inter_pixel::immerse.

#include <mln/core/var.hh>
#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>
#include <mln/world/inter_pixel/immerse.hh>


int main()
{
  using namespace mln;

  int vals[] = { 3, 4, 5,
		 1, 3, 6 ,
		 8, 7, 3 } ;

  typedef image2d<int> I;
  I ima = make::image2d(vals);

  using namespace world::inter_pixel;

  typedef image_if<I, is_pixel> Ix;
  Ix imax = immerse(ima);


  int refs[] = { 3, 0, 4, 0, 5,
		 0, 0, 0, 0, 0,
		 1, 0, 3, 0, 6,
		 0, 0, 0, 0, 0,
		 8, 0, 7, 0, 3 };

  mln_assertion(imax == (make::image2d(refs) | is_pixel()));

  mln_piter_(Ix) p(imax.domain());
  for_all(p)
    mln_assertion(is_pixel()(p));
}
