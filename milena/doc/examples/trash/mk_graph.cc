// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file doc/examples/mk_graph.cc
///
/// This example shows how to create a graph from an influence zone image

# include <mln/essential/2d.hh>

# include <mln/core/site_set/p_vertices.hh>
# include <mln/core/site_set/p_edges.hh>

# include <mln/debug/colorize.hh>
# include <mln/debug/draw_graph.hh>

# include <mln/make/graph.hh>

# include <mln/transform/influence_zone_geodesic.hh>

# include <mln/util/graph.hh>


int main(int argc, char *argv[])
{
  using namespace mln;
  using value::int_u16;

  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <image.pbm>" << std::endl;
    return 1;
  }

  image2d<bool> ima = io::pbm::load(argv[1]);
  logical::not_inplace(ima);

  // Create a label image and compute the influence zones.
  int_u16 nlabels;
  image2d<int_u16> lbl = labeling::blobs(ima, c8(), nlabels);
  io::ppm::save(debug::colorize< image2d<value::rgb8> >(lbl, nlabels),
		"1_blobs.ppm");
  image2d<int_u16> iz = transform::influence_zone_geodesic(lbl,
							  c8(),
							  lbl.ncols());
  io::ppm::save(debug::colorize< image2d<value::rgb8> >(iz, nlabels),
		"2_influence_zone_geodesic.ppm");

  /// Make a graph from the influence zone image.
  util::graph g = make::graph(iz, nlabels);

  // Use the center of each bbox component as vertex in the graph.
  typedef fun::i2v::array<point2d> i2v_t;
  util::array<point2d> centers = labeling::compute(accu::center<point2d>(), iz, nlabels);
  i2v_t f = convert::to<i2v_t>(centers);
  p_vertices<util::graph, i2v_t> pv(g, f);

  // Find lines (sites) associated to edges.
  typedef fun::i2v::array<p_line2d> i2e_t;
  util::array<p_line2d> lines;
  mln_edge_iter_(util::graph) e(g);
  for_all(e)
    lines.append(p_line2d(f(e.v1()), f(e.v2())));
  p_edges<util::graph, i2e_t> pe(g, convert::to<i2e_t>(lines));

  // Draw graph.
  image2d<value::rgb8> out(ima.domain());
  debug::draw_graph(out, pv, literal::red, literal::green);
  io::ppm::save(out, "3_graph.ppm");
}
