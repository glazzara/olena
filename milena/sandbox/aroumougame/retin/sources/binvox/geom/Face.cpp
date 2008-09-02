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
// $Id: Face.cc,v 1.11 2004/12/21 18:04:09 min Exp $
//

#include <assert.h>
#include "binvox/geom/Face.h"

#define DEBUG(x)
#define DEBUG_DISTANCE(x)

using namespace binvox;




Face::Face()
{
  clear();
  
}  // constructor



Face::Face(Face& other)
{
  clear(); 
  
  for(int i=0; i < other.vertices.size(); i++) vertices.push_back(other.vertices[i]);

}  // copy constructor



Face::~Face()
{
  clear();
  
}  // destructor



void
Face::clear()
{
  vertices.clear();

}  // Face::clear



void
Face::add_triangle(int v0, int v1, int v2)
{
  vertices.push_back(v0);
  vertices.push_back(v1);
  vertices.push_back(v2);
  
}  // Face::add_triangle



void
Face::replace_vertex(int old_id, int new_id)
{
  //  cout << "  Face::replace_vertex(" << old_id << ", " << new_id << ")" << endl;
  
  int nr_vertices = vertices.size();
  for(int i=0; i < nr_vertices; i++) {
    if (vertices[i] == old_id) {
      vertices[i] = new_id;
      return;
    }
  }  // for

  //  cout << "    error: could not find " << old_id << endl;
  
}  // Face::replace_vertex


namespace binvox {

ostream& operator<<(ostream& out_stream, const Face& F)
{
  int nr_vertices = F.vertices.size();
  out_stream << "[";
  
  for(int i=0; i < nr_vertices; i++) {
    out_stream << F.vertices[i];
    if (i < (nr_vertices - 1)) out_stream << " ";
  }

  return out_stream;
  
}  // operator<< for Face
}


int
Face::has_vertex(int v_id)
{
  int nr_vertices = vertices.size();
  
  for(int i=0; i < nr_vertices; i++) {
    if (vertices[i] == v_id) return (i + 1);  // note 
  }

  return 0;
  
}  // Face::has_vertex



int
Face::get_third_vertex(int v0, int v1)
{
  for(int i=0; i < 3; i++) {
    if ((vertices[i] != v0) && (vertices[i] != v1)) return vertices[i];
  }

  return -1;
  
}  // Face::get_third_vertex



void
Face::reverse_vertices()
{
  int nr_vertices = vertices.size();
  for(int i=0; i < nr_vertices/2; i++) {
    int v_id = vertices[i];
    vertices[i] = vertices[nr_vertices - i - 1];
    vertices[nr_vertices - i - 1] = v_id;
  }
  
}  // Face::reverse_vertices



bool
Face::has_same_vertice(const Face& f)
{
  bool result = false;
  int nbV1 = vertices.size();
  int nbV2 = f.vertices.size();

  for(int i=0; (i < nbV1) && (!result) ; i++)
  {
    for(int j=0; (j < nbV2) && (!result) ; j++)
    {
      if(vertices[i]==f.vertices[j])
      {
        result = true;
      }
    }
  }

  return result;

}

bool
Face::has_same_edge(const Face& f)
{
  int result = 0;
  int nbV1 = vertices.size();
  int nbV2 = f.vertices.size();

  for(int i=0; i < nbV1; i++)
  {
    for(int j=0; j < nbV2; j++)
    {
      if(vertices[i]==f.vertices[j])
      {
        result++;
      }
    }
  }

  return (result==2);

}

