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
#include <mln/io/tiff/load.hh>

#include <mln/data/compare.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include "tests/data.hh"

#include <mln/debug/println.hh>

using mln::value::rgb8;
using mln::value::int_u8;

rgb8 ref_rgb[6][6] = { { rgb8(0,0,0), rgb8(255,255,255), rgb8(255,255,255), rgb8(255,255,255), rgb8(255,246,0), rgb8(0,0,0) },
		       { rgb8(255,255,255), rgb8(0,0,0), rgb8(255,255,255), rgb8(255,255,255), rgb8(255,255,255), rgb8(0,0,0) },
		       { rgb8(0,255,48), rgb8(255,255,255), rgb8(255,0,0), rgb8(255,255,255), rgb8(0,255,246), rgb8(0,0,0) },
		       { rgb8(255,255,255), rgb8(255,255,255), rgb8(255,255,255), rgb8(0,0,0), rgb8(255,255,255), rgb8(0,0,0) },
		       { rgb8(42,0,255), rgb8(255,255,255), rgb8(255,255,255), rgb8(255,255,255), rgb8(0,0,0), rgb8(0,0,0) },
		       { rgb8(0,0,0), rgb8(0,0,0), rgb8(0,0,0), rgb8(0,0,0), rgb8(0,0,0), rgb8(0,0,0) } };

int_u8 ref_gl[6][6] = { {   0, 255, 255, 255, 230, 0 },
			{ 255,	 0, 255, 255, 255, 0 },
			{ 186, 255,  54, 255, 200, 0 },
			{ 255, 255, 255,   0, 255, 0 },
			{  27, 255, 255, 255,	0, 0 },
			{   0,	 0,   0,   0,	0, 0 } };

bool ref_bw[6][6] = { { 0, 1, 1, 1, 1, 0 },
		      { 1, 0, 1, 1, 1, 0 },
		      { 1, 1, 0, 1, 1, 0 },
		      { 1, 1, 1, 0, 1, 0 },
		      { 0, 1, 1, 1, 0, 0 },
		      { 0, 0, 0, 0, 0, 0 } };


int main()
{
  using namespace mln;
  using namespace mln::value;

  /// Value: rgb8
  {
    image2d<rgb8> ref = make::image(ref_rgb);
    image2d<rgb8> pic;
    io::tiff::load(pic, MLN_TESTS_IMG_DIR "/test_rgb8.tif");
    mln_assertion(pic == ref);
  }

  {
    image2d<int_u8> ref = make::image(ref_gl);
    image2d<int_u8> pic;
    io::tiff::load(pic, MLN_TESTS_IMG_DIR "/test_gl.tif");
    mln_assertion(pic == ref);
  }

  {
    image2d<bool> ref = make::image(ref_bw);
    image2d<bool> pic;
    io::tiff::load(pic, MLN_TESTS_IMG_DIR "/test_bw.tif");
    mln_assertion(pic == ref);
  }

}
