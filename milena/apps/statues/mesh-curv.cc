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

/// \file apps/statues/mesh-curv.cc
/// \brief A program computing the mean curvature at each (2-)face of
/// a mesh.

#include <cstdlib>
#include <cmath>

#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

#include <TriMesh.h>

#include <mln/accu/min_max.hh>
#include <mln/fun/v2v/linear.hh>
#include <mln/math/abs.hh>

#include "io.hh"


// Doesn't C++ have a better way to express Pi?
const float pi = 4 * atanf(1);


int main(int argc, char* argv[])
{
  if (argc != 3)
    {
      std::cerr << "usage: " << argv[0] << " input.off output.off"
		<< std::endl;
      exit(1);
    }

  std::string input_filename = argv[1];
  std::string output_filename = argv[2];


  // TriMesh is a pain: it systematically allocates on the heap.
  // Introduce another name to manipulate the mesh as a (non-pointer)
  // object.
  TriMesh* mesh_ptr = TriMesh::read(input_filename.c_str());
  if (!mesh_ptr)
    exit(2);
  TriMesh& mesh = *mesh_ptr;

  // Computes faces (triangles).
  mesh.need_faces();
  // Computation of the mean curvature on each vertex of the mesh.
  mesh.need_curvatures();
  std::vector<float> vertex_h_inv(mesh.vertices.size(), 0.f);
  for (unsigned v = 0; v < mesh.vertices.size(); ++v)
    {
      float h = (mesh.curv1[v] + mesh.curv2[v]) / 2;
      float h_inv = 1 / pi * atan(-h) + pi / 2;
      vertex_h_inv[v] = h_inv;
    }
  // For each face of the mesh, computean an average curvature value
  // from the mean curvature at its vertices.
  std::vector<float> face_h_inv(mesh.faces.size(), 42.f);
  mln::accu::min_max<float> acc;
  for (unsigned f = 0; f < mesh.faces.size(); ++f)
    {
      float h_inv = (vertex_h_inv[mesh.faces[f][0]] +
		     vertex_h_inv[mesh.faces[f][1]] +
		     vertex_h_inv[mesh.faces[f][2]]) / 3;
      face_h_inv[f] = h_inv;
      acc.take(h_inv);
    }

  /* Shrink the values of FACE_H_INV into the range 0..1, as these are
     the only values accepted a an RGB floating-point component in the
     OFF file format.  */
  std::vector<float> normalized_face_h_inv(face_h_inv.size(), 0.0f);
  std::pair<float, float> min_max(acc);
  // FIXME: Taken from mln/level/stretch.hh (this should be factored).
  float min = min_max.first;
  float max = min_max.second;
  // Don't normalize actually if the curvature is constants (i.e.,
  // if min == max).
  if (min != max)
    {
      float m = 0.0f;
      float M = 1.0f;
      float a = (M - m) / (max - min);
      float b = (m * max - M * min) / (max - min);
      mln::fun::v2v::linear<float, float, float> f(a, b);
      std::transform(face_h_inv.begin(), face_h_inv.end(),
		     normalized_face_h_inv.begin(), f);
    }

  // Taken and adapted from TriMesh_io.cc
  FILE* f_out = fopen(output_filename.c_str(), "wb");
  if (!f_out)
    {
      std::cerr << "Error opening " << output_filename.c_str()
		<< " for writing." << std::endl;
      exit(2);
    }
  write_off_float(mesh_ptr, normalized_face_h_inv, f_out);
  fclose(f_out);

  delete mesh_ptr;
}
