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

# include <cmath>
#include <mln/essential/2d.hh>
#include <mln/transform/influence_zone_geodesic.hh>
#include <mln/make/graph.hh>
#include <mln/util/graph.hh>
#include <mln/util/line_graph.hh>
#include <mln/debug/draw_graph.hh>
#include <mln/debug/colorize.hh>


template <typename P>
struct lg_vertex_values : public mln::Function_v2v< lg_vertex_values<P> >
{
  typedef float result;

  float operator()(const P& p) const
  {
    mln::algebra::vec<2,float> v;
    v[0] = 0;
    v[1] = 1;
    float norm = mln::math::sqrt(std::pow(p.to_vec()[0], 2)
				  + std::pow(p.to_vec()[1], 2));
    // FIXME: missing proxy_impl for point and line2d?
    float res = (v * p.to_vec()) / norm;
    return res;
  }

};



int main(int argc, char *argv[])
{
  using namespace mln;
  using value::label_16;
  using value::rgb8;

  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <image> <max cosinus>" << std::endl;
    return 1;
  }

  float max_cos = atof(argv[2]);

  image2d<bool> ima;
  io::pbm::load(ima, argv[1]);
  logical::not_inplace(ima);

  label_16 nlabels;
  image2d<label_16> lbl = labeling::blobs(ima, c8(), nlabels);

#ifndef NOUT
  io::ppm::save(debug::colorize(rgb8(), lbl, nlabels), "01-lbl.ppm");
#endif

  image2d<label_16> iz = transform::influence_zone_geodesic(lbl, c8(), mln_max(int));

#ifndef NOUT
  io::ppm::save(debug::colorize(rgb8(), iz, nlabels), "02-iz.ppm");
#endif

  typedef util::graph G;
  G g = make::graph(iz | (pw::value(iz) != pw::cst(0u)), nlabels);

  // Compute the component centers and use them as vertex.
  //FIXME: Add fun::vertex2p
  typedef fun::i2v::array<point2d> fv2p_t;
  util::array<point2d> centers = labeling::compute(accu::center<point2d>(), iz, nlabels);
  fv2p_t fv2p = convert::to<fv2p_t>(centers);

  // Create a p_vertices.
  p_vertices<G, fv2p_t> pv(g, fv2p);

#ifndef NOUT
  image2d<rgb8> gima = data::convert(rgb8(), ima);
  debug::draw_graph(gima,
		    pv,
		    pw::cst(literal::cyan),
		    pw::cst(literal::olive));
  io::ppm::save(gima, "03-graph.ppm");
#endif

  typedef util::line_graph<G> LG;
  LG lg(g);

  // Find lines (sites) associated to edges in g.
  typedef fun::i2v::array<p_line2d> i2e_t;
  util::array<p_line2d> lines;
  mln_edge_iter_(G) e(g);
  for_all(e)
    lines.append(p_line2d(fv2p(e.v1()), fv2p(e.v2())));

  // Map lines to vertices in lg.
  typedef p_vertices<LG, i2e_t> pvlg_t;
  pvlg_t pvlg(lg, convert::to<i2e_t>(lines));

#ifndef NOUT
  image2d<rgb8> lgima = data::convert(rgb8(), ima);
  debug::draw_graph(lgima,
		    pvlg,
		    pw::cst(literal::cyan),
		    pw::cst(literal::olive));
  io::ppm::save(lgima, "04-line-graph.ppm");
#endif

  // Construct an image from a p_edges and a function mapping
  // lines to angles.
  typedef lg_vertex_values<p_line2d> lgv2v_t;
  lgv2v_t lgv2v;

  mln_VAR(lg_ima, lgv2v | pvlg);


  // Colorize lg's vertices (i.e. g's edges) according their angle.
  //
  // literal::olive: cos(angle) < max_cos and cos(angle) > - max_cos
  // literal::red: cos(angle) > max_cos or cos(angle) < - max_cos
  typedef fun::i2v::array<value::rgb8> lgcolor_t;
  lgcolor_t ecolor(pvlg.nsites());
  mln_piter_(lg_ima_t) p(lg_ima.domain());
  for_all (p)
    if ((lg_ima(p) > max_cos) || (lg_ima(p) < - max_cos))
      ecolor(p) = literal::red;
    else
      ecolor(p) = literal::olive;


#ifndef NOUT
  image2d<rgb8> lgima2 = data::convert(rgb8(), ima);
  debug::draw_graph(lgima2, pvlg, pw::cst(literal::cyan), ecolor);
  io::ppm::save(lgima2, "05-line-graph-cleanup.ppm");
#endif
}
