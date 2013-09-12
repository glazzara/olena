// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory (LRDE)
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
#include <mln/core/var.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/data/convert.hh>
#include <mln/data/fill.hh>
#include <mln/debug/int2rgb.hh>
#include <mln/display/display_region.hh>
#include <mln/literal/colors.hh>
#include <mln/math/diff_abs.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/display_edge.hh>
#include <mln/world/inter_pixel/display_region.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/is_pixel.hh>
#include <mln/world/inter_pixel/is_separator.hh>
#include <mln/world/inter_pixel/neighb2d.hh>


using namespace mln;
using value::int_u8;
using value::label_16;
using value::rgb8;


struct dist_t : Function_vv2v<dist_t>
{
  typedef int_u8 result;

  template <typename V>
  int_u8 operator()(const V v1, const V v2) const
  {
    return math::diff_abs(v1, v2);
  }
} dist;



int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input.pgm" << std::endl;
    return 1;
  }

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  // Gradient.
  image2d<int_u8> grad = morpho::elementary::gradient(input, c4());
  io::pgm::save(grad, "gradient.pgm");

  label_16 nbasins;

  // Normal watershed.
  image2d<label_16> normal_wst = morpho::watershed::flooding(input, c4(), nbasins);
  io::ppm::save(display::display_region(input, normal_wst, literal::red), "normal_watershed.ppm");



  // Distance on edges.
  typedef image_if<image2d<int_u8>, world::inter_pixel::is_pixel> Ix;
  Ix imax = world::inter_pixel::immerse(input);
  image_if<image2d<int_u8>, world::inter_pixel::is_separator> edges;
  edges = world::inter_pixel::compute(imax, dist);

  io::pgm::save(world::inter_pixel::display_edge(edges.unmorph_(), 0, 3), "edge_dist.pgm");

  // Color on edge image.
  image2d<rgb8> edge_color = data::convert(rgb8(), edges.unmorph_());
  mln_piter_(image2d<int_u8>) p(input.domain());
  for_all(p)
    convert::from_to(input(p), opt::at(edge_color, p.row() * 2, p.col() * 2));
  data::fill((edge_color | world::inter_pixel::is_separator()).rw(), literal::green);
  data::fill((edge_color | world::inter_pixel::is_zero_face()).rw(), literal::blue);
  io::ppm::save(edge_color, "edge_color.ppm");

  // Color on stretched edge image.
  box2d b = edge_color.bbox();
  image2d<rgb8> edge_stretch(make::box2d(((b.pmin()[0] + 1) / 2) * 4,         ((b.pmin()[1] + 1) / 2) * 4,
					 ((b.pmax()[0] + 1) / 2 + 1) * 4 - 2, ((b.pmax()[1] + 1) / 2 + 1) * 4 - 2));
  typedef image_if<image2d<rgb8>, world::inter_pixel::is_separator> edge_t;
  edge_t edge_sep = edge_color | world::inter_pixel::is_separator();
  mln_piter_(edge_t) q(edge_sep.domain());
  for_all(q)
    if (q.row() % 2) // horizontal edge
    {
      unsigned row = (q.row() / 2 + 1) * 4 - 1;
      unsigned col = (q.col() / 2) * 4;
      for (unsigned i = 0; i < 3; ++i)
	opt::at(edge_stretch, row, col + i) = literal::green;
    }
    else // vertical edge
    {
      unsigned row = (q.row() / 2) * 4;
      unsigned col = (q.col() / 2 + 1) * 4 - 1;
      for (unsigned i = 0; i < 3; ++i)
	opt::at(edge_stretch, row + i, col) = literal::green;
    }
  mln_VAR(edge_zero, edge_color | world::inter_pixel::is_zero_face());
  mln_piter_(edge_zero_t) ez(edge_zero.domain());
  for_all(ez)
    opt::at(edge_stretch, ((ez.row() + 1) / 2) * 4 - 1, ((ez.col() + 1) / 2) * 4 - 1) = literal::blue;
  mln_VAR(edge_pix, edge_color | world::inter_pixel::is_pixel());
  mln_piter_(edge_pix_t) ex(edge_pix.domain());
  for_all(ex)
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j)
	opt::at(edge_stretch, (ex.row() / 2) * 4 + i, (ex.col() / 2) * 4 + j) = edge_color(ex);
  io::ppm::save(edge_stretch, "edge_stretch.ppm");


  // On edges watershed.
  mln_VAR(edge_clo, morpho::closing::volume(edges, world::inter_pixel::e2e(), 25));
  mln_VAR(edge_wst, morpho::watershed::flooding(edge_clo, world::inter_pixel::e2e(), nbasins));
  io::ppm::save(world::inter_pixel::display_region(input, edge_wst.unmorph_(), literal::red), "edge_watershed.ppm");
}
