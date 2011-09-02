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

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pbms/load.hh>

#include <mln/data/compare.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;

  image2d<bool> pic = io::pbm::load(MLN_IMG_DIR "/tiny.pbm");
  image2d<bool> pic2 = duplicate(pic);

  pic2(point2d(0,0)) = true;
  io::pbm::save(pic2, "pbms-out.pbm");

  util::array<std::string> files(2);
  files[0] = MLN_IMG_DIR "/tiny.pbm";
  files[1] = "pbms-out.pbm";

  image3d<bool> ima3d;
  io::pbms::load(ima3d, files);

  mln_assertion(ima3d.nslis() == 2);
  mln_assertion(slice(ima3d, 0) == pic);
  mln_assertion(slice(ima3d, 1) == pic2);

}
