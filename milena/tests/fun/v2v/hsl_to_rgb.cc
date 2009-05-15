// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file tests/fun/v2v/hsl_to_rgb.cc
///
/// Test HSL-to-RGB conversion.

#include <mln/core/image/image2d.hh>

#include <mln/fun/v2v/hsl_to_rgb.hh>

#include <mln/level/compare.hh>
#include <mln/level/transform.hh>

#include <mln/value/hsl.hh>
#include <mln/value/rgb8.hh>

#include <mln/debug/println.hh>

using mln::value::rgb8;
using mln::value::hsl_f;

rgb8 ref[][2] = { { rgb8(0,0,0), rgb8(255,255,255) },
		  { rgb8(128,128,128), rgb8(90,90, 90) } };

hsl_f dat[][2] = { { hsl_f(0,0,0), hsl_f(0,0,255) },
		   { hsl_f(0,0,128), hsl_f(0,0,90) } };

int main()
{
  using namespace mln;

  image2d<rgb8> ref_ima = make::image(ref);
  image2d<hsl_f> ima = make::image(dat);

  image2d<value::rgb8> ima_rgb = level::transform(ima,
						  fun::v2v::f_hsl_to_rgb_3x8);

  mln_assertion(ima_rgb == ref_ima);
}

