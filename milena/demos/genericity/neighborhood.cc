// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file demos/genericity/neighborhood.cc
///
/// Test on mln::labeling::blobs.

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/labeling/blobs.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/flat_zones.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/save.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/make/dual_neighb.hh>
#include <mln/make/double_neighb2d.hh>
#include <mln/debug/println.hh>

#include "../../tests/data.hh"


template <typename N>
void run(const mln::image2d<bool>& pic,
	 const N& nbh,
	 const std::string& filename)
{
  using namespace mln;
  using value::rgb8;
  unsigned n;
  image2d<unsigned> lab = labeling::blobs(pic, nbh, n);
  image2d<rgb8> out = labeling::colorize(rgb8(), lab, n);
  io::ppm::save(out, filename);
}


int main()
{
  using namespace mln;

  image2d<bool> pic = io::pbm::load(MLN_IMG_DIR "/small.pbm");

  run(pic, c4(), "c4.ppm");
  run(pic, c8(), "c8.ppm");

  run(pic, c6_2d(), "c6.ppm");

  {
    bool vert[] = { 0, 1, 0,
		    0, 0, 0,
		    0, 1, 0 };

    bool hori[] = { 0, 0, 0,
		    1, 0, 1,
		    0, 0, 0 };
    run(pic,
	make::double_neighb2d(fun::p2b::chess(), vert, hori),
	"cmy.ppm");
  }

  {
    using value::rgb8;
    unsigned n;
    image2d<unsigned> lab;
    lab = labeling::flat_zones(pic,
			       make::dual_neighb(pic,
						 c4(),  // object
						 c8()), // background
			       n);
    image2d<rgb8> out = labeling::colorize(rgb8(), lab, n);
    io::ppm::save(out, "cdual48.ppm");
  }

}
