// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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
#include <mln/core/image/image3d.hh>
#include <mln/core/image/dmorph/slice_image.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/value/rgb8.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/ppms/load.hh>

#include <mln/data/compare.hh>

#include <mln/util/array.hh>

#include <mln/literal/colors.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::rgb8;

  image2d<rgb8> ima = io::ppm::load<rgb8>(MLN_IMG_DIR "/fly.ppm");
  io::ppm::save(ima, "load-out.ppm");
  image2d<rgb8> ima2 = duplicate(ima);


  ima2(point2d(0,0)) = literal::green;
  io::ppm::save(ima2, "load-out.ppm");

  util::array<std::string> files(2);
  files[0] = MLN_IMG_DIR "/fly.ppm";
  files[1] = "load-out.ppm";

  image3d<rgb8> ima3d;
  io::ppms::load(ima3d, files);

  mln_assertion(ima3d.nslis() == 2);
  mln_assertion(slice(ima3d, 0) == ima);
  mln_assertion(slice(ima3d, 1) == ima2);
}
