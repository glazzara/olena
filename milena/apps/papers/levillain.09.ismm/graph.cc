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
#include <iomanip>

#include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_queue_fast.hh>
#include <mln/labeling/blobs.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>
#include <mln/draw/line.hh>
#include <mln/pw/all.hh>
#include <mln/binarization/threshold_ge.hh>

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

#include "chain.hh"


// FIXME: Copied and adjusted from pics/graph.cc; factor.

namespace mln
{

  template <typename I, typename N>
  mln_concrete(I)
  influence_zones(const I& input, const N& nbh)
  {
    mln_concrete(I) output = duplicate(input);

    p_queue_fast<mln_site(I)> q;

    {
      // Initialization.
      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
	if (input(p) != 0)
	  for_all(n) if (input.has(n))
	    if (input(n) == 0)
	      {
		q.push(p);
		break;
	      }
    }
    {
      // Body.
      mln_site(I) p;
      mln_niter(N) n(nbh, p);
      while (! q.is_empty())
	{
	  p = q.pop_front();
	  mln_invariant(output(p) != 0);
	  for_all(n) if (input.has(n))
	    if (output(n) == 0)
	      {
		output(n) = output(p);
		q.push(n);
	      }
	}
    }

    return output;

  }


  namespace io
  {

    namespace neato
    {

      /* FIXME: This is just the gray-level version.  Handle other value
	 types as well.  */
      void
      save(const complex_image<1, discrete_plane_1complex_geometry,
	                       value::int_u8>& ima,
	   const std::string& filename,
	   const std::string& bgcolor = "#0000C0",
	   const std::string& fontcolor = "#0000C0",
	   bool empty_vertex_label = true)
      {
	typedef value::int_u8 V;
	typedef complex_image<1, discrete_plane_1complex_geometry, V> I;
	const unsigned D = 1;
	typedef discrete_plane_1complex_geometry G;

	std::ofstream g(filename.c_str());
	g << "graph wst"  << std::endl
	  << "{" << std::endl
	  << "  graph [bgcolor = \"" << bgcolor << "\"]" << std::endl
	  << "  edge [color = \"#FFFFFF\"]" << std::endl
	  << "  node [color = \"#FFFFFF\", height=\"5\", width=\"5\","
	  << " fontsize=\"100\", fontcolor = \"" << fontcolor << "\"]"
	  << std::endl;

	// Vertices.
	p_n_faces_fwd_piter<D, G> v(ima.domain(), 0);
	typedef complex_higher_neighborhood<D, G> e_nbh_t;
	e_nbh_t e_nbh;
	for_all(v)
	{
	  V vertex_color = ima(v);
	  std::ostringstream vertex_color_str;
	  // FIXME: Only valid for gray-level images.
	  vertex_color_str << '#'
			   << std::hex
			   << std::setfill('0')
			   << std::setw(2) << vertex_color
			   << std::setw(2) << vertex_color
			   << std::setw(2) << vertex_color
			   << std::dec;

	  g << "  v" << v.unproxy_().face_id()
	    << " [pos = \""
	    << std::fixed << std::setprecision(1)
	    << (float)v.to_site().front()[1] << ", "
	    << -(float)v.to_site().front()[0]
	    << "\", color = \"" << vertex_color_str.str()
	    << "\", fillcolor = \"" << vertex_color_str.str()
	    << "\", pin = \"true\", style=\"filled,setlinewidth(3)\"";
	  if (empty_vertex_label)
	    g << ", label = \"\"";
	  g << "];"
	    << std::endl;
	}

	// Edges.
	p_n_faces_fwd_piter<D, G> e(ima.domain(), 1);
	typedef complex_lower_neighborhood<D, G> v_nbh_t;
	v_nbh_t v_nbh;
	mln_niter_(v_nbh_t) adj_v(v_nbh, e);
	for_all(e)
	{
	  V edge_color = ima(e);
	  std::ostringstream edge_color_str;
	  edge_color_str << '#'
			  << std::hex
			  << std::setfill('0')
			  << std::setw(2) << edge_color
			  << std::setw(2) << edge_color
			  << std::setw(2) << edge_color
			  << std::dec;

	  // Adjacent vertices.
	  adj_v.start();
	  topo::face<1> v1 = adj_v.unproxy_().face();
	  adj_v.next();
	  topo::face<1> v2 = adj_v.unproxy_().face();
	  adj_v.next();
	  mln_invariant(!adj_v.is_valid());

	  g << "  v" << v1.face_id() << " -- v" << v2.face_id() << " ";
	  g << "[color = \"" << edge_color_str.str()
	    << "\", style=\"setlinewidth(10)\"];" << std::endl;
	}

	g << "}" << std::endl;
	g.close();
      }

