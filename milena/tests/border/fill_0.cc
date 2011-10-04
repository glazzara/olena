// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Test filling an image with a border of 0.

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/debug/iota.hh>
#include <mln/border/fill.hh>

int
main()
{
  using namespace mln;

  border::thickness = 0;

  {
    // Image with values of width 1 (sizeof(int_u8) == 1), the border
    // of which is filled using mln::border::impl::fill_size_1().
    using mln::value::int_u8;
    image2d<int_u8> ima(3, 3);
    debug::iota(ima);
    border::fill(ima, 42);
  }

  {
    // Image with values of width > 1 (sizeof(int_u16) == 2), the border
    // of which is filled using mln::border::impl::fill_size_n().
    using mln::value::int_u16;
    image2d<int_u16> ima(3, 3);
    debug::iota(ima);
    border::fill(ima, 42);
  }
}
