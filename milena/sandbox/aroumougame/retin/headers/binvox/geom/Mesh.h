//
// binvox, a binary 3D mesh voxelizer
// Copyright (c) 2004-2007 by Patrick Min, patrick.n.min "at" gmail "dot" com
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
// 
//
// $Id: Mesh.h,v 1.21 2001/07/20 15:25:19 min Exp min $
//

#ifndef __MESH_H
#define __MESH_H

#ifdef IRIX
#include <locale>
#endif
#include <string>
#include <vector>
#include "binvox/math/Vector.h"
#include "binvox/math/Matrix.h"
#include "binvox/geom/Vertex.h"
#include "binvox/geom/Face.h"


using namespace std;


namespace binvox {

class MeshView;

class Mesh
{

public:
  
  Mesh(string name = "noname");
  Mesh(Mesh& other);
  ~Mesh();

  void clear();
  void init();

  string get_name() { return my_name; }
  void set_name(string new_name) { my_name = new_name; }

  //
  // vertices
  //
  inline Vertex *get_vertex(int index) { return vertices[index]; }
  void set_vertex(int index, Vertex *v_p) { vertices[index] = v_p; }
  int add_vertex(Vertex *v_p);
  int get_nr_vertices() { return vertices.size(); }
  vector<Vertex *>& get_vertices_ref() { return vertices; }
  
  //
  // faces
  //
  int add_face(Face *f_p);
  int add_triangle(int v0, int v1, int v2);
  Vector get_face_center(int index);
  inline Face *operator[](int index) { return faces[index]; }
  int get_nr_faces() { return faces.size(); }
  int has_triangle(int v0, int v1, int v2);
  void clear_faces();
  int my_triangulate();
  void flip_normals();
  void remove_zero_area_faces();
  
  void purge_removed_faces();
  
  int compute_bounding_box(int remove_largest = 0);
  void get_bounding_box(Vector *bmin_p, Vector *bmax_p);
  
  Float normalize();
  Matrix get_norm_transform() { return normalization_transform; }
  
  void transform(Matrix& M);
  void translate(Vector trans);
  void rotate(Matrix& R);
  Vector get_center() { return center; }
  
  void compute_data();
  
  friend class MeshFile;
  friend class ObjMeshFile;
  friend class OffMeshFile;
  friend class WrlMeshFile;
  friend class MeshView;
  

protected:

  string my_name;

  Matrix normalization_transform;
  
  Vector center;  // of bounding box
  Vector centroid;
  
  vector<Vertex *> vertices;
  vector<Face *> faces;
  
  Vector bmin, bmax;
  
  
};  // Mesh class
}
#endif

