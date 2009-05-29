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

/// \file tests/io/magick/save.cc
///
/// Test on mln::io::magick::save.

#include <mln/core/image/image2d.hh>
#include <mln/io/magick/load.hh>
#include <mln/io/magick/save.hh>
#include <mln/io/ppm/load.hh>

#include <mln/data/compare.hh>

#include <mln/io/magick/load.hh>

#include "tests/data.hh"

int main()
{
  using namespace mln;

  image2d<value::rgb8> lena_mln;
  io::ppm::load(lena_mln, MLN_IMG_DIR "/tiny.ppm");

  io::magick::save(lena_mln, "tiny.png");

  image2d<value::rgb8> lena_im;
  io::magick::load(lena_im, "tiny.png");

  mln_assertion(lena_im.domain() == lena_mln.domain());
  mln_assertion(lena_im == lena_mln);

}

