// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>
#include <mln/world/binary_2d/enlarge.hh>
#include <mln/make/image.hh>
#include <mln/value/int_u8.hh>

#include "tests/data.hh"

mln::value::int_u8 vals_i[][4] = { { 4, 4, 4, 5 },
				   { 4, 4, 4, 5 },
				   { 3, 3, 4, 3 },
				   { 3, 3, 2, 2 } };

bool vals_b[][4] = { { 0, 0, 0, 1 },
		     { 0, 0, 0, 1 },
		     { 0, 0, 0, 0 },
		     { 1, 1, 0, 0 } };

int main()
{
  using namespace mln;

  {
    value::int_u8 vals[][2] = { { 4, 5 },
				{ 3, 2 } } ;

    image2d<value::int_u8> input = make::image(vals);
    image2d<value::int_u8> input_2x = world::binary_2d::enlarge(input,1);

    image2d<value::int_u8> ref = make::image(vals_i);

    mln_assertion(ref == input_2x);
  }

  {
    bool vals[][2] = { { 0, 1 },
		       { 1, 0 } } ;

    image2d<bool> input = make::image(vals);
    image2d<bool> input_2x = world::binary_2d::enlarge(input,1);

    image2d<bool> ref = make::image(vals_b);

    mln_assertion(ref == input_2x);
  }

}
