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

/// \file apps/mesh-segm-skel/mesh-max-curv.cc
/// \brief A program computing the max curvature at each (2-)face of
/// a mesh.

#include <cstdlib>

#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

#include <TriMesh.h>

#include <mln/math/max.hh>
#include <mln/math/sqr.hh>
#include <mln/accu/stat/min_max.hh>
#include <mln/fun/v2v/linear.hh>

#include "io.hh"


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

  std::vector<float> vertex_m(mesh.vertices.size(), 0.f);
  for (unsigned v = 0; v < mesh.vertices.size(); ++v)
    // Max curvature.
    vertex_m[v] = mln::math::max(mln::math::sqr(mesh.curv1[v]),
				 mln::math::sqr(mesh.curv2[v]));

  // For each face of the mesh, computean an average curvature value
  // from the mean curvature at its vertices.
  std::vector<float> face_m(mesh.faces.size(), 0.f);
  mln::accu::stat::min_max<float> acc;
  for (unsigned f = 0; f < mesh.faces.size(); ++f)
    {
      float m = (vertex_m[mesh.faces[f][0]] +
		 vertex_m[mesh.faces[f][1]] +
		 vertex_m[mesh.faces[f][2]]) / 3;
      face_m[f] = m;
      acc.take(m);
    }

  /* Shrink the values of FACE_M into the range 0..1, as these are
     the only values accepted a an RGB floating-point component in the
     OFF file format.  */
  std::vector<float> normalized_face_m(face_m.size(), 0.0f);
  std::pair<float, float> min_max(acc);
  // FIXME: Taken from mln/data/stretch.hh (this should be factored).
  float min = min_max.first;
  float max = min_max.second;
  // Don't normalize actually if the curvature is constant (i.e.,
  // if min == max).
  if (min != max)
    {
      float m = 0.0f;
      float M = 1.0f;
      float a = (M - m) / (max - min);
      float b = (m * max - M * min) / (max - min);
      mln::fun::v2v::linear<float, float, float> f(a, b);
      std::transform(face_m.begin(), face_m.end(),
		     normalized_face_m.begin(), f);
    }

  // Taken and adapted from TriMesh_io.cc
  FILE* f_out = fopen(output_filename.c_str(), "wb");
  if (!f_out)
    {
      std::cerr << "Error opening " << output_filename.c_str()
		<< " for writing." << std::endl;
      std::exit(2);
    }
  write_off_float(mesh_ptr, normalized_face_m, f_out);
  fclose(f_out);

  delete mesh_ptr;
}
