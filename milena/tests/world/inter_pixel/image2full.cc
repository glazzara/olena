// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/world/inter_pixel/image2full.cc
///
/// Tests on mln::world::inter_pixel::image2full

#include <mln/core/image/image2d.hh>
#include <mln/make/image.hh>
#include <mln/value/int_u8.hh>
#include <mln/level/compare.hh>
#include <mln/world/inter_pixel/image2full.hh>

int main()
{
  using namespace mln;

  value::int_u8 vals[][3] = { { 3, 4, 5 },
			      { 1, 3, 6 },
			      { 8, 7, 3 } } ;

  value::int_u8 refs[][5] = { { 3, 0, 4, 0, 5 },
			      { 0, 0, 0, 0, 0 },
			      { 1, 0, 3, 0, 6 },
			      { 0, 0, 0, 0, 0 },
			      { 8, 0, 7, 0, 3 } };

  typedef image2d<value::int_u8> ima_t;
  ima_t ima = make::image(vals);
  ima_t ref = make::image(refs);

  ima_t ima_l = world::inter_pixel::image2full(ima);

  mln_assertion(ima_l == ref);

}
