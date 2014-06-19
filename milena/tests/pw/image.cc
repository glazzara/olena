// Copyright (C) 2007, 2008, 2009, 2014 EPITA Research and Development
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

#include <mln/fun/p2b/chess.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/core/alias/box2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/pw/image.hh>


int main()
{
  using namespace mln;

  typedef pw::image<fun::p2b::chess,
		    box<mln::point<grid::square, short int> > > ima_t;
  ima_t ima = fun::p2b::chess() | make::box2d(8, 8);
  // trait::image::print(ima);

  unsigned i = 0;
  mln_piter_(ima_t) p(ima.domain());
  for_all(p)
  {
    if (p.row() % 2)
      mln_assertion(ima(p) == (p.col() % 2));
    else
      mln_assertion(ima(p) == !(p.col() % 2));
    ++i;
  }
  mln_assertion(i == 64);

  // A mutable pw::image.
  {
    p_array<int> arr; // Sites are ints (why not?)
    arr.insert(51); // Site 51.
    typedef pw::image<fun::i2v::array<int>, p_array<int> > ima_t;
    ima_t ima = fun::i2v::array<int>(1) | arr; // An array psite converts to int so that works :-)
    // trait::image::print(ima);

    p_array<int>::psite p(ima.domain(), 0); // index 0 means the 1st element of arr
    ima(p) = 7;
    mln_assertion(ima(p) == 7);
  }

}
