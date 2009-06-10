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


#include <mln/data/fill.hh>
#include <mln/debug/println.hh>
#include <mln/core/alias/w_window2d_int.hh>
#include <mln/core/alias/w_window2d_float.hh>

#include <mln/make/win_chamfer.hh>
#include <mln/geom/chamfer.hh>
#include <mln/data/compare.hh>

#include <mln/opt/at.hh>


int main()
{
  using namespace mln;
  unsigned max = 51;

  image2d<bool> ima(9, 9);

  {
    data::fill(ima, false);
    opt::at(ima, 4,4) = true;
    const w_window2d_int& w_win = make::mk_chamfer_3x3_int<2, 0> ();
    image2d<unsigned> out = geom::chamfer(ima, w_win, max);
    unsigned r[9][9] =
      {
	{16, 14, 12, 10,  8, 10, 12, 14, 16},
	{14, 12, 10,  8,  6,  8, 10, 12, 14},
	{12, 10,  8,  6,  4,  6,  8, 10, 12},
	{10,  8,  6,  4,  2,  4,  6,  8, 10},
	{ 8,  6,  4,  2,  0,  2,  4,  6,  8},
	{10,  8,  6,  4,  2,  4,  6,  8, 10},
	{12, 10,  8,  6,  4,  6,  8, 10, 12},
	{14, 12, 10,  8,  6,  8, 10, 12, 14},
	{16, 14, 12, 10,  8, 10, 12, 14, 16}
      };

    image2d<unsigned> ref (make::image(r));
    mln_assertion (out == ref);
  }

  {
    data::fill(ima, false);
    opt::at(ima, 4,4) = true;
    const w_window2d_int& w_win = make::mk_chamfer_3x3_int<2, 3> ();
    image2d<unsigned> out = geom::chamfer(ima, w_win, max);

    unsigned r[9][9] =
      {
	{12, 11, 10, 9, 8, 9, 10, 11, 12},
	{11,  9,  8, 7, 6, 7,  8,  9, 11},
	{10,  8,  6, 5, 4, 5,  6,  8, 10},
	{ 9,  7,  5, 3, 2, 3,  5,  7,  9},
	{ 8,  6,  4, 2, 0, 2,  4,  6,  8},
	{ 9,  7,  5, 3, 2, 3,  5,  7,  9},
	{10,  8,  6, 5, 4, 5,  6,  8, 10},
	{11,  9,  8, 7, 6, 7,  8,  9, 11},
	{12, 11, 10, 9, 8, 9, 10, 11, 12}
      };

    image2d<unsigned> ref (make::image(r));
    mln_assertion (out == ref);
  }

  {
    data::fill(ima, false);
    opt::at(ima, 4,4) = true;
    const w_window2d_int& w_win = make::mk_chamfer_5x5_int<4, 6, 9> ();
    image2d<unsigned> out = geom::chamfer(ima, w_win, max);
    image2d<unsigned>::fwd_piter p(out.domain());
    for_all(p)
      out(p) = out(p) / 2;

    unsigned r[9][9] =
      {
	{12, 10, 9, 8, 8, 8, 9, 10, 12},
	{10,  9, 7, 6, 6, 6, 7,  9, 10},
	{ 9,  7, 6, 4, 4, 4, 6,  7,  9},
	{ 8,  6, 4, 3, 2, 3, 4,  6,  8},
	{ 8,  6, 4, 2, 0, 2, 4,  6,  8},
	{ 8,  6, 4, 3, 2, 3, 4,  6,  8},
	{ 9,  7, 6, 4, 4, 4, 6,  7,  9},
	{10,  9, 7, 6, 6, 6, 7,  9, 10},
	{12, 10, 9, 8, 8, 8, 9, 10, 12}
      };

    image2d<unsigned> ref (make::image(r));
    mln_assertion (out == ref);

  }

}
