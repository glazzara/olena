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
#include <mln/pw/all.hh>

#include <mln/value/int_u8.hh>
#include <mln/core/alias/point2d.hh>

#include <mln/core/site_set/p_faces.hh>
#include <mln/core/image/complex_image.hh>
#include <mln/core/alias/complex_image.hh>

#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/complex_neighborhood_piter.hh>

#include <mln/debug/iota.hh>
#include <mln/data/fill.hh>
#include <mln/norm/l2.hh>

#include <mln/labeling/colorize.hh>

#include "influence_zones.hh"
#include "chain.hh"


/** Create a 1-complex (graph) image from the connected components of
    \a seeds.  The values associated to a 1-face (edge) is equal to
    the L2-distance between its two adjacent vertices.  The values on
    a 0-face (vertex) is set to 0.  */
mln::int_u8_1complex_image2d
make_complex_image(const mln::image2d<bool>& seeds)
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

  /*--------------------------------.
  | Construction of the 1-complex.  |
  `--------------------------------*/

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


  const unsigned D = 1;
  // 1-complex data structure. 
  topo::complex<D> c;

  typedef point2d P;
  // Geometry (spatial locations) associated to the complex's faces.
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
	    // Create an edge as an oriented (``algebraic'') face.
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

  // An image type built on a 1-complex with int_u8 values on each
  // face (both vertices and edges).
  typedef complex_image<D, G, int_u8> dist_ima_t;

  // Create and initialize an image based on PC.
  dist_ima_t dist_ima(pc);
  data::fill(dist_ima, 0u);

  /*--------------------------------.
  | Complex-based image iterators.  |
  `--------------------------------*/

  accu::stat::max<int_u8> dist_max;

  // Output image for visualization purpose only.
  image2d<int_u8> canvas(seeds.domain());
  data::fill(canvas, 0);

  // For each edge (1-face), compute the distance between the two
  // adjacent vertices (0-faces).
  p_n_faces_fwd_piter<D, G> e(dist_ima.domain(), 1);
  typedef complex_lower_neighborhood<D, G> v_nbh_t;
  v_nbh_t v_nbh;
  mln_niter_(v_nbh_t) v(v_nbh, e);
  for_all(e)
  {
    v.start();
    point2d p1 = v.to_site().front();
    v.next();
    point2d p2 = v.to_site().front();
    v.next();
    mln_invariant(!v.is_valid());

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

  // Shortcut: type of a 1-complex image located in a discrete 2D
  // space with int_u8 values for each face.
  typedef int_u8_1complex_image2d ima_t;
  // Shortcuts: dimension and geometry (set of spatial locations)
  // associated to this image type.
  const unsigned D = 1;
  typedef mln_geom_(ima_t) G;

  // Image computed from the graph of influence zones (IZ) of seeds.
  ima_t ima = make_complex_image(seeds);

  // Adjacency relation between two n-faces through a shared adjacent
  // (n-1)-face.  (An edge is adjacent to its neighboring edges; a
  // vertex has no neighbors.)
  typedef complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;

  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  // Output image type.
  typedef complex_image<D, G, wst_val_t> wst_ima_t;

  // Chain.
  wst_ima_t wst_ima = chain(ima, nbh, lambda, nbasins);

  // Output image showing the results of the above chain on an image
  // similar to the input.
  image2d<int_u8> canvas_wst(seeds.domain());
  data::fill(canvas_wst, 255);

  // Iterator on the edges (1-faces) of WST_IMA.
  p_n_faces_fwd_piter<D, G> e(wst_ima.domain(), 1);
  typedef complex_lower_neighborhood<D, G> v_nbh_t;
  v_nbh_t v_nbh;
  // Iterator on the vertices adjacent to E.
  mln_niter_(v_nbh_t) v(v_nbh, e);

  for_all(e)
  {
    v.start();
    point2d p1 = v.to_site().front();
    v.next();
    point2d p2 = v.to_site().front();
    v.next();
    mln_invariant(!v.is_valid());

    draw::line(canvas_wst, p1, p2, wst_ima(e));
  }

  io::ppm::save(labeling::colorize(value::rgb8(), canvas_wst, nbasins),
		output_filename);
}
