// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

// FIXME: Update.
/** \brief More tests on the Watershed Transform (WST) on a
    mln::line_graph_image.

    The scenario is as follows:
    \li load a 2-D, gray-level image from a PGM file;
    \li convert this 2-D image into a line graph-based one, where values
        on edges are computed as the absolute value of the difference
        between the values on the vertices adjacent to the edge, so as to
        create a (norm of the) gradient ``between the pixels'' of the
        input image;
    \li reduce the number of minima using an area opening (counting the
        vertices to compute the area, not the edges);
    \li perform a WST on this simplified line graph image;
    \li create a 2-D, color output image with height and width double
        the size the original one, and copy the data of the input image
        in it, interpolating inter-pixel points;
    \li print the watershed on lines into that same image, and save it.  */

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/neighb2d.hh>

/// Required for line graph images.
#include <mln/core/site_set/p_edges.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>
#include <mln/core/var.hh>
#include <mln/pw/all.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/util/graph.hh>

#include <mln/morpho/line_gradient.hh>
#include <mln/morpho/closing_area_on_vertices.hh>
#include <mln/morpho/meyer_wst.hh>
#include <mln/data/stretch.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/black.hh>
#include <mln/literal/colors.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/math/max.hh>
#include <mln/math/abs.hh>


int main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "usage: " << argv[0] << " input.pgm lambda output.pgm"
		<< std::endl;
      std::exit(1);
    }
  std::string input_filename = argv[1];
  unsigned lambda = atoi(argv[2]);
  std::string output_filename = argv[3];


  using namespace mln;
  using value::int_u8;
  using value::int_u16;
  using value::rgb8;

  /*--------.
  | Input.  |
  `--------*/

  typedef int_u8 input_val_t;
  image2d<input_val_t> input;
  io::pgm::load(input, input_filename);

  /*----------------.
  | Line gradient.  |
  `----------------*/

  // Line graph image.
  mln_VAR(lg_ima, morpho::line_gradient(input));

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  typedef line_graph_elt_neighborhood<util::graph, lg_ima_t::pset::fun_t> nbh_t;
  nbh_t nbh;

  mln_VAR(closed_lg_ima, morpho::closing_area_on_vertices(lg_ima, nbh, lambda));

  /*---------.
  | Output.  |
  `---------*/

  // FIXME: Inlined conversion, to be reifed into a routine.

  // Data.
  typedef int_u8 output_val_t;
  typedef image2d<output_val_t> output_t;
  point2d output_pmin = input.domain().pmin();
  point2d output_pmax(input.domain().pmax()[0] * 2,
		      input.domain().pmax()[1] * 2);
  output_t output(box2d(output_pmin, output_pmax));
  data::fill(output, 0);
  // Draw the filtered gradient.
  mln_piter_(closed_lg_ima_t) p(closed_lg_ima.domain());
  for_all(p)
  {
    mln_psite_(lg_ima_t) pp(p);
    // Equivalent of the line (edge) PP in OUTPUT.
    int row1 = pp.first()[0] * 2;
    int col1 = pp.first()[1] * 2;
    int row2 = pp.second()[0] * 2;
    int col2 = pp.second()[1] * 2;
    point2d q((row1 + row2) / 2, (col1 + col2) / 2);
    // Print the watershed in red.
    output(q) = closed_lg_ima(p);
  }
  io::pgm::save(output, output_filename);
}
