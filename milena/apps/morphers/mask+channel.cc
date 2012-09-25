// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Example projecting a RGB image to the green channel and
/// then applying a domain restricting with a mask.

#include <sstream>
#include <iomanip>

#include <string>

#include <mln/core/image/image2d.hh>
#include <mln/make/box2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/value/rgb8.hh>

#include <mln/fun/component/green.hh>
/* FIXME: We wanted to use `fun_image' and `operator<<' from
   <mln/core/image/vmorph/fun_image.hh, but they only create read-only
   images.  Use `thru_image' instead.  */
#include <mln/core/image/vmorph/thru_image.hh>

#include <mln/core/routine/duplicate.hh>

#include <mln/literal/colors.hh>
#include <mln/data/fill.hh>

#include <mln/io/ppm/all.hh>

#include "apps/data.hh"


int main()
{
  using namespace mln;
  using mln::value::rgb8;

  image2d<rgb8> lena = io::ppm::load<rgb8>(MLN_IMG_DIR "/tiny.ppm");
  fun::green green;
  data::fill((thru(green, lena).rw() | make::box2d(5,5, 10,10)).rw(),
	     255);
  io::ppm::save(lena, "lena-mask-channel.ppm");
}
