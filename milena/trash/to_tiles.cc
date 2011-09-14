// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/convert/to_tiles.cc
 *
 * \brief Tests on mln::convert::to_tiles.
 */

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/iota.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/border/fill.hh>
#include <mln/debug/println_with_border.hh>
#include <mln/debug/println.hh>
#include <mln/core/image/translate_image.hh>
#include <mln/convert/to_tiles.hh>

int main ()
{
  using namespace mln;

  typedef image2d<value::int_u8> I;
  std::vector<I> vec;

  I ima1 (4, 2, 1);
  data::fill(ima1, 1);
  vec.push_back(ima1);

  I ima2 (4, 2, 1);
  data::fill(ima2, 2);
  vec.push_back(ima2);

  I ima3 (4, 2, 1);
  data::fill(ima3, 3);
  vec.push_back(ima3);

  I ima4 (4, 2, 1);
  data::fill(ima4, 4);
  vec.push_back(ima4);

  I ima5 (4, 2, 1);
  data::fill(ima5, 5);
  vec.push_back(ima5);

  I ima6 (4, 2, 1);
  data::fill(ima6, 6);
  vec.push_back(ima6);

  I ima7 (4, 2, 1);
  data::fill(ima7, 7);
  vec.push_back(ima7);

  I ima8 (4, 2, 1);
  data::fill(ima8, 8);
  vec.push_back(ima8);

  I ima9 (4, 2, 1);
  data::fill(ima9, 9);
  vec.push_back(ima9);

  I output = convert::to_tiles(vec, 1.33333f);

  debug::println (output);
}
