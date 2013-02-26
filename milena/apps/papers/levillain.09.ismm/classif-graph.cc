// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
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

#include <iostream>

#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/blobs.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/debug/println.hh>
#include <mln/draw/line.hh>

#include <mln/value/int_u8.hh>
#include <mln/core/alias/point2d.hh>

#include <mln/util/site_pair.hh>

#include <mln/core/image/edge_image.hh>

#include <mln/data/fill.hh>
#include <mln/norm/l2.hh>

#include <mln/labeling/colorize.hh>

#include "influence_zones.hh"
#include "chain.hh"

/* FIXME: Introduce aliases for common specializations of graph images
   such as the return type of make_graph_image.  */

/** Create a 1-complex (graph) image from the connected components of
    \a seeds.  The values associated to a 1-face (edge) is equal to
    the L2-distance between its two adjacent vertices.  The values on
    a 0-face (vertex) is set to 0.  */
mln::edge_image<mln::util::site_pair<mln::point2d>, mln::value::int_u8>
make_graph_image(const mln::image2d<bool>& seeds)
{
  using namespace mln;
  using mln::value::int_u8;

  border::thickness = 0;

  /*-----------------------.
  | Connected components.  |
  `-----------------------*/

  unsigned nlabels;
  image2d<unsigned> label = labeling::blobs(seeds, c4(), nlabels);

  std::cout << "n seeds = " << nlabels << std::endl;
  {
    image2d<int_u8> lab(label.domain());
    data::paste(label, lab);
#if 0
    io::pgm::save(lab, "label.pgm");
#endif
  }

  /*------------------.
  | Influence zones.  |
  `------------------*/

  image2d<unsigned> iz = influence_zones(label, c4());
  {
    image2d<int_u8> IZ(iz.domain());
    data::paste(iz, IZ);
#if 0
    io::pgm::save(IZ, "iz.pgm");
#endif
  }

  /*----------------------------.
  | Construction of the graph.  |
  `----------------------------*/

  // Adjacency matrix.
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
	  if (iz(p) != iz(r))
	    adj[iz(p)][iz(r)] = true;
	  else
	    adj[iz(r)][iz(p)] = true;
	}
      if (iz.has(b) && iz(p) != iz(b))
	{
	  if (iz(p) != iz(b))
	    adj[iz(p)][iz(b)] = true;
	  else
	    adj[iz(b)][iz(p)] = true;
	}
    }
  }

  // Graph structure.
  typedef util::graph G;
  G g;

  // Vertices.  Note that vertex 0 corresponds to the background, and
  // is therefore not connected to any other vertex.
  g.add_vertices(nlabels + 1);

  // Sites associated to edges.
  typedef util::site_pair<point2d> site_t;
  typedef fun::i2v::array<site_t> fsite_t;
  fsite_t sites;

  {
    // Vertices locations.
    std::vector<point2d> v_site (nlabels + 1, point2d(0,0));
    {
      box2d::piter p(label.domain());
      for_all(p)
	if (label(p) != 0)
	  v_site[label(p)] = p;
    }
    std::cout << "v_site size = " << v_site.size() << std::endl;

    // Edges.
    std::vector<util::edge_id_t> e;
    {
      for (unsigned l = 1; l <= nlabels; ++l)
	for (unsigned ll = l + 1; ll <= nlabels; ++ll)
	  if (adj[l][ll])
	    {
	      sites.append(site_t(v_site[l], v_site[ll]));
	      e.push_back(g.add_edge(l, ll));
	    }
    }
    std::cout << "e size = " << e.size() << std::endl;
  }

  /*-------------.
  | Edge image.  |
  `-------------*/

  /* FIXME: edge_image should provide a ctor to initialize an instance
     with just a graph an an edge-to-site function (but no values).
     Currently this is not possible.  Hence the ``adhead of time''
     creation of these values .  */
  // Values.
  typedef fun::i2v::array<int_u8> dist_values_t;
  dist_values_t dist_values(sites.size());
  for (unsigned i = 0; i < dist_values.size(); ++i)
    dist_values(i) = 0;


  // Type of an edge-valued graph image located in a discrete 2D space
  // with int_u8 values on each edge.
  typedef edge_image<site_t, int_u8, util::graph> dist_ima_t;
  dist_ima_t dist_ima(g, sites, dist_values);
  data::fill(dist_ima, 0u);

  /*------------------------.
  | Graph image iterators.  |
  `------------------------*/

  accu::stat::max<int_u8> dist_max;

  // Output image for visualization purpose only.
  image2d<int_u8> canvas(seeds.domain());
  data::fill(canvas, 0);

  // Iterator on (the edges of) DIST_IMA.
  mln_piter_(dist_ima_t) e(dist_ima.domain());
  for_all(e)
  {
    // Site associated to E.
    site_t p = e;
    point2d p1 = e.first();
    point2d p2 = e.second();

    dist_ima(e) = norm::l2_distance(p1.to_vec(), p2.to_vec());
    dist_max.take(dist_ima(e));

    draw::line(canvas, p1, p2, dist_ima(e));
    canvas(p1) = 255;
    canvas(p2) = 255;
  }

  std::cout << "distance max = " << dist_max << std::endl;

#if 0
  io::pgm::save(canvas, "canvas.pgm");
#endif

  return dist_ima;
}


int
main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "usage: " << argv[0] << " input.pbm lambda output.ppm"
		<< std::endl;
      std::exit(1);
    }
  std::string input_filename = argv[1];
  unsigned lambda = atoi(argv[2]);
  std::string output_filename = argv[3];

  using namespace mln;
  using mln::value::int_u8;

  // Seeds.
  image2d<bool> seeds;
  io::pbm::load(seeds, input_filename);

  // Shortcut: type of an edge-valued graph image located in a
  // discrete 2D space with int_u8 values for each edge.
  typedef edge_image<util::site_pair<point2d>, int_u8> ima_t;

  // Image computed from the graph of influence zones (IZ) of seeds.
  ima_t ima = make_graph_image(seeds);

  // IMA's elementary neighborhood: adjacency relation between two
  // edges through a shared vertex
  typedef ima_t::nbh_t nbh_t;
  nbh_t nbh;

  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  // Output image type.
  typedef edge_image<util::site_pair<point2d>, wst_val_t> wst_ima_t;

  // Chain.
  wst_ima_t wst_ima = chain(ima, nbh, lambda, nbasins);

  // Output image showing the results of the above chain on an image
  // similar to the input.
  image2d<int_u8> canvas_wst(seeds.domain());
  data::fill(canvas_wst, 0);

  // Iterator on (the edges of) WST_IMA.
  mln_piter_(wst_ima_t) e(wst_ima.domain());

  for_all(e)
  {
    // Site associated to E.
    util::site_pair<point2d> p = e;
    point2d p1 = e.first();
    point2d p2 = e.second();

    draw::line(canvas_wst, p1, p2, wst_ima(e));
  }

  io::ppm::save(labeling::colorize(value::rgb8(), canvas_wst, nbasins),
		output_filename);
}
