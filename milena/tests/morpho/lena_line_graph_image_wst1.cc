// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

/* FIXME: We should factor as much things as possible between
   tests/morpho/lena_line_graph_image_wst1.cc and
   tests/morpho/lena_line_graph_image_wst2.cc, starting from conversion
   routines.  */

/** \file
    \brief Tests on the Watershed Transform (WST) on a mln::line_graph_image.
   
    The scenario is as follows:
    \li load a 2-D, gray-level image from a PGM file;
    \li compute a morphological gradient of this image;
    \li simplify the image to reduce the number of local minima;
    \li convert this 2-D image into a line graph-based one, where values
        on edges are computed as the maxmimum of the values on the vertices
	adjacent to the edge;
    \li perform a WST on the line graph image;
    \li create an 2-D, color output image with height and width double
        the size the original one, and copy the data of the input image
	in it, interpolating inter-pixel points;
    \li print the watershed on lines into that same image, and save it.  */

#include <map>
#include <vector>

#include <mln/util/ord.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/convert/to_window.hh>

#include <mln/util/graph.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/site_pair.hh>
#include <mln/core/image/edge_image.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/black.hh>
#include <mln/literal/colors.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/math/max.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  /*--------.
  | Input.  |
  `--------*/

  typedef int_u8 input_val_t;
  image2d<input_val_t> input;
  io::pgm::load(input, MLN_IMG_DIR "/tiny.pgm");

  // In this test, the gradient is directly computed on the input
  // image, not on the edges of the line graph image.
  image2d<input_val_t> gradient =
    morpho::gradient (input, convert::to_window(c4()));

  // Simplify the input image.
  image2d<input_val_t> work(input.domain());
  work = morpho::closing::area(gradient, c4(), 10);

  /*-------------.
  | Line graph.  |
  `-------------*/

  // FIXME: Inlined conversion, to be reifed into a routine.

  util::graph g;

  // Points.
  image2d<unsigned> equiv_vertex;
  initialize(equiv_vertex, work);

  // Vertices.
  mln_fwd_piter_(image2d<input_val_t>) p(work.domain());
  for_all(p)
    equiv_vertex(p) = g.add_vertex();

  // Edges.
  window2d next_c4_win;
  next_c4_win.insert(0, 1).insert(1, 0);
  typedef fun::i2v::array<int> edge_values_t;
  typedef fun::i2v::array< util::site_pair<point2d> > edge_sites_t;
  edge_values_t edge_values;
  edge_sites_t edge_sites;
  mln_fwd_qiter_(window2d) q(next_c4_win, p);
  for_all (p)
    for_all(q)
      if (work.domain().has(q))
      {
        g.add_edge(equiv_vertex(p), equiv_vertex(q));
	edge_values.append(math::max(work(p), work(q)));
	edge_sites.append(util::site_pair<point2d>(p, q));
      }

  // Line graph point set.
  typedef edge_image<util::site_pair<point2d>,int,util::graph> lg_ima_t;
  lg_ima_t lg_ima(g, edge_sites, edge_values);

  /*------.
  | WST.  |
  `------*/

  // Elementary neighborhood of an edge.
  typedef lg_ima_t::nbh_t nbh_t;
  nbh_t nbh;

  // Perform a Watershed Transform.
  int_u8 nbasins;
  typedef edge_image<util::site_pair<point2d>,int_u8,util::graph> wshed_t;
  wshed_t wshed = morpho::watershed::flooding(lg_ima, nbh, nbasins);
  mln_assertion(nbasins == 5u);

  /*---------.
  | Output.  |
  `---------*/

  // FIXME: Inlined conversion, to be reifed into a routine.

  // Save the result in gray levels (data) + color (wshed).

  // Data.
  typedef rgb8 output_val_t;
  typedef image2d<output_val_t> output_t;
  point2d output_pmin = input.domain().pmin();
  point2d output_pmax(input.domain().pmax()[0] * 2,
		      input.domain().pmax()[1] * 2);
  output_t output(box2d(output_pmin, output_pmax));
  data::fill(output, literal::black);
  // Reuse the piter on INPUT.
  for_all(p)
  {
    // Equivalent of P in OUTPUT.
    point2d q(p[0] * 2, p[1] * 2);
    input_val_t v = input(p);
    /* FIXME: Use a conversion function from input_val_t to
       output_val_t instead of an explicit construction.  */
    output(q) = output_val_t(v, v, v);
  }
  // Interpolate missing points in OUTPUT.
  mln_piter_(output_t) p_out(output.domain());
  for_all(p_out)
  {
    // Process points on even rows and odd columns
    if (p_out[0] % 2 == 0 && p_out[1] % 2 == 1)
      output(p_out) = (output(p_out + left) + output(p_out + right)) / 2;
    // Process points on odd rows and even columns
    if (p_out[0] % 2 == 1 && p_out[1] % 2 == 0)
      output(p_out) = (output(p_out + up) + output(p_out + down)) / 2;
    // Process points on odd rows and odd columns
    if (p_out[0] % 2 == 1 && p_out[1] % 2 == 1)
      output(p_out) =
	(output(p_out + dpoint2d(-1, -1)) +
	 output(p_out + dpoint2d(-1, +1)) +
	 output(p_out + dpoint2d(+1, -1)) +
	 output(p_out + dpoint2d(+1, +1))) / 4;
  }
  // Draw the watershed.
  /* FIXME: We should draw the watershed on another image and
     superimpose it on OUTPUT instead of drawing it directly into
     OUTPUT.  */
  mln_piter_(wshed_t) pw(wshed.domain());
  for_all(pw)
  {
    if (wshed(pw) == 0u)
      {
	mln_psite_(lg_ima_t) pp(pw);
	// Equivalent of the line (edge) PP in OUTPUT.
	int row1 = pp.first()[0] * 2;
	int col1 = pp.first()[1] * 2;
	int row2 = pp.second()[0] * 2;
	int col2 = pp.second()[1] * 2;
	point2d q((row1 + row2) / 2, (col1 + col2) / 2);
	// Print the watershed in red.
	output(q) = literal::red;
      }
  }
  // Fill the holes, so that the watershed looks connected.
  /* FIXME: This approach is bad: it creates thick lines of watershed.
     We should probably solve this when we ``paint'' the watershed
     over the ``doubled'' image.

     A better approach is probably to iterate over the set of vertices,
     and ``connect'' edges according to patterns (vertically or
     horizontally connected egdes member of the watershed, etc.).  */
  // Reuse the piter on OUTPUT.
  for_all (p_out)
    // Only handle points on odd rows and columns.
    if (p_out[0] % 2 == 1 && p_out[1] % 2 == 1)
  {
    // Count the number of adjacent watershed points.  If there are
    // two or more, consider, create a watershed point.
    /* FIXME: Iterating over a c4 window would be more elegant, of
       course.  */
    unsigned nwsheds = 
      (output.has(p_out + up   ) && output(p_out + up   ) == literal::red) +
      (output.has(p_out + down ) && output(p_out + down ) == literal::red) +
      (output.has(p_out + left ) && output(p_out + right) == literal::red) +
      (output.has(p_out + right) && output(p_out + left ) == literal::red);
    if (nwsheds >= 2)
      output(p_out) = literal::red;      
  }
  io::ppm::save(output, "lena_line_graph_image_wst1-out.ppm");
}
