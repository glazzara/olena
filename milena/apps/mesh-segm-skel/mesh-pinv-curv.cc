// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file apps/mesh-segm-skel/mesh-pinv-curv.cc
/// \brief A program computing the pseudo-inverse curvature at each
/// (2-)face of a mesh.

#include <cstdlib>
#include <cmath>

#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

#include <TriMesh.h>

#include "io.hh"


// Doesn't C++ have a better way to express Pi?
static const float pi = 4 * atanf(1);


int main(int argc, char* argv[])
{
  if (argc != 3)
    {
      std::cerr << "usage: " << argv[0] << " input.off output.off"
		<< std::endl;
      std::exit(1);
    }

  std::string input_filename = argv[1];
  std::string output_filename = argv[2];


  // TriMesh is a pain: it systematically allocates on the heap.
  // Introduce another name to manipulate the mesh as a (non-pointer)
  // object.
  TriMesh* mesh_ptr = TriMesh::read(input_filename.c_str());
  if (!mesh_ptr)
    std::exit(2);
  TriMesh& mesh = *mesh_ptr;

  // Computes faces (triangles).
  mesh.need_faces();
  // Computation of the curvature on each vertex of the mesh.
  mesh.need_curvatures();
  std::vector<float> vertex_h_inv(mesh.vertices.size(), 0.f);
  for (unsigned v = 0; v < mesh.vertices.size(); ++v)
    {
      float h = (mesh.curv1[v] + mesh.curv2[v]) / 2;
      // Pseudo-inverse curvature.
      float h_inv = 1 / pi * (atan(-h) + pi / 2);
      vertex_h_inv[v] = h_inv;
    }
  // For each face of the mesh, computean an average curvature value
  // from the mean curvature at its vertices.
  std::vector<float> face_h_inv(mesh.faces.size(), 42.f);
  for (unsigned f = 0; f < mesh.faces.size(); ++f)
    {
      float h_inv = (vertex_h_inv[mesh.faces[f][0]] +
		     vertex_h_inv[mesh.faces[f][1]] +
		     vertex_h_inv[mesh.faces[f][2]]) / 3;
      mln_invariant(0.f <= h_inv);
      mln_invariant(h_inv <= 1.f);
      face_h_inv[f] = h_inv;
    }

  // Taken and adapted from TriMesh_io.cc
  FILE* f_out = fopen(output_filename.c_str(), "wb");
  if (!f_out)
    {
      std::cerr << "Error opening " << output_filename.c_str()
		<< " for writing." << std::endl;
      std::exit(2);
    }
  write_off_float(mesh_ptr, face_h_inv, f_out);
  fclose(f_out);

  delete mesh_ptr;
}
