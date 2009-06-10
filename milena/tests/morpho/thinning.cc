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
#include <mln/value/int_u8.hh>

#include <mln/win/rectangle2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>

#include <mln/morpho/thinning.hh>

#include<mln/convert/to.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  bool fg[] = { 0, 0, 0,
		0, 1, 0,
		0, 0, 0 };
  window2d win_fg = convert::to<window2d>(fg);

  bool bg[] = { 0, 0, 0,
		1, 0, 1,
		0, 0, 0 };
  window2d win_bg = convert::to<window2d>(bg);

  border::thickness = 2;

  {
    image2d<bool> pic;
    io::pbm::load(pic, MLN_IMG_DIR "/picasso.pbm");
    io::pbm::save(morpho::thinning(pic, win_fg, win_bg), "out.pbm");
  }
  // FIXME: Dead code?
//   {
//     image2d<int_u8> pic = io::pgm::load(MLN_IMG_DIR "/picasso.pgm");
//     io::pgm::save( morpho::thinning(pic, win_fg, win_bg), "out.pgm" );
//   }
}
