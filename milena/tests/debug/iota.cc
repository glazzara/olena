// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE).
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

#include <mln/value/int_u8.hh>

#include <mln/debug/iota.hh>
#include <mln/data/compare.hh>
#include <mln/test/predicate.hh>

#include <mln/pw/all.hh>
#include <mln/fun/vv2v/diff_abs.hh>


int main()
{
  using namespace mln;
  using value::int_u8;

  // Exercise mln::debug::iota with an image of ints.
  {
    int vs[4][4] = { { 1,  2,  3,  4},
		     { 5,  6,  7,  8},
		     { 9, 10, 11, 12},
		     {13, 14, 15, 16} };

    image2d<int> ref = make::image(vs);
    image2d<int> ima(4, 4);

    debug::iota(ima);
    mln_assertion(ima == ref);
  }

  // Exercise mln::debug::iota with an image of floats.
  {
    float vs[4][4] = { { 1.f,  2.f,  3.f,  4.f},
                       { 5.f,  6.f,  7.f,  8.f},
                       { 9.f, 10.f, 11.f, 12.f},
                       {13.f, 14.f, 15.f, 16.f} };

    image2d<float> ref = make::image(vs);
    image2d<float> ima(4, 4);

    debug::iota(ima);
    // Use a ``tolerant'' comparison.
    float threshold = 0.0001f;
    mln_assertion(test::predicate(ima.domain(),
                                  pw::bind(fun::vv2v::diff_abs<float>(),
                                           pw::value(ima),
                                           pw::value(ref))
                                  < pw::cst(threshold)));
  }
}
