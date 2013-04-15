// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory
// (LRDE)
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
#include <mln/core/alias/neighb2d.hh>

#include <mln/literal/colors.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u16.hh>

#include <mln/labeling/wrap.hh>
#include <mln/labeling/colorize.hh>
#include <mln/labeling/blobs.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/transform/distance_and_influence_zone_geodesic.hh>
#include <mln/core/var.hh>

#include <mln/win/hline2d.hh>
#include <mln/win/disk2d.hh>

#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/watershed/superpose.hh>
#include <mln/morpho/closing/structural.hh>

#include <scribo/make/debug_filename.hh>


int main(int argc, char *argv[])
{
  using namespace mln;

  using value::int_u16;
  using value::label_16;
  using value::label_8;
  using value::rgb8;

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " <input.pbm>" << std::endl;
    return 1;
  }

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  scribo::make::internal::debug_filename_prefix = "morpho";

  label_16 nlabels;
  image2d<label_16> lbl = labeling::blobs(input, c8(), nlabels);

  mln_VAR(res, transform::distance_and_influence_zone_geodesic(lbl, c8(), mln_max(unsigned)));

  io::pgm::save(labeling::wrap(res.first()), "dmap.pgm");
  io::ppm::save(labeling::colorize(rgb8(), res.second(), nlabels), "iz.ppm");

  image2d<unsigned>& dmap = res.first();
  {
    image2d<unsigned> clo = morpho::closing::structural(dmap, win::hline2d(51));
    io::pgm::save(clo, scribo::make::debug_filename("clo_line_51.pgm"));
    label_16 nlabels;
    image2d<label_16> wsd = morpho::watershed::flooding(clo, c8(), nlabels);

    io::ppm::save(morpho::watershed::superpose(input, wsd),
		  scribo::make::debug_filename("wsd_line_51.ppm"));
  }

  {
    image2d<unsigned> clo = morpho::closing::structural(dmap, win::rectangle2d(11, 101));
    io::pgm::save(clo, scribo::make::debug_filename("clo_rectangle_11_101.pgm"));
    label_16 nlabels;
    image2d<label_16> wsd = morpho::watershed::flooding(clo, c8(), nlabels);

    io::ppm::save(morpho::watershed::superpose(input, wsd),
		  scribo::make::debug_filename("wsd_rectangle_11_101.ppm"));
  }

  {
    image2d<unsigned> clo = morpho::closing::structural(dmap, win::disk2d(51));
    io::pgm::save(clo, scribo::make::debug_filename("clo_disk_51.pgm"));
    label_16 nlabels;
    image2d<label_16> wsd = morpho::watershed::flooding(clo, c8(), nlabels);

    io::ppm::save(morpho::watershed::superpose(input, wsd),
		  scribo::make::debug_filename("wsd_disk_51.ppm"));
  }

}
