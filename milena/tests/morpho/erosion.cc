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

#include <mln/core/image/image2d.hh>
#include <mln/win/all.hh>
#include <mln/io/pgm/load.hh>
#include <mln/value/int_u8.hh>

#include <mln/morpho/erosion.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 0;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");

  win::rectangle2d rec(21, 21);
  morpho::erosion(lena, rec);

  win::hline2d hline(31);
  morpho::erosion(lena, hline);

  win::vline2d vline(31);
  morpho::erosion(lena, vline);

  win::diag2d diag2d(31);
  morpho::erosion(lena, diag2d);

  win::backdiag2d backdiag2d(31);
  morpho::erosion(lena, backdiag2d);

  win::octagon2d oct(6 * 3 + 1);
  morpho::erosion(lena, oct);
}
