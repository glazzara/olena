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

/// \file demos/genericity/neighborhood/duality.cc
///
/// Demo on dual neighborhoods.

#include <mln/core/image/image2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/flat_zones.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/save.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/make/dual_neighb.hh>



template <typename N>
void labelize(const mln::image2d<bool>& pic,
	      const N& nbh,
	      const std::string& filename)
{
  using namespace mln;
  using value::rgb8;
  unsigned n;
  image2d<unsigned> lab = labeling::flat_zones(pic, nbh, n);
  image2d<rgb8> out = labeling::colorize(rgb8(), lab, n);
  io::ppm::save(out, filename);
}



int main()
{
  using namespace mln;

  image2d<bool> pic = io::pbm::load("drawing.pbm");

  labelize(pic, make::dual_neighb(pic, c4(), c8()), "c4_c8.ppm");
  labelize(pic, make::dual_neighb(pic, c8(), c4()), "c8_c4.ppm");
  labelize(pic, c6_2d(), "c6_c6.ppm");
}
