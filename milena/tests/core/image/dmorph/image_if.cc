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
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/fun/p2b/chess.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  typedef image2d<int> I;
  I ima(8, 8);
  // debug::println(ima | fun::p2b::chess());
  // FIXME: is this line corret?
  ///mln_assertion((ima | fun::p2b::chess()).nsites() == 32);

  {
    typedef image_if<I, fun::p2b::chess> II;
    II ima_ref = ima | fun::p2b::chess();
    debug::println(ima_ref);
    {
      II ima_ref_;
      ima_ref_ = ima_ref;
      debug::println(ima_ref_);
    }
    {
      II ima_;
      initialize(ima_, ima_ref);
      debug::println(ima_);
    }
  }
}
