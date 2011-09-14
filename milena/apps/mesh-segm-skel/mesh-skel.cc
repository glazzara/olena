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

/// \file apps/mesh-segm-skel/mesh-skel.cc
/// \brief A program computing a skeleton of the surface of the
/// (triangle) mesh of a statue.

// FIXME: Factor commons parts between mesh-segm and mesh-skel.

#include <cstdlib>
#include <cmath>

#include <utility>
#include <iostream>

#include <TriMesh.h>

#include <mln/core/alias/point3d.hh>
#include <mln/core/alias/point3d.hh>

#include <mln/util/graph.hh>
#include <mln/core/image/graph_image.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>

#include <mln/morpho/closing/area.hh>
#include <mln/labeling/regional_minima.hh>

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
  std::vector<float> vertex_h_inv(mesh.faces.size(), 0.f);
  for (unsigned v = 0; v < mesh.vertices.size(); ++v)
    {
      float h = (mesh.curv1[v] + mesh.curv2[v]) / 2;
      float h_inv = 1 / pi * atan(-h) + pi / 2;
      vertex_h_inv[v] = h_inv;
    }
  // Computation of the mean curvature on each face of the mesh.
  /* FIXME: Our implementation of the computation of the regional
     minima doesn't work well with floats (yet).  Convert floating
     point values to a proportional integer value for the moment.  */
  typedef int curv_t;
  std::vector<curv_t> face_h_inv(mesh.faces.size(), 0.f);
  for (unsigned f = 0; f < mesh.faces.size(); ++f)
    {
      float h_inv =    
	(vertex_h_inv[mesh.faces[f][0]] +
	 vertex_h_inv[mesh.faces[f][1]] +
	 vertex_h_inv[mesh.faces[f][2]])
	/ 3;
      /* FIXME: This coefficient is used to distinguish small
         curvature values.  We should get rid of it as soon as
         labeling::regional_minima works correctly on images of float
         values.  */
      face_h_inv[f] = 1000 * h_inv;
    }

  /*--------.
  | Graph.  |
  `--------*/

  /* Build a graph whose vertices correspond to the faces of the mesh,
     whose edges (between two vertices) correspond to edges (between
     two faces) of the mesh.  */

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
	  // Add an edge into the graph.
	  g.add_edge(f, f_adj);
      }

  /*--------------.
  | Graph image.  |
  `--------------*/

  mln::p_graph<mln::point3d> pg(g);

  typedef mln::graph_image<mln::point3d, curv_t> ima_t;
  ima_t g_ima(pg, face_h_inv);

  /*-----------------.
  | Simplification.  |
  `-----------------*/

  typedef mln::graph_elt_neighborhood<mln::point3d> nbh_t;
  nbh_t nbh;

  ima_t closed_g_ima = mln::morpho::closing::area(g_ima, nbh, lambda);

  /*------------------.
  | Regional minima.  |
  `------------------*/

  typedef unsigned label_t;
  label_t nlabels;
  typedef mln::graph_image<mln::point3d, label_t> label_ima_t;
  label_ima_t minima =
    mln::labeling::regional_minima(closed_g_ima, nbh, nlabels);
  std::cout << "nlabels = " << nlabels << std::endl;
  
  /*-----------.
  | Skeleton.  |
  `-----------*/
  
  // FIXME: To do.

  /*---------.
  | Output.  |
  `---------*/

  /*  FIXME We should created a boolean graph_image instead.  But as
      we cannot directly save a graph_image as an OFF file now, just
      store the values of this would-be image into an array.  */
  // Assign a boolean value to graph vertices (mesh faces).
  std::vector<bool> face_value (minima.domain().nvertices(), true);
  mln_piter_(label_ima_t) pm(minima.domain());
  for_all(pm)
    // FIXME: Use literal::zero.
    if (minima(pm) != 0)
      {
	// The face belongs to a regional minima: ``remove'' it from
	// the mesh by tagging it as false.
	mln_psite_(label_ima_t) pp(pm);
	face_value[pp.id().to_equiv()] = false;
      }

  // Taken and adapted from TriMesh_io.cc
  FILE* f_out = fopen(output_filename.c_str(), "wb");
  if (!f_out)
    {
      std::cerr << "Error opening " << output_filename.c_str()
		<< " for writing." << std::endl;
      std::exit(2);
    }
  write_off_binary(mesh_ptr, face_value, f_out);
  fclose(f_out);

  delete mesh_ptr;
}
