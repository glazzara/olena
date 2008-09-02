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
// $Id: Mesh.cc,v 1.23 2001/07/20 15:25:17 min Exp min $
//

#include <algorithm>
#include <queue>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "binvox/file/Tokenfile.h"
#include "binvox/math/MyRandom.h"
#include "binvox/time/WallTimer.h"
#include "binvox/geom/Mesh.h"

using namespace std;

#define DEBUG(x)
#define DEBUG_POT(x)     // debug part_of_triangle function
#define DEBUG_PRUNE(x)   // debug prune_terminal_faces
#define DEBUG_CONNECT(x)
#define DEBUG_TEXTURES(x)

using namespace binvox;



Mesh::Mesh(string name) :
  my_name(name)
{
  clear();
  init();

}  // constructor



Mesh::Mesh(Mesh& other)
{
  my_name = other.my_name;
  center = other.center;

  for(int i=0; i < other.vertices.size(); i++) {
    Vertex *v_p = new Vertex(other.vertices[i]->get_vector());
    vertices.push_back(v_p);
  }

  for(int i=0; i < other.faces.size(); i++) {
    Face *f_p = new Face(*other.faces[i]);
    faces.push_back(f_p);
  }

}  // copy constructor



Mesh::~Mesh()
{
  clear();
}  // destructor



void
Mesh::clear()
{
  for(int i=0; i < vertices.size(); i++) delete vertices[i];
  vertices.clear();
  clear_faces();

}  // Mesh::clear



void
Mesh::init()
{

}  // Mesh::init



int
Mesh::add_vertex(Vertex *v_p)
{
  int index = vertices.size();

  vertices.push_back(v_p);
  return index;
  
}  // Mesh::add_vertex



int
Mesh::add_face(Face *f_p)
{
  int index = faces.size();

  faces.push_back(f_p);
  return index;
  
}  // Mesh::add_face



int
Mesh::add_triangle(int v0, int v1, int v2)
{
  Face *f_p = new Face();
  f_p->add_triangle(v0, v1, v2);
  int face_index = add_face(f_p);
  return face_index;
  
}  // Mesh::add_triangle



void
Mesh::clear_faces()
{
  //  cout << "Mesh::clear_faces, deleting " << faces.size() << " faces" << endl;
  for(int i=0; i < faces.size(); i++) delete faces[i];
  faces.clear();
  
}  // Mesh::clear_faces



int
Mesh::compute_bounding_box(int remove_largest)
{
  //  cout << "Mesh::compute_bounding_box" << endl;

  bmin.set(MaxFloat, MaxFloat, MaxFloat);
  bmax.set(-MaxFloat, -MaxFloat, -MaxFloat);

  int nr_vertices = vertices.size();
  for(int i=0; i < nr_vertices; i++) {
    Vertex *v_p = vertices[i];
    Vector v = v_p->get_vector();

    if (v[X] > bmax[X]) bmax[X] = v[X];
    if (v[X] < bmin[X]) bmin[X] = v[X];
    if (v[Y] > bmax[Y]) bmax[Y] = v[Y];
    if (v[Y] < bmin[Y]) bmin[Y] = v[Y];
    if (v[Z] > bmax[Z]) bmax[Z] = v[Z];
    if (v[Z] < bmin[Z]) bmin[Z] = v[Z];
    
  }  // for

  center = (bmin + bmax) / 2;

  // still do a range check
  for(int i=0; i < 3; i++) {
    if ((bmin[i] < -1e12) || (bmin[i] > 1e12) ||  // some arbitrary limit
	(bmax[i] < -1e12) || (bmax[i] > 1e12)) {
      cout << "Error: bmin/bmax out of range: bmin: " << bmin << ", bmax: " << bmax << endl;
      bmin.set(0, 0, 0);  // just so adjust_camera won't hang
      bmax.set(1, 1, 1);
      return 0;
    }
  }

  return 1;
  
}  // Mesh::compute_bounding_box



void
Mesh::get_bounding_box(Vector *bmin_p, Vector *bmax_p)
{
  *bmin_p = bmin;
  *bmax_p = bmax;
  
}  // Mesh::get_bounding_box



void
Mesh::transform(Matrix& M)
{
  vector<Vertex *>::iterator vertex_it;
    
  for(vertex_it = vertices.begin(); vertex_it != vertices.end(); vertex_it++) {
    Vector v = (*vertex_it)->get_vector();
    Vector w = M * v;
    (*vertex_it)->set_vector(w);
  }

}  // Mesh::transform



void
Mesh::translate(Vector trans)
{
  int nr_vertices = vertices.size();
  for(int i=0; i < nr_vertices; i++)
    vertices[i]->set_vector(vertices[i]->get_vector() + trans);
  
}  // Mesh::translate



void
Mesh::rotate(Matrix& R)
{
  compute_bounding_box();

  translate(-center);
  transform(R);
  translate(center);

}  // Mesh::rotate



// return the scale factor used
Float
Mesh::normalize()
{
  //  cout << "Mesh::normalize" << endl;

  // bbox is assumed
  //  compute_bounding_box();
  cout << "  bounding box: " << bmin << " - " << bmax << endl;
  
  Vector length = bmax - bmin;
  length.vec_abs();  // !!!
  Float scale_x = 0.95/length[X];
  Float scale_y = 0.95/length[Y];
  Float scale_z = 0.95/length[Z];
  Float min_scale = MaxFloat;
  if (scale_x < min_scale) min_scale = scale_x;
  if (scale_y < min_scale) min_scale = scale_y;
  if (scale_z < min_scale) min_scale = scale_z;
  
  Matrix scale;
  scale.scale(min_scale, min_scale, min_scale);

  // actually transform the mesh
  Matrix trans;
  trans.translate(-bmin);

  Matrix trans2;
  trans2.translate(0.025, 0.025, 0.025);
  cout << "  normalization transform:" << endl;
  cout << "  (1) translate " << -bmin << ", (2) scale " << min_scale
       << ", (3) translate [" << trans2[3] << ", " << trans2[7] << ", " << trans2[11] << "]" << endl;

  normalization_transform = trans2 * scale * trans;
  //  cout << "  full transform:" << endl << normalization_transform << endl;
  
  transform(normalization_transform);
  
  return min_scale;
  
}  // Mesh::normalize


