// Copyright (C) 2007, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/core/image/morph/t_image.cc
///
/// Tests on mln::t_image.


#include <mln/core/image/image2d.hh>
#include <mln/core/image/morph/t_image.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

int main()
{
  using namespace mln;

  typedef image2d<int> I;

  I ima(2, 3);
  debug::iota(ima);
  debug::println(ima);

  t_image<I> t_ima(ima, 0, 1);
  debug::println(t_ima);

  // FIXME: Enrich this test (exercise more features and write
  // assertions).
}
