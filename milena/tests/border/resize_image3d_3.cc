// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/border/resize_image3d_3.cc
///
/// Tests on mln::border::resize.


#include <mln/core/image/image3d.hh>
#include <mln/debug/iota.hh>
#include <mln/value/int_u8.hh>
#include <mln/border/resize.hh>
#include <mln/debug/println_with_border.hh>
#include <mln/border/fill.hh>
#include <mln/data/fill.hh>

using namespace mln;

int main ()
{
  unsigned border = 3;
  unsigned new_border = 1;

  image3d<value::int_u8> ima(1, 3, 2, border);
  data::fill (ima, 2);
  border::fill(ima, 8);

  mln_assertion(ima.has(point3d(-3, -3, -3)) == true);
  mln_assertion(ima.has(point3d(-4, -4, -4)) == false);

  border::resize (ima, new_border);
  mln_assertion(ima.border() == new_border);
  mln_assertion(ima.has(point3d(-1, -1, -1)) == true);
  mln_assertion(ima.has(point3d(-1, -2, -1)) == false);
  mln_assertion(ima.has(point3d(-3, -3, -3)) == false);
  mln_assertion(ima.has(point3d(-4, -4, -4)) == false);
}
