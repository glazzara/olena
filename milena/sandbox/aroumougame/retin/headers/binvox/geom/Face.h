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
// $Id: Face.h,v 1.10 2004/12/21 18:04:15 min Exp $
//

#ifndef __Face_h
#define __Face_h

#include <iostream>
#include <vector>
#include "binvox/geom/Vertex.h"
#include "binvox/geom/geom_defs.h"

using namespace std;


namespace binvox {


class Face
{

public:

  Face();
  Face(Face& other);
  ~Face();

  void clear();

  //
  // vertices
  //
  void add_vertex(int v_id) { vertices.push_back(v_id); }
  void add_triangle(int v0, int v1, int v2);
  int get_vertex(int index) { return vertices[index]; }
  int operator[](int index) { return vertices[index]; }
  void set_vertex(int index, int new_value) { vertices[index] = new_value; }
  int get_nr_vertices() { return vertices.size(); }
  void replace_vertex(int old_id, int new_id);
  int has_vertex(int v_id);
  int get_third_vertex(int v0, int v1);
  void reverse_vertices();
  bool has_same_vertice(const Face& f);
  bool has_same_edge(const Face& f);

  friend ostream& operator<<(ostream& out_stream, const Face& F);

  
protected:

  vector<int> vertices;

};  // Face class

}
#endif


