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
// $Id: MeshView.cc,v 1.2 2007/01/19 13:22:38 min Exp min $
//

#include <stdio.h>
#include "binvox/geom/MeshView.h"
#include "binvox/geom/geom_defs.h"

using namespace binvox;



MeshView::MeshView(Mesh *mesh_p) :
  View()
{
  this->mesh_p = mesh_p;
    
}  // constructor



MeshView::~MeshView()
{

}  // destructor



void
MeshView::draw_faces()
{
  //
  // get data and flags
  //
  int nr_faces = mesh_p->get_nr_faces();
  //  cout << "MeshView::draw_faces, " << nr_faces << " faces" << endl;
  
  //
  // main loop
  //
  for(int i=0; i < nr_faces; i++) {

    Face *my_face_p = (*mesh_p)[i];

    int nr_vertices = my_face_p->get_nr_vertices();

    glBegin(GL_POLYGON);
    {
      for(int j=0; j < nr_vertices; j++) {
	int v_id = my_face_p->get_vertex(j);
	Vertex *v_p = mesh_p->get_vertex(v_id);
	if (v_p) {
	  // send vertex
	  Vector V = v_p->get_vector();
	  glVertex3f(V[X], V[Y], V[Z]);
	}	
      }  // for
    }
    glEnd();
    
  }  // for each face

}  // MeshView::draw_faces



void
MeshView::redraw()
{
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glShadeModel(GL_SMOOTH);

  glLineWidth(1.0);

  glDisable(GL_TEXTURE_2D);

  draw_faces();
  
}  // MeshView::redraw



