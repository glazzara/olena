// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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
/// \brief Testing the Watershed Transform by flooding an
/// mln::complex_image.

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/black.hh>
#include <mln/literal/white.hh>

#include <mln/core/concept/function.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/site_set/p_faces.hh>
#include <mln/core/image/complex_image.hh>
// FIXME: Include these elsewhere? (In complex_image.hh?)
#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/complex_neighborhood_piter.hh>

#include <mln/data/fill.hh>

#include <mln/norm/l2.hh>

#include <mln/morpho/closing/area.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/convert/to.hh>

#include <mln/debug/iota.hh>

// FIXME: To be put elsewhere (from milena/sandbox/geraud/wst_edge.cc).
struct colorize : mln::Function_v2v< colorize >
{
  typedef mln::value::rgb8 result;
  colorize(unsigned max)
    : lut(max + 1)
  {
    lut[0] = mln::literal::black;
    for (unsigned i = 1; i <= max; ++i)
      lut[i] = result(100 + std::rand() % 150,
		      100 + std::rand() % 150,
		      100 + std::rand() % 150);
  }
  result operator()(unsigned i) const
  {
    return lut[i];
  }
  std::vector<result> lut;
};



int main()
{
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


  const unsigned D = 1;

  topo::complex<D> c;

  typedef point2d P;
  typedef geom::complex_geometry<D, P> G;
  G geom;

  // Convenience typedefs.
  typedef topo::n_face<0, D> vertex;
  typedef topo::n_face<1, D> edge;

  // 0-faces (vertices).
  vertex v0 = c.add_face();  geom.add_location(point2d(0,1));
  vertex v1 = c.add_face();  geom.add_location(point2d(2,0));
  vertex v2 = c.add_face();  geom.add_location(point2d(2,2));
  vertex v3 = c.add_face();  geom.add_location(point2d(0,3));

  // 1-faces (edges).
  edge e0 = c.add_face(-v1 + v0);
  edge e1 = c.add_face(-v2 + v0);
  edge e2 = c.add_face(-v2 + v1);
  edge e3 = c.add_face(-v0 + v3);
  edge e4 = c.add_face(-v3 + v2);

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

    dist_ima(e) = convert::to<unsigned>(10 * norm::l2_distance(p1.to_vec(), p2.to_vec()));
  }
  // Initialize 0-faces to a dummy value, to prevent the watershed from
  // finding minima on 0-faces.
  p_n_faces_fwd_piter<D, G> v_(dist_ima.domain(), 0);
  for_all(v_)
    dist_ima(v_) = mln_max(mln_value_(dist_ima_t));

  // For all edges, iterate on adjacent edges (i.e., on edges sharing
  // an adjacent vertex).
  typedef complex_lower_dim_connected_n_face_neighborhood<D, G> nbh_t;
  nbh_t nbh;
  // Neighbor edge.
  mln_niter_(nbh_t) ne(nbh, e);
  for_all(e)
  {
    std::cout << "dist_ima(" << e << ") = " << dist_ima(e)
	      << " -- adjacent edges :" << std::endl;
    for_all(ne)
      std::cout << "           " << ne << std::endl;
  }

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  // Currently, does nothing (lambda = 1).
  dist_ima_t closed_dist_ima = morpho::closing::area(dist_ima, nbh, 1);

  /*------.
  | WST.  |
  `------*/

  // Perform a Watershed Transform.
  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  typedef complex_image<D, G, wst_val_t> wst_ima_t;
  wst_ima_t wshed = morpho::watershed::flooding(closed_dist_ima, nbh, nbasins);
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
  wst_val_t actual_nbasins = nbasins - c.nfaces_of_static_dim<0>();
  std::cout << "nbasins = " << actual_nbasins << std::endl;


  colorize color(nbasins);

  std::ofstream g("complex_image_wst-wst.neato");
  g << "graph wst"  << std::endl
    << "{" << std::endl
    << "  graph [bgcolor = \"#000000\"]" << std::endl
    << "  edge [color = \"#FFFFFF\"]" << std::endl
    << "  node [color = \"#FFFFFF\", fontcolor = \"#FFFFFF\" ]" << std::endl;

  // Vertices.
  typedef complex_higher_neighborhood<D, G> e_nbh_t;
  e_nbh_t e_nbh;
  mln_niter_(e_nbh_t) v_e(e_nbh, v_);   
  for_all(v_)
  {
    // Find the adjacent basin (color).
    value::rgb8 basin_color = literal::white;
    for_all(v_e)
      if (wshed(v_e) != 0)
	{
	  basin_color = color(wshed(v_e));
	  break;
	}
    std::ostringstream basin_color_str;
    basin_color_str << '#'
		    << std::hex
		    << std::setfill('0')
		    << std::setw(2) << basin_color.red()
		    << std::setw(2) << basin_color.green()
		    << std::setw(2) << basin_color.blue()
		    << std::dec;

    g << "  v" << v_.unproxy_().face_id()
      << " [pos = \""
      << std::fixed << std::setprecision(1)
      << (float)v_.to_site().front()[1] << ", "
      << -(float)v_.to_site().front()[0]
      << "\", color = \"" << basin_color_str.str()
      << "\", fillcolor = \"" << basin_color_str.str()
      << "\", pin = \"true\", style=\"filled,setlinewidth(3)\"];"
      << std::endl;
  }

  for_all(e)
  {
    value::rgb8 basin_color = color(wshed(e));
    std::ostringstream basin_color_str;
    basin_color_str << '#'
	      << std::hex
	      << std::setfill('0')
	      << std::setw(2) << basin_color.red()
	      << std::setw(2) << basin_color.green()
	      << std::setw(2) << basin_color.blue()
	      << std::dec;

    // Adjacent vertices.
    v.start();
    topo::face<1> v1 = v.unproxy_().face();
    point2d p1 = v.to_site().front();
    (void) p1;
    v.next();
    topo::face<1> v2 = v.unproxy_().face();
    point2d p2 = v.to_site().front();
    (void) p2;
    v.next();
    mln_invariant(!v.is_valid());

    // Edges.
    g << "  v" << v1.face_id() << " -- v" << v2.face_id() << " ";
    if (wshed(e) == 0)
      g << "[color = \"#FFFFFF\"];" << std::endl;
    else
      g << "[color = \"" << basin_color_str.str()
	<< "\", style=\"setlinewidth(3)\"];" << std::endl;
  }

  g << "}" << std::endl;
  g.close();
}
