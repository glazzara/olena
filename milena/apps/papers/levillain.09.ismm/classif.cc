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
#include <mln/core/routine/duplicate.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_queue_fast.hh>
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

// FIXME: Include these elsewhere? (In complex_image.hh?)
#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/complex_neighborhood_piter.hh>

#include <mln/debug/iota.hh>
#include <mln/data/fill.hh>
#include <mln/norm/l2.hh>

#include <mln/morpho/closing/area.hh>
#include <mln/morpho/closing/volume.hh>
#include <mln/morpho/closing/height.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/labeling/colorize.hh>

#include "influence_zones.hh"

// FIXME: Include and use chain.hh.


int main(int argc, char* argv[])
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

  /*----------------------------------------.
  | Complex + complex geometry (location).  |
  `----------------------------------------*/

  /* A (simplicial) 1-complex and its adjacency graph.

       c   0     1     2     3
     r .------------------------
       |        v0      e3     v3
     0 |         o-----------o                v0----e3----v3      
       |        / \         /                / \         /   
       |       /   \       /                /   \       /    
     1 |   e0 /     e1    / e4             e0    e1    e4  
       |     /       \   /                /       \   /      
       |    /         \ /                /         \ /       
     2 |   o-----------o                v1----e2----v2
       | v1     e2      v2

       v = vertex
       e = edge
  */




  border::thickness = 0;

  image2d<bool> seeds;
  io::pbm::load(seeds, input_filename);

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

  image2d<unsigned> iz = influence_zones(label, c4());
  {
    image2d<int_u8> IZ(iz.domain());
    data::paste(iz, IZ);
#if 0
    io::pgm::save(IZ, "iz.pgm");
#endif
  }
//   debug::println( (pw::value(iz) - pw::cst(1)) | iz.domain() );


  // mk_graph

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

  image2d<int_u8> canvas(seeds.domain());
  data::fill(canvas, 0);


  /*---------------------.
  | Complex-based pset.  |
  `---------------------*/

  p_complex<D, G> pc(c, geom);

  /*----------------------.
  | Complex-based image.  |
  `----------------------*/

  // An image type built on a 1-complex with unsigned values on each
  // face (both vertices and edges).
  typedef complex_image<D, G, unsigned> dist_ima_t;

  // Create and initialize an image based on PC.
  dist_ima_t dist_ima(pc);
  data::fill(dist_ima, 0u);

  /*--------------------------------.
  | Complex-based image iterators.  |
  `--------------------------------*/

  accu::stat::max<unsigned> dist_max;

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

    /* FIXME: Why is the norm divided by 2?  Is is to have it fit on
       an int_u8?  If so, we shall use a canvas with a larger image
       value (type e.g. int_u16) and use data::stretch() to save it as
       an 8-bit image. */
    dist_ima(e) = norm::l2_distance(p1.to_vec(), p2.to_vec()) / 2;
    dist_max.take(dist_ima(e));

    draw::line(canvas, p1, p2, dist_ima(e));
    canvas(p1) = 255;
    canvas(p2) = 255;
  }

  std::cout << "distance max = " << dist_max << std::endl;


#if 0
  io::pgm::save(canvas, "canvas.pgm");
#endif


//   // Initialize 0-faces to a dummy value, to prevent the watershed from
//   // finding minima on 0-faces.
//   p_n_faces_fwd_piter<D, G> v_(dist_ima.domain(), 0);
//   for_all(v_)
//   {
//     // FIXME: canvas(v_.to_site().front()) = 255;
//     dist_ima(v_) = mln_max(mln_value_(dist_ima_t));
//   }


  // FIXME: Use the chain() routine.


//   // For all edges, iterate on adjacent edges (i.e., on edges sharing
//   // an adjacent vertex).
  typedef complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;
//   // Neighbor edge.
//   mln_niter_(nbh_t) ne(nbh, e);
//   for_all(e)
//   {
//     std::cout << "dist_ima(" << e << ") = " << dist_ima(e)
// 	      << " -- adjacent edges :" << std::endl;
//     for_all(ne)
//       std::cout << "           " << ne << std::endl;
//   }



  /*-----------------.
  | Simplification.  |
  `-----------------*/

  // Currently, does nothing (lambda = 1).
  dist_ima_t closed_dist_ima; // (dist_ima.domain());
  // FIXME: In other programs, an area closing is used.
  closed_dist_ima = morpho::closing::height(dist_ima, nbh, lambda);

  /*------.
  | WST.  |
  `------*/

  // Perform a Watershed Transform.
  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  typedef complex_image<D, G, wst_val_t> wst_ima_t;
  wst_ima_t wshed = morpho::meyer_wst(closed_dist_ima, nbh, nbasins);
  /* Note that since the image is based not only on the 1-faces but
     also on the 0-faces of the complex, and given the above
     neighborhood, the domain seen by the WST is not connected!  It is
     actually composed of five components :

     - a component containing all the 1-faces (egdes) which are all
       connected through
       mln::complex_lower_dim_connected_n_face_neighborhood;

     - four (singleton) components corresponding to the 0-faces
       (vertices), connected to no other part of the complex according to
       mln::complex_lower_dim_connected_n_face_neighborhood.

     Since the component made of the edges contains two local minima,
     the number of basins is equal to 6:

           2 minima for the edges' component
     + 4 * 1 minima for the vertices's components
     --------------------------------------------
           6 basins.

     Hence the result.


     We definitely need a complex_image that can accept a subset of a
     complex as domain (or at least a p_face<N, D, P>.  */
  std::cout << "nbasins = " << nbasins - c.nfaces_of_dim(0) << std::endl;


  image2d<int_u8> canvas_wst(seeds.domain());
  data::fill(canvas_wst, 255);

  for_all(e)
  {
    v.start();
    point2d p1 = v.to_site().front();
    v.next();
    point2d p2 = v.to_site().front();
    v.next();
    mln_invariant(!v.is_valid());

    draw::line(canvas_wst, p1, p2, wshed(e));
  }

  io::ppm::save(labeling::colorize(value::rgb8(), canvas_wst, nbasins),
		output_filename);
}
