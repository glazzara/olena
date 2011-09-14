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

/// \file apps/mesh-segm-skel/mesh-segm.cc
/// \brief A program performing a WST-based segmentation of the
/// surface of the (triangle) mesh of a statue.

// FIXME: Factor commons parts between mesh-segm and mesh-skel.

#include <cstdlib>
#include <cmath>

#include <utility>
#include <iostream>

#include <TriMesh.h>

#include <mln/core/alias/point3d.hh>
#include <mln/core/alias/point3d.hh>

#include <mln/util/graph.hh>
#include <mln/core/image/line_graph_image.hh>
#include <mln/core/image/line_graph_elt_neighborhood.hh>

#include <mln/morpho/closing/area.hh>
#include <mln/morpho/meyer_wst.hh>

#include "io.hh"


// Doesn't C++ have a better way to express Pi ?
const float pi = 4 * atanf(1);


int main(int argc, char* argv[])
{
  if (argc != 4)
    {
      std::cerr << "usage: " << argv[0] << " input.off lambda output.off"
		<< std::endl;
      std::exit(1);
    }

  std::string input_filename = argv[1];
  unsigned lambda = atoi(argv[2]);
  std::string output_filename = argv[3];


  /*-------.
  | Mesh.  |
  `-------*/

  // TriMesh is a pain: it systematically allocates on the heap.
  // Introduce another name to manipulate the mesh as a (non-pointer)
  // object.
  TriMesh* mesh_ptr = TriMesh::read(input_filename.c_str());
  if (!mesh_ptr)
    std::exit(2);
  TriMesh& mesh = *mesh_ptr;

  // Computes faces (triangles).
  mesh.need_faces();
  // Computation of the mean curvature on each vertex of the mesh.
  mesh.need_curvatures();
  /* FIXME: Our implementation of the WST doesn't work well with
     floats (yet).  Convert floating point values to a proportional
     integer value for the moment.  */
  typedef int curv_t;
  std::vector<curv_t> vertex_h_inv(mesh.vertices.size(), 0.f);
  for (unsigned v = 0; v < mesh.vertices.size(); ++v)
    {
      float h = (mesh.curv1[v] + mesh.curv2[v]) / 2;
      float h_inv = 1 / pi * atan(-h) + pi / 2;
      /* FIXME: This coefficient is used to distinguish small
         curvature values.  We should get rid of it as soon as
         morpho::meyer_wst works correctly on images of float
         values.  */
      vertex_h_inv[v] = 1000 * h_inv;
    }

  /*--------.
  | Graph.  |
  `--------*/

  /* Build a graph whose vertices correspond to the faces of the mesh,
     whose edges (between two vertices) correspond to edges (between
     two faces) of the mesh.  */

  // Values associated to the graph sites.
  // (Reminder: currently, values associated to vertices in a
  // line_graph_image are dummy.)
  std::vector<curv_t> vertex_values (mesh.faces.size(), 0.f);
  std::vector<curv_t> edge_values;

  /* FIXME: We don't have the required site type yet.  Simulate with a
     dummy type (point3d).  */
  mln::util::graph<mln::point3d> g;
  // Populate the graph with vertices.
  for (unsigned i = 0; i < mesh.faces.size(); ++i)
    g.add_vertex (mln::point3d(i, i, i));

  // Populate the graph with edges.
  mesh.need_across_edge();
  for (unsigned f = 0; f < mesh.faces.size(); ++f)
    for (unsigned e = 0; e < 3; ++e)
      {
	int f_adj = mesh.across_edge[f][e];
	if (f_adj != -1)
	  {
	    // Add an edge into the graph.
	    if (g.add_edge(f, f_adj) != mln_max(mln::util::edge_id::equiv))
	      {
		// Find the edge (i.e., the two vertices) common to faces
		// F and F_ADJ.
		/* FIXME: We lack a proper interface from the TriMesh
		   structure to do this elegantly.  */
		std::vector<int> adj_vertices;
		adj_vertices.reserve(2);
		for (unsigned i = 0; i < 3; ++i)
		  for (unsigned j = 0; j < 3; ++j)
		    if (mesh.faces[f][i] == mesh.faces[f_adj][j])
		      adj_vertices.push_back(mesh.faces[f][i]);
		mln_assertion(adj_vertices.size() == 2);
		
		// Compute the mean curvature on the edge.
		edge_values.push_back((vertex_h_inv[adj_vertices[0]] + 
				       vertex_h_inv[adj_vertices[1]])
				      / 2);
	      }

	    // Check the consistency of the two arrays.
	    mln_assertion(g.edges().size() == edge_values.size());
	  }
      }

  /*-------------------.
  | Line graph image.  |
  `-------------------*/

  mln::p_line_graph<mln::point3d> plg(g);

  typedef mln::line_graph_image<mln::point3d, curv_t> ima_t;
  ima_t lg_ima(plg, vertex_values, edge_values);

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  typedef mln::line_graph_elt_neighborhood<mln::point3d> nbh_t;
  nbh_t nbh;

  ima_t closed_lg_ima = mln::morpho::closing::area(lg_ima, nbh, lambda);

  /*------.
  | WST.  |
  `------*/

  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  typedef mln::line_graph_image<mln::point3d, wst_val_t> wst_ima_t;
  wst_ima_t wshed = mln::morpho::meyer_wst(closed_lg_ima, nbh, nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  /*------------------------------------------.
  | Label graph vertices (i.e., mesh faces).  |
  `------------------------------------------*/

  /* FIXME: We should be using wshed.vertex_values_ if
     mln::line_graph_image were fully functional...  */
  std::vector<unsigned> vertex_label(wshed.domain().nvertices(), 0);
  mln_piter_(wst_ima_t) pw(wshed.domain());
  for_all(pw)
    if (wshed(pw) != 0)
      {
	mln_psite_(wst_ima_t) pp(pw);
	vertex_label[pp.first_id().to_equiv()] = wshed(pw);
	vertex_label[pp.second_id().to_equiv()] = wshed(pw);
      }

  /*---------.
  | Output.  |
  `---------*/

  // Choose random colors for each basin number.
  std::vector<mln::value::rgb8> basin_color (nbasins + 1);
  for (unsigned i = 0; i <= nbasins; ++i)
    basin_color[i] = mln::value::rgb8(random() % 256,
				      random() % 256,
				      random() % 256);

  // Assign colors to graph vertices (mesh faces).
  std::vector<mln::value::rgb8> face_color (vertex_label.size());
  for (unsigned i = 0; i < vertex_label.size() ; ++i)
    face_color[i] = basin_color[vertex_label[i]];

  // Taken and adapted from TriMesh_io.cc
  FILE* f_out = fopen(output_filename.c_str(), "wb");
  if (!f_out)
    {
      std::cerr << "Error opening " << output_filename.c_str()
		<< " for writing." << std::endl;
      std::exit(2);
    }
  write_off_colored(mesh_ptr, face_color, f_out);
  fclose(f_out);

  delete mesh_ptr;
}
