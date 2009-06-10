// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/debug/println.hh>



int main()
{
  using namespace mln;

  typedef image2d<int> I;

  I ref( make::box2d(3,3) );
  box2d b = make::box2d(2,2);
  
  {
    I ima;
    initialize(ima, ref);
    debug::println(ima);
    mln_assertion(ima.border() == ref.border());
  }

  {
    I ima_; // to init'd
    sub_image<I, box2d> ima = ref | b;
    initialize(ima_, ima);
    debug::println(ima);
  }

  {
    sub_image<I, box2d> ima;
    initialize(ima, ref);
    debug::println(ima);
  }

  {
    sub_image<I, box2d> ima;
    initialize(ima, ref | b);
    debug::println(ima);
  }

}
