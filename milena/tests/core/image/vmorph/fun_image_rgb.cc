// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE).
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
/// \brief Exercise mln::fun_image with RGB component functions.

#include <mln/core/image/image2d.hh>
#include <mln/core/image/vmorph/fun_image.hh>

#include <mln/value/rgb8.hh>
#include <mln/fun/v2v/component.hh>

#include <mln/io/ppm/load.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;

  typedef value::rgb8 V;
  typedef image2d<V> I;
  I lena;
  io::ppm::load(lena, MLN_IMG_DIR "/tiny.ppm");

  // Red (#0) component.
  typedef fun::v2v::component<V, 0> FR;
  FR red;
  fun_image<FR, I> lena_red = red << lena;

  // Green (#1) component.
  typedef fun::v2v::component<V, 1> FG;
  FG green;
  fun_image<FG, I> lena_green = green << lena;

  // Blue (#2) component.
  typedef fun::v2v::component<V, 2> FB;
  FB blue;
  fun_image<FB, I> lena_blue = blue << lena;

  mln_piter_(I) p(lena.domain());
  for_all(p)
  {
    V v(lena_red(p), lena_green(p), lena_blue(p));
    mln_assertion(lena(p) == v);
  }
}
