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

/// \file

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/literal/colors.hh>
#include <mln/opt/at.hh>

#include <tests/data.hh>
#include <doc/tools/sample_utils.hh>

int main()
{
  using namespace mln;

  // \{
  image2d<value::rgb8> ima(40, 40);
  // \}

  // \{
  data::fill(ima, literal::red);
  // \}

  // \{
  for (def::coord row = 20; row < 30; ++row)
    for (def::coord col = 20; col < 30; ++col)
      ima(point2d(row, col)) = literal::blue;
  // \}

  // \{
  for (def::coord row = 20; row < 30; ++row)
    for (def::coord col = 20; col < 30; ++col)
      opt::at(ima, row, col) = literal::blue;
  // \}
  doc::ppmsave(ima, "tuto3_rw_image");

  image2d<value::rgb8> ima2 = duplicate(ima);

  // \{
  image2d<value::rgb8> lena;
  io::ppm::load(lena, MLN_IMG_DIR "/small.ppm");
  // \}

  // \{
  data::fill(ima, lena);
  // \}
  doc::ppmsave(ima, "tuto3_rw_image");

  ima = ima2;
  // \{
  data::paste(ima, lena);
  // \}
  doc::ppmsave(lena, "tuto3_rw_image");
}
