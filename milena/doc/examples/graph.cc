// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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


#include <mln/essential/2d.hh>
#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/make/graph.hh>
#include <mln/util/graph.hh>
#include <mln/util/line_graph.hh>
#include <mln/debug/draw_graph.hh>
#include <mln/debug/colorize.hh>

template <typename G>
struct gcolor_t : public mln::Function< gcolor_t<G> >
{
  typedef mln::value::rgb8 result;

  mln::value::rgb8
  operator()(const mln::util::vertex<G>& v) const
  {
    return mln::literal::cyan;
  }

  mln::value::rgb8
  operator()(const mln::util::edge<G>& e) const
  {
    return mln::literal::magenta;
  }

};


int main(int, char *argv[])
{
  using namespace mln;
  using value::label16;
  using value::rgb8;

  image2d<bool> ima;
  io::pbm::load(ima, argv[1]);
  //logical::not_inplace(ima);

  label16 nlabels;
  image2d<label16> lbl = labeling::blobs(ima, c8(), nlabels);

#ifndef NOUT
  io::ppm::save(debug::colorize(rgb8(), lbl, nlabels), "01-lbl.ppm");
#endif

  image2d<label16> iz = transform::influence_zone_geodesic(lbl, c8(), mln_max(int));

#ifndef NOUT
  io::ppm::save(debug::colorize(rgb8(), iz, nlabels), "02-iz.ppm");
#endif

  util::graph g = make::graph(iz, nlabels);

  // Compute the components center and use them as vertex.
  //FIXME: Add fun::vertex2p
  typedef fun::i2v::array<point2d> fv2p_t;
  util::array<point2d> centers = labeling::compute(accu::center<point2d>(), iz, nlabels);
  fv2p_t fv2p = convert::to<fv2p_t>(centers);

  // Create a p_vertices.
  p_vertices<util::graph, fv2p_t> pv(g, fv2p);

#ifndef NOUT
  image2d<rgb8> gima = level::convert(rgb8(), ima);
  debug::draw_graph(gima, pv, gcolor_t<util::graph>());
  io::ppm::save(gima, "03-graph.ppm");
#endif

  typedef util::line_graph<util::graph> LG;
  LG lg(g);

  // Find lines (sites) associated to edges in g.
  typedef fun::i2v::array<p_line2d> i2e_t;
  util::array<p_line2d> lines;
  mln_edge_iter_(util::graph) e(g);
  for_all(e)
    lines.append(p_line2d(fv2p(e.v1()), fv2p(e.v2())));

  // Map lines to vertices in lg.
  p_vertices<LG, i2e_t> pvlg(lg, convert::to<i2e_t>(lines));
/*
#ifndef NOUT
  image2d<rgb8> lgima = level::convert(rgb8(), ima);
  debug::draw_graph(lgima, pvlg, gcolor_t<LG>());
  io::ppm::save(gima, "04-line-graph.ppm");
#endif
*/
}
