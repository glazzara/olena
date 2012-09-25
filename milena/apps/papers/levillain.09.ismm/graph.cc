// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <iostream>

#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/blobs.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/data/stretch.hh>
#include <mln/debug/println.hh>
#include <mln/draw/line.hh>
#include <mln/pw/all.hh>
#include <mln/binarization/threshold.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/core/alias/point2d.hh>

#include <mln/core/site_set/p_faces.hh>
#include <mln/core/image/complex_image.hh>
#include <mln/core/alias/complex_geometry.hh>
#include <mln/core/alias/complex_image.hh>

// FIXME: Include these elsewhere? (In complex_image.hh?)
#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/complex_neighborhood_piter.hh>
#include <mln/core/image/complex_windows.hh>

#include <mln/data/fill.hh>
#include <mln/morpho/gradient.hh>
#include <mln/labeling/colorize.hh>

#include "io.hh"
#include "influence_zones.hh"
#include "chain.hh"


// FIXME: Clean up and move into Milena?
mln::int_u8_1complex_image2d
make_complex_image(const mln::image2d<mln::value::int_u8>& input)
{
  using namespace mln;
  using mln::value::int_u8;

  /*----------------------------------------.
  | Complex + complex geometry (location).  |
  `----------------------------------------*/

  border::thickness = 0;

  unsigned nlabels;
  image2d<unsigned> label =
    labeling::blobs(mln::binarization::threshold(input, 1), c4(), nlabels);

  std::cout << "n seeds = " << nlabels << std::endl;
  {
    image2d<int_u8> lab(label.domain());
    data::paste(label, lab);
  }

  image2d<unsigned> iz = influence_zones(label, c4());
  {
    image2d<int_u8> IZ(iz.domain());
    data::paste(iz, IZ);
  }

#if 0
  io::pgm::save (data::stretch(mln::value::int_u8(), iz), "iz.pgm");
#endif

  // Make graph/complex.

  std::vector< std::vector<bool> > adj(nlabels + 1);
  for (unsigned l = 1; l <= nlabels; ++l)
    adj[l].resize(nlabels + 1, false);

  {
    box2d::piter p(iz.domain());
    for_all(p)
    {
      point2d r = p + right, b = p + down;
      if (iz.has(r) && iz(p) != iz(r))
	{
	  if (iz(p) <= iz(r))
	    adj[iz(p)][iz(r)] = true;
	  else
	    adj[iz(r)][iz(p)] = true;
	}
      if (iz.has(b) && iz(p) != iz(b))
	{
	  if (iz(p) <= iz(b))
	    adj[iz(p)][iz(b)] = true;
	  else
	    adj[iz(b)][iz(p)] = true;
	}
    }
  }

  // end of mk_graph


  const unsigned D = 1;

  topo::complex<D> c;

  typedef point2d P;
  typedef geom::complex_geometry<D, P> G;
  G geom;

  // Convenience typedefs.
  typedef topo::n_face<0, D> vertex;
  typedef topo::n_face<1, D> edge;

  {

    // 0-faces (vertices).
    std::vector<vertex> v;
    {
      box2d::piter p(label.domain());
      for_all(p)
	if (label(p) != 0)
	  {
	    geom.add_location(p);
	    v.push_back(c.add_face());
	  }
    }

    std::cout << "v size = " << v.size() << std::endl;

    // 1-faces (edges).
    std::vector<edge> e;
    {
      for (unsigned l = 1; l <= nlabels; ++l)
	for (unsigned ll = l + 1; ll <= nlabels; ++ll)
	  if (adj[l][ll])
	    e.push_back( c.add_face(-v[l-1] + v[ll-1]) );
    }

    std::cout << "e size = " << e.size() << std::endl;

  }


  /*---------------------.
  | Complex-based pset.  |
  `---------------------*/

  p_complex<D, G> pc(c, geom);

  /*----------------------.
  | Complex-based image.  |
  `----------------------*/

  // An image type built on a 1-complex with unsigned values on each
  // face (both vertices and edges).
  typedef complex_image<D, G, int_u8> output_t;

  // Create and initialize an image based on PC.
  output_t output(pc);

  // Color OUTPUT's vertices with INPUT's values.
  p_n_faces_fwd_piter<D, G> v(output.domain(), 0);
  for_all(v)
    output(v) = input(v.to_site().front());

  // Use a medium gray to color edges.
  p_n_faces_fwd_piter<D, G> e(output.domain(), 1);
  for_all(e)
    output(e) = 128;

  return output;
}


// FIXME: Move to Milena?
template <unsigned D, typename G, typename V>
mln::complex_higher_dim_connected_n_face_window_p<D, G>
make_elt_win(const mln::complex_image<D, G, V>& /* ima */)
{
  return mln::complex_higher_dim_connected_n_face_window_p<D, G>();
}

// FIXME: Move to Milena?
template <unsigned D, typename G, typename V>
mln::complex_higher_dim_connected_n_face_neighborhood<D, G>
make_elt_nbh(const mln::complex_image<D, G, V>& /* ima */)
{
  return mln::complex_higher_dim_connected_n_face_neighborhood<D, G>();
}


int main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "usage: " << argv[0] << " seeds.pgm lambda output.neato"
		<< std::endl;
      std::exit(1);
    }
  std::string input_filename = argv[1];
  unsigned lambda = atoi(argv[2]);
  std::string output_filename = argv[3];

  using namespace mln;
  using mln::value::int_u8;
  // Use clearer names.
  typedef int_u8_1complex_image2d int_u8_graph_image2d;

  typedef int_u8_graph_image2d input;
  typedef value::label_8 label;
  typedef mln_ch_value_(input, label) output;
  label nbasins;

  // Seeds.
  image2d<int_u8> seeds = io::pgm::load<int_u8>(input_filename);
  // Input image computed from the graph of influence zones (IZ) of seeds.
  typedef int_u8_graph_image2d ima_t;
  ima_t ima = make_complex_image(seeds);
#if 0
  io::neato::save(ima, "graph.neato");
#endif

  // Gradient.
  input g = morpho::gradient(ima, make_elt_win(ima));

#if 0
  // FIXME: get the name as argument.
  io::neato::save(g, "graph-g.neato");
#endif

  // Chain.
  output s = chain(g, make_elt_nbh(g), lambda, nbasins);
  io::neato::save(labeling::colorize(value::rgb8(), s, nbasins),
		  output_filename);
}
