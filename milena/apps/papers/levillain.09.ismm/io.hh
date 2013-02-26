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

/// \file
/// \brief Some neato (Graphviz) I/O routines.

// FIXME: Refactor and move into the library.

#include <fstream>
#include <sstream>
#include <iomanip>

#include <string>

#include <mln/core/image/complex_image.hh>
#include <mln/core/alias/complex_geometry.hh>

#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/complex_neighborhood_piter.hh>

#include <mln/value/int_u8.hh>


namespace mln
{

  namespace io
  {

    namespace neato
    {

      /* FIXME: This is just the gray-level version.  Handle other value
	 types as well.  */
      // FIXME: Use an alias instead of the long type name.
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
	  point2d p1 = adj_v.to_site().front();
	  adj_v.next();
	  topo::face<1> v2 = adj_v.unproxy_().face();
	  point2d p2 = adj_v.to_site().front();
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
	  point2d p1 = adj_v.to_site().front();
	  adj_v.next();
	  topo::face<1> v2 = adj_v.unproxy_().face();
	  point2d p2 = adj_v.to_site().front();
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
