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
#include <mln/debug/iota.hh>
#include <mln/border/duplicate.hh>
#include <mln/opt/element.hh>


using namespace mln;

int
main (void)
{
  image2d<int> ima(3, 3, 1);

  debug::iota(ima);
  border::duplicate(ima);

  mln_assertion(opt::element(ima, 0) == 1);
  mln_assertion(opt::element(ima, 1) == 1);
  mln_assertion(opt::element(ima, 2) == 2);
  mln_assertion(opt::element(ima, 3) == 3);
  mln_assertion(opt::element(ima, 4) == 3);
  mln_assertion(opt::element(ima, 5) == 1);
  mln_assertion(opt::element(ima, 6) == 1);
  mln_assertion(opt::element(ima, 7) == 2);
  mln_assertion(opt::element(ima, 8) == 3);
  mln_assertion(opt::element(ima, 9) == 3);
  mln_assertion(opt::element(ima, 10) == 4);
  mln_assertion(opt::element(ima, 11) == 4);
  mln_assertion(opt::element(ima, 12) == 5);
  mln_assertion(opt::element(ima, 13) == 6);
  mln_assertion(opt::element(ima, 14) == 6);
  mln_assertion(opt::element(ima, 15) == 7);
  mln_assertion(opt::element(ima, 16) == 7);
  mln_assertion(opt::element(ima, 17) == 8);
  mln_assertion(opt::element(ima, 18) == 9);
  mln_assertion(opt::element(ima, 19) == 9);
  mln_assertion(opt::element(ima, 20) == 7);
  mln_assertion(opt::element(ima, 21) == 7);
  mln_assertion(opt::element(ima, 22) == 8);
  mln_assertion(opt::element(ima, 23) == 9);
  mln_assertion(opt::element(ima, 24) == 9);
}
