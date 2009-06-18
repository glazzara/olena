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

/// \file demos/genericity/neighborhood/permissive.cc
///
/// Demo on user-defined simple neighborhood.

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>

#include <mln/make/neighb2d.hh>
#include <mln/labeling/blobs.hh>

#include <mln/io/ppm/save.hh>
#include <mln/labeling/colorize.hh>
#include <mln/value/rgb8.hh>

#include <mln/fun/p2p/fold.hh>
#include <mln/core/image/dmorph/transformed_image.hh>



int main()
{
  using namespace mln;

  image2d<bool> world = io::pbm::load("world.pbm");

  bool large[] = { 1, 1, 1, 1, 1,
		   1, 1, 1, 1, 1,
		   1, 1, 0, 1, 1,
		   1, 1, 1, 1, 1,
		   1, 1, 1, 1, 1 };
  neighb2d nbh = make::neighb2d(large);

  unsigned n;
  image2d<unsigned> labeled = labeling::blobs(world, nbh, n);

  io::ppm::save(labeling::colorize(value::rgb8(), labeled, n),
		"world.ppm");


  fun::p2p::fold<point2d,0,1> f(world.domain());
  labeled = labeling::blobs(transform_domain(world, f), nbh, n).unmorph_();

  io::ppm::save(labeling::colorize(value::rgb8(), labeled, n),
		"world_bis.ppm");
}
