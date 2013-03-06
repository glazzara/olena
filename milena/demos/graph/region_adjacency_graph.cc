// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

/// \file
///
/// Demo on using region adjacency graph.


#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/var.hh>

#include <mln/accu/center.hh>
#include <mln/accu/stat/mean.hh>

#include <mln/io/ppm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>

#include <mln/data/transform.hh>

#include <mln/labeling/wrap.hh>
#include <mln/labeling/mean_values.hh>

#include <mln/convert/to_fun.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/dilation.hh>

#include <mln/make/vertex_image.hh>
#include <mln/make/edge_image.hh>
#include <mln/make/region_adjacency_graph.hh>

#include <mln/math/diff_abs.hh>

#include <mln/debug/draw_graph.hh>
#include <mln/debug/filename.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_int_u.hh>

namespace mln
{

  /// Compute a distance between two rgb8 values.
  struct dist : Function_vv2v< dist >
  {

    typedef value::int_u8 result;

    result operator()(const value::rgb8& c1, const value::rgb8& c2) const
    {
      unsigned d = math::diff_abs(c1.red(), c2.red());
      unsigned d_;
      d_ = math::diff_abs(c1.green(), c2.green());

      if (d_ > d)
	d = d_;

      d_ = math::diff_abs(c1.blue(), c2.blue());

      if (d_ > d)
	d = d_;
      return d;
    }

  };

  /// Get the value associated to a given edge.
  template <typename I, typename V>
  struct edge_to_color : Function_v2v< edge_to_color<I,V> >
  {
    typedef V result;

    edge_to_color(const I& ima)
      : ima_(ima)
    {
    }

    V
    operator()(const unsigned& e) const
    {
      return convert::to<V>(ima_(e));
    }

    I ima_;
  };


  /// Draw a graph.
  template <typename I, typename V, typename E>
  inline
  image2d<mln_value(I)>
  make_debug_graph_image(const I& input,
			 const V& ima_v, const E& ima_e,
			 const value::rgb8& bg)
  {
    image2d<mln_value(I)> ima;
    initialize(ima, input);

    data::fill(ima, bg);
    debug::draw_graph(ima, ima_v.domain(),
		      pw::cst(mln_value(I)(literal::green)),
		      edge_to_color<E, mln_value(I)>(ima_e));

    /// Vertices are drawn with a box.
    dpoint2d tl(-3,-3);
    dpoint2d br(3,3);
    mln_piter(V) p(ima_v.domain());
    for_all(p)
      if (p.id() != 0)
      {
	box2d b(p + tl, p + br);
	b.crop_wrt(ima.domain());
	data::fill((ima | b).rw(), convert::to<mln_value(I)>(ima_v(p)));
      }

    return ima;
  }

}

int main(int argc, char *argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;
  using value::label_16;

  if (argc != 3 && argc != 4)
  {
    std::cout << "Usage: " << argv[0] << " <input.*> <output_prefix>"
	      << " [closing_area_value (default:25)]" << std::endl;
    return 1;
  }

  /// Load input image.
  image2d<rgb8> input_ppm;
  io::ppm::load(input_ppm, argv[1]);

  /// Convert input image to graylevel image.
  image2d<value::int_u8>
    input_pgm = data::transform(input_ppm, fun::v2v::rgb_to_int_u<8>());

  /// Set output filename prefix.
  debug::internal::filename_prefix = argv[2];

  unsigned area_value = 25;
  if (argc == 4)
    area_value = atoi(argv[3]);

  /// Gradient of the gray level image.
  image2d<int_u8> grad = morpho::gradient(input_pgm, win_c4p());
  io::pgm::save(grad, debug::filename("grad_c4p.pgm"));

  /// Closing of the gradient.
  image2d<int_u8> clo = morpho::closing::area(grad, c4(), area_value);
  io::pgm::save(clo, debug::filename("clo_a100.pgm"));

  /// Watershed on the closing image.
  label_16 nbasins;
  image2d<label_16> wshed = morpho::watershed::flooding(clo, c4(), nbasins);

  /// Output: A watershed image where each basin value is set to the basin
  /// mean value in the original ppm image.
  io::ppm::save(labeling::mean_values(input_ppm, wshed, nbasins),
		debug::filename("wshed_mean_colors.ppm"));


  /// Set watershed line value to yellow.
  data::fill((input_ppm | (pw::value(wshed) == 0u)).rw(), literal::yellow);
  io::ppm::save(input_ppm, debug::filename("wshed_color.ppm"));


  /// Build region adjacency graph.
  util::graph g = make::region_adjacency_graph(wshed, c4(), nbasins);


  /// Compute basin mass centers. They will be used as site for each vertex in
  /// the graph image.
  util::array<point2d>
    basin_centers = labeling::compute(accu::center<point2d, point2d>(),
				      wshed, nbasins);

  /// Compute basin mean values. They will be used as value for each vertex in
  /// the graph image.
  typedef accu::stat::mean<rgb8, mln_sum_(rgb8), rgb8>
	  accu_mean_t;
  util::array<rgb8>
    mean_values = labeling::compute(accu_mean_t(), input_ppm, wshed, nbasins);

  /// Construct a vertex image.
  mln_VAR(ima_v, make::vertex_image(g, basin_centers, mean_values));

  /// Construct an edge image.
  mln_VAR(ima_e, make::edge_image(ima_v, dist()));

  io::ppm::save(make_debug_graph_image(input_ppm, ima_v, ima_e, literal::white),
		debug::filename("wst_rag_graph_image_white.ppm"));
}
