// Copyright (C) 2008, 2009, 2013, 2014 EPITA Research and Development
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

/// \file
///
/// More tests on the Watershed Transform (WST) on a
/// an image base on a p_edges (line_graph_image).
/*!
    The scenario is as follows:
    \li load a 2-D, gray-level image from a PGM file;
    \li convert this 2-D image into a line graph-based one, where values
        on edges are computed as the absolute value f the difference
        between the values on the nodes	adjacent to the edge, so as to
        create a (norm of the gradient) ``between the pixels'' of the
        input image;
    \li perform a hierarchical flooding using an area opening, until a
        maximum number of regions has been reached;
    \li compute a watershed transform on this image;
    \li create a 2D image of this watershed image, where each basin has
        an average grey level of the corresponding region in the input
        image.

*/

#include <cstdlib>

#include <vector>

#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>

#include <mln/core/routine/duplicate.hh>

#include <mln/core/image/image2d.hh>

#include <mln/morpho/line_gradient.hh>
#include <mln/morpho/closing/area_on_vertices.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

int main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr
	<< "usage: " << argv[0] << " max_nregions input.pgm output.pgm"
	<< std::endl;
      std::exit(EXIT_FAILURE);
    }

  /*--------.
  | Input.  |
  `--------*/

  using namespace mln;
  using value::int_u8;
  using value::int_u16;

  typedef int_u8 val_t;
  typedef image2d<val_t> orig_ima_t;

  orig_ima_t input;
  io::pgm::load(input, argv[2]);
  if (!input.is_valid())
  {
    std::cerr << "Error reading input " << argv[2] << std::endl;
    std::exit(2);
  }

  /*----------------.
  | Line gradient.  |
  `----------------*/

  // Type of the function mapping graph edges and image sites.
  typedef util::site_pair<point2d> P;
  typedef fun::i2v::array<P> fedge_site_t;

  // Line graph image.
  typedef fun::i2v::array<val_t> fval_t;
  fval_t values;
  typedef edge_image<P,val_t> lg_ima_t;
  lg_ima_t lg_ima = morpho::line_gradient(input);

  /*-----------.
  | Flooding.  |
  `-----------*/

  /* FIXME: I'm not sure this is the way it should be done.  Anyway,
     we should implement this as a canvas.  */

  typedef lg_ima_t::nbh_t nbh_t;
  nbh_t nbh;

  unsigned area = 0;
  unsigned max_area = input.nsites();
  unsigned nregions = mln_max(unsigned);
  unsigned max_nregions = atoi(argv[1]);

  lg_ima_t result = duplicate(lg_ima);
  while (area < max_area && nregions > max_nregions)
    {
      ++area;
      std::cerr << "area = " << area << " \t"
		<< "nregions = " << nregions << std::endl;
      lg_ima_t work = duplicate(result);
      // Compute the closing.
      result = morpho::closing::area_on_vertices(work, nbh, area);
      // Compute the number of local minima (but get rid of the image,
      // as we don't need it).
      labeling::regional_minima(result, nbh, nregions);
    }

  /*------.
  | WST.  |
  `------*/

  // Perform a Watershed Transform.
  typedef int_u16 wst_val_t;
  wst_val_t nbasins;
  typedef edge_image<P,wst_val_t> wshed_t;

  wshed_t wshed = morpho::watershed::flooding(result, nbh, nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  /*---------.
  | Output.  |
  `---------*/

  const wst_val_t wshed_label = 0;

  // Create a 2D-equivalent of WSHED.
  image2d<wst_val_t> wshed2d(input.domain());

  /* FIXME: It'd better if we could iterate over the *vertices* of a
     line graph image.  We could avoid all this lengthy code.  */
  // Iterate over each edge of the watershed image, and propagate the
  // label of an edge to its adjacent vertices when this edge is not
  // part of the watershed.
  mln_piter_(wshed_t) p(wshed.domain());
  for_all(p)
    if (wshed(p) != wshed_label)
    {
      wshed2d(p.first()) = wshed(p);
      wshed2d(p.second()) = wshed(p);
    }

  // For each basin, compute the average gray level.
  std::vector<mln_sum_(val_t)> sum(nbasins + 1, 0);
  std::vector<unsigned> nsites(nbasins + 1, 0);
  mln_piter_(orig_ima_t) q(input.domain());
  for_all(q)
  {
    sum[wshed2d(q)] += input(q);
    ++nsites[wshed2d(q)];
  }
  std::vector<float> average(nbasins + 1);
  for (unsigned i = 1; i <= nbasins; ++i)
    average[i] = float (sum[i]) / float (nsites[i]);

  // Create an output image using the average gray levels of the basins.
  orig_ima_t output(input.domain());
  for_all(q)
    output(q) = convert::to<mln_value_(orig_ima_t)>(average[wshed2d(q)]);

  std::cout << "area = " << area << " \t"
	    << "nregions = " << nregions << std::endl;
  io::pgm::save(output, argv[3]);
}
