// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file tests/linear/convolve_2x1d.cc
///
/// Tests on mln::linear::convolve_2x1d.

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/math/round.hh>
#include <mln/data/transform.hh>

#include <mln/core/alias/w_window2d_float.hh>
#include <mln/border/thickness.hh>
#include <mln/linear/convolve_2x1d.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 5;

  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");

  float
    w = 1.f / 11,
    v = 1.f / 3;
  float hws[] = { w, w, w, w, w,   w,   w, w, w, w, w };
  float vws[] = { v, v, v };
  image2d<float> tmp = linear::convolve_2x1d(lena, hws, vws);
  
  io::pgm::save(data::transform(tmp, math::round<int_u8>()),
		"out.pgm");
}