      // FIXME: Factor with the previous version.
      void
      save(const complex_image<1, discrete_plane_1complex_geometry,
	                       value::rgb8>& ima,
	   const std::string& filename,
	   const std::string& bgcolor = "#0000C0",
	   const std::string& fontcolor = "#0000C0",
	   bool empty_vertex_label = true)
      {
	typedef value::rgb8 V;
	typedef complex_image<1, discrete_plane_1complex_geometry, V> I;
	const unsigned D = 1;
	typedef discrete_plane_1complex_geometry G;

	std::ofstream g(filename.c_str());
	g << "graph wst"  << std::endl
	  << "{" << std::endl
	  << "  graph [bgcolor = \"" << bgcolor << "\"]" << std::endl
	  << "  edge [color = \"#FFFFFF\"]" << std::endl
	  << "  node [color = \"#FFFFFF\", height=\"5\", width=\"5\","
	  << " fontsize=\"100\", fontcolor = \"" << fontcolor << "\"]"
	  << std::endl;

	// Vertices.
	p_n_faces_fwd_piter<D, G> v(ima.domain(), 0);
	typedef complex_higher_neighborhood<D, G> e_nbh_t;
	e_nbh_t e_nbh;
	for_all(v)
	{
	  V vertex_color = ima(v);
	  std::ostringstream vertex_color_str;
	  // FIXME: Only valid for gray-level images.
	  vertex_color_str << '#'
			   << std::hex
			   << std::setfill('0')
			   << std::setw(2) << vertex_color.red()
			   << std::setw(2) << vertex_color.green()
			   << std::setw(2) << vertex_color.blue()
			   << std::dec;

	  g << "  v" << v.unproxy_().face_id()
	    << " [pos = \""
	    << std::fixed << std::setprecision(1)
	    << (float)v.to_site().front()[1] << ", "
	    << -(float)v.to_site().front()[0]
	    << "\", color = \"" << vertex_color_str.str()
	    << "\", fillcolor = \"" << vertex_color_str.str()
	    << "\", pin = \"true\", style=\"filled,setlinewidth(3)\"";
	  if (empty_vertex_label)
	    g << ", label = \"\"";
	  g << "];"
	    << std::endl;
	}

	// Edges.
	p_n_faces_fwd_piter<D, G> e(ima.domain(), 1);
	typedef complex_lower_neighborhood<D, G> v_nbh_t;
	v_nbh_t v_nbh;
	mln_niter_(v_nbh_t) adj_v(v_nbh, e);
	for_all(e)
	{
	  V edge_color = ima(e);
	  std::ostringstream edge_color_str;
	  edge_color_str << '#'
			 << std::hex
			 << std::setfill('0')
			 << std::setw(2) << edge_color.red()
			 << std::setw(2) << edge_color.green()
			 << std::setw(2) << edge_color.blue()
			 << std::dec;

	  // Adjacent vertices.
	  adj_v.start();
	  topo::face<1> v1 = adj_v.unproxy_().face();
	  adj_v.next();
	  topo::face<1> v2 = adj_v.unproxy_().face();
	  adj_v.next();
	  mln_invariant(!adj_v.is_valid());

	  g << "  v" << v1.face_id() << " -- v" << v2.face_id() << " ";
	  g << "[color = \"" << edge_color_str.str()
	    << "\", style=\"setlinewidth(10)\"];" << std::endl;
	}

	g << "}" << std::endl;
	g.close();
      }

    } // end of namespace mln::io::neato

  } // end of namespace mln::io

} // end of namespace mln


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
    labeling::blobs(mln::binarization::threshold_ge(input, 1), c4(), nlabels);

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
  io::neato::save(ima, "apps/graph.neato");

  // Gradient.
  input g = morpho::gradient(ima, make_elt_win(ima));

#if 0
  // FIXME: get the name as argument.
  io::neato::save(g, "apps/graph-g.neato");
#endif

  // Chain.
  output s = chain(g, make_elt_nbh(g), lambda, nbasins);
  io::neato::save(labeling::colorize(value::rgb8(), s, nbasins),
		  output_filename);
}
