// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file tests/io/pbm/pbm.cc
///
/// Test on mln::io::pbm::.

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>

#include <mln/data/compare.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;

  image2d<bool> pic = io::pbm::load(MLN_IMG_DIR "/picasso.pbm");
  io::pbm::save(pic, "out.pbm");
  image2d<bool> pic2 = io::pbm::load("out.pbm");

  mln_assertion(pic == pic2);


  pic2(point2d(0,0)) = true;
  io::pbm::save(pic2, "out.pbm");

  util::array<std::string> files(2);
  files[0] = MLN_IMG_DIR "/picasso.pbm";
  files[1] = "out.pbm";

  image3d<bool> ima3d;
  io::pbm::load(ima3d, files);

  mln_assertion(ima3d.nslices() == 2);
  mln_assertion(slice(ima3d, 0) == pic);
  mln_assertion(slice(ima3d, 1) == pic2);

}


// sample binary image to test with xv, imview, and display
// to bring into the fore the binary image bug with raw pbm.

// | | | | -
// | | - - -
// | - - - -
// - - - - -
// - - - - -
// - - - - -
