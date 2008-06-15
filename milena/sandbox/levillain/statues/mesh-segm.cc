// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <cmath>

#include <utility>
#include <iostream>

#include <TriMesh.h>

#include <mln/value/rgb8.hh>

#include <mln/core/point3d.hh>
#include <mln/make/point3d.hh>

#include <mln/util/graph.hh>
#include <mln/core/line_graph_image.hh>
#include <mln/core/line_graph_elt_neighborhood.hh>

#include <mln/morpho/closing_area.hh>
#include <mln/morpho/meyer_wst.hh>


// Doesn't C++ have a better way to express Pi ?
const float pi = 4 * atanf(1);


// -------------------------------------------------------------------
// Taken from TriMesh_io.cc
// -------------------------------------------------------------------
// Convert colors float -> uchar
static unsigned char color2uchar(float p)
{
	return min(max(int(255.0f * p + 0.5f), 0), 255);
}

// Write a bunch of vertices to an ASCII file
static void write_verts_asc(TriMesh *mesh, FILE *f,
			    const char *before_vert,
			    const char *before_norm,
			    const char *before_color,
			    bool float_color,
			    const char *before_conf,
			    const char *after_line)
{
    for (int i = 0; i < mesh->vertices.size(); i++) {
		fprintf(f, "%s%.7g %.7g %.7g", before_vert,
				mesh->vertices[i][0],
				mesh->vertices[i][1],
				mesh->vertices[i][2]);
		if (!mesh->normals.empty() && before_norm)
			fprintf(f, "%s%.7g %.7g %.7g", before_norm,
				mesh->normals[i][0],
				mesh->normals[i][1],
				mesh->normals[i][2]);
		if (!mesh->colors.empty() && before_color && float_color)
			fprintf(f, "%s%.7g %.7g %.7g", before_color,
				mesh->colors[i][0],
				mesh->colors[i][1],
				mesh->colors[i][2]);
		if (!mesh->colors.empty() && before_color && !float_color)
			fprintf(f, "%s%d %d %d", before_color,
				color2uchar(mesh->colors[i][0]),
				color2uchar(mesh->colors[i][1]),
				color2uchar(mesh->colors[i][2]));
		if (!mesh->confidences.empty() && before_conf)
			fprintf(f, "%s%.7g", before_conf, mesh->confidences[i]);
		fprintf(f, "%s\n", after_line);
	}
}
// -------------------------------------------------------------------


// -------------------------------------------------------------------
// Taken and adapted from TriMesh_io.cc
// -------------------------------------------------------------------
// Write a bunch of faces to an ASCII file with colors.
static void write_faces_asc_colored(TriMesh *mesh,
				    const std::vector<mln::value::rgb8>& colors,
				    FILE *f,
				    const char *before_face,
				    const char *after_line)
{
  mesh->need_faces();
  for (int i = 0; i < mesh->faces.size(); i++)
    {
      fprintf(f, "%s%d %d %d %d %d %d%s\n",
	      before_face,
	      mesh->faces[i][0], mesh->faces[i][1], mesh->faces[i][2],
	      int(colors[i].red()),
	      int(colors[i].green()),
	      int(colors[i].blue()),
	      after_line);
    }
}

/// Write an off file with colors.
static void write_off_colored(TriMesh *mesh,
			      const std::vector<mln::value::rgb8>& colors,
			      FILE *f)
{
  fprintf(f, "OFF\n");
  mesh->need_faces();
  fprintf(f, "%lu %lu 0\n", (unsigned long) mesh->vertices.size(),
	  (unsigned long) mesh->faces.size());
  write_verts_asc(mesh, f, "", 0, 0, false, 0, "");
  write_faces_asc_colored(mesh, colors, f, "3 ", "");
}
// -------------------------------------------------------------------


int main(int argc, char* argv[])
{
  /*-------.
  | Mesh.  |
  `-------*/

//   std::string filename = "../../aroumougame/test/test.off";
  std::string filename = "bunny-holefilled.off";

  // TriMesh is a pain: it systematically allocate on the heap.
  TriMesh* mesh_ptr = TriMesh::read(filename.c_str());
  if (!mesh_ptr)
    exit(1);
  TriMesh& mesh = *mesh_ptr;

  // Computes faces (triangles).
  mesh.need_faces();

  // Computation of the mean curvature on each vertex of the mesh.
  mesh.need_curvatures();
  // FIXME: Our implementation of the WST doesn't work well with floats.
  // Convert to a proportional integer value for the moment.
  typedef int curv_t;
  std::vector<float> vertex_h_inv(mesh.vertices.size(), 0.f);
  for (unsigned v = 0; v < mesh.vertices.size(); ++v)
    {
      float h = (mesh.curv1[v] + mesh.curv2[v]) / 2;
      float h_inv = 1 / pi * atan(-h) + pi / 2;
      /* FIXME: This coefficient is used to distinguish small
         curvature values.  We sould get rid of it as soon as
         meyer_wst works correctly on image of float values.  */
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
    g.add_vertex (mln::make::point3d(i, i, i));

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
		   strucure to do this elegantly.  */
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

  ima_t closed_lg_ima (lg_ima.domain());
  mln::morpho::closing_area(lg_ima, nbh, 500, closed_lg_ima);

  /*------.
  | WST.  |
  `------*/

  // Perform a Watershed Transform.
  typedef unsigned wst_val_t;
  wst_val_t nbasins;
  typedef mln::line_graph_image<mln::point3d, wst_val_t> wst_ima_t;
  wst_ima_t wshed = mln::morpho::meyer_wst(closed_lg_ima, nbh, nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;

  /*------------------------------------.
  | Label graph vertices (mesh faces).  |
  `------------------------------------*/

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

  // Get random colors for each basin.
  std::vector<mln::value::rgb8> basin_color (nbasins + 1);
  for (unsigned i = 0; i <= nbasins; ++i)
    basin_color[i] = mln::value::rgb8(random() % 256,
				      random() % 256,
				      random() % 256);
  

  // Assign colors to faces.
  std::vector<mln::value::rgb8> face_color (vertex_label.size());
  for (unsigned i = 0; i < vertex_label.size() ; ++i)
    face_color[i] = basin_color[vertex_label[i]];

  // FIXME: This is ugly; convert to C++ code ASAP.
  // Taken and adapted from TriMesh_io.cc
  const char* out_filename = "out.off";
  FILE* f_out = fopen(out_filename, "wb");
  if (!f_out)
    {
      perror("fopen");
      fprintf(stderr, "Error opening %s for writing.\n", out_filename);
      exit(1);
    }

  write_off_colored(mesh_ptr, face_color, f_out);
  fclose(f_out);

  delete mesh_ptr;
}
