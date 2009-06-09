// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/debug/iota.hh>
#include <mln/border/mirror.hh>
#include <mln/opt/element.hh>

#include <mln/debug/println_with_border.hh>

using namespace mln;

int
main (void)
{
  {
    image1d<int> ima(2, 3);
    debug::iota(ima);
    border::mirror(ima);
    mln_assertion(opt::element(ima, 0) == 2);
    mln_assertion(opt::element(ima, 1) == 2);
    mln_assertion(opt::element(ima, 2) == 1);
    mln_assertion(opt::element(ima, 3) == 1);
    mln_assertion(opt::element(ima, 4) == 2);
    mln_assertion(opt::element(ima, 5) == 2);
    mln_assertion(opt::element(ima, 6) == 1);
    mln_assertion(opt::element(ima, 7) == 1);
  }


  image2d<int> ima(2, 3, 2);

  debug::iota(ima);
  border::mirror(ima);

  mln_assertion(opt::element(ima, 0) == 1);
  mln_assertion(opt::element(ima, 1) == 1);
  mln_assertion(opt::element(ima, 2) == 4);
  mln_assertion(opt::element(ima, 3) == 5);
  mln_assertion(opt::element(ima, 4) == 6);
  mln_assertion(opt::element(ima, 5) == 3);
  mln_assertion(opt::element(ima, 6) == 3);
  mln_assertion(opt::element(ima, 7) == 1);
  mln_assertion(opt::element(ima, 8) == 1);
  mln_assertion(opt::element(ima, 9) == 1);
  mln_assertion(opt::element(ima, 10) == 2);
  mln_assertion(opt::element(ima, 11) == 3);
  mln_assertion(opt::element(ima, 12) == 3);
  mln_assertion(opt::element(ima, 13) == 3);
  mln_assertion(opt::element(ima, 14) == 2);
  mln_assertion(opt::element(ima, 15) == 1);
  mln_assertion(opt::element(ima, 16) == 1);
  mln_assertion(opt::element(ima, 17) == 2);
  mln_assertion(opt::element(ima, 18) == 3);
  mln_assertion(opt::element(ima, 19) == 3);
  mln_assertion(opt::element(ima, 20) == 2);
  mln_assertion(opt::element(ima, 21) == 5);
  mln_assertion(opt::element(ima, 22) == 4);
  mln_assertion(opt::element(ima, 23) == 4);
  mln_assertion(opt::element(ima, 24) == 5);
  mln_assertion(opt::element(ima, 25) == 6);
  mln_assertion(opt::element(ima, 26) == 6);
  mln_assertion(opt::element(ima, 27) == 5);
  mln_assertion(opt::element(ima, 28) == 4);
  mln_assertion(opt::element(ima, 29) == 4);
  mln_assertion(opt::element(ima, 30) == 4);
  mln_assertion(opt::element(ima, 31) == 5);
  mln_assertion(opt::element(ima, 32) == 6);
  mln_assertion(opt::element(ima, 33) == 6);
  mln_assertion(opt::element(ima, 34) == 6);
  mln_assertion(opt::element(ima, 35) == 4);
  mln_assertion(opt::element(ima, 36) == 4);
  mln_assertion(opt::element(ima, 37) == 1);
  mln_assertion(opt::element(ima, 38) == 2);
  mln_assertion(opt::element(ima, 39) == 3);
  mln_assertion(opt::element(ima, 40) == 6);
  mln_assertion(opt::element(ima, 41) == 6);
}
