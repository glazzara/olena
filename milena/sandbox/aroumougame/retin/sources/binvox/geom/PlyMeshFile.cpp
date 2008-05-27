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
// $Id$
//

#include "binvox/geom/PlyMeshFile.h"
#include "binvox/ply/ply_gatech.h"


using namespace binvox;


PlyMeshFile::PlyMeshFile(Mesh& mesh_ref, string filespec) :
  MeshFile(mesh_ref, filespec)
{

}  // constructor



PlyMeshFile::~PlyMeshFile()
{

}  // destructor



//
// PLY data structures
//
const char *elem_names[] = { "vertex", "face" };

typedef struct PlyVertex {
  float x, y, z;
} PlyVertex;

PlyProperty vert_props[] = {
  {"x", Float32, Float32, offsetof(PlyVertex, x), 0, 0, 0, 0},
  {"y", Float32, Float32, offsetof(PlyVertex, y), 0, 0, 0, 0},
  {"z", Float32, Float32, offsetof(PlyVertex, z), 0, 0, 0, 0},
};

typedef struct PlyFace {
  unsigned char nr_vertices;
  int *vertices;
} PlyFace;

PlyProperty face_props[] = {
  {"vertex_indices", Int32, Int32, offsetof(PlyFace, vertices),
   1, Uint8, Uint8, offsetof(PlyFace, nr_vertices)},
};

int
PlyMeshFile::load()
{
  cout << "PlyMeshFile::load(" << my_filespec << ")" << endl;
  
  int nr_elems;
  char **elist;
  
  FILE *fp = fopen((char *) my_filespec.c_str(), "rb");
  PlyFile *ply = read_ply(fp);
  //  open_for_reading_ply((char *) total_filespec.c_str(), &nr_elems, &elist, &file_type, &version);
  if (!ply) return 0;
  
  nr_elems = ply->num_elem_types;
  
  cout << "  version: " << ply->version << endl;
  cout << "  file type: " << ply->file_type << endl;
  cout << "  nr_elems: " << nr_elems << endl;

  //
  // read each element
  //
  for(int i=0; i < nr_elems; i++) {

    int nr;
    char *elem_name = setup_element_read_ply(ply, i, &nr);
    cout << "  reading element [" << elem_name << "], count " << nr << endl;
    
    int nr_props = ply->elems[i]->nprops;
    PlyProperty **plist = get_element_description_ply(ply, elem_name, &nr, &nr_props);
    if (!plist) return 0;
    cout << "  number of element properties: " << nr_props << endl;

    //
    // vertex
    //
    if (equal_strings("vertex", elem_name)) {

      for(int k=0; k < nr_props; k++) {
	char *prop_name = plist[k]->name;

	// setup properties we encounter
	for(int j=0; j < 3; j++) {
	  char *my_prop_name = vert_props[j].name;
	  if (strcmp(my_prop_name, prop_name) == 0) {
	    setup_property_ply(ply, &vert_props[j]);
	  }
	}
      }  // for, check properties

      // read the vertices
      for (int j=0; j < nr; j++) {
	PlyVertex ply_vertex;
        get_element_ply(ply, (void *) &ply_vertex);
	// add vertex to my mesh
	Vertex *v_p = new Vertex(ply_vertex.x, ply_vertex.y, ply_vertex.z);
	mesh.add_vertex(v_p);
      }  // for, read vertices
      
    }  // if vertex
    //
    // face
    //
    else {
      if (equal_strings("face", elem_name)) {

	// setup face properties
	for(int k=0; k < nr_props; k++) {
	  char *prop_name = plist[k]->name;

	  // check for (currently) single property
	  if (strcmp(face_props[0].name, prop_name) == 0) {
	      setup_property_ply(ply, &face_props[0]);
	  }
	}  // for

	//
	// load the faces
	//
	for(int j=0; j < nr; j++) {
	  
	  PlyFace ply_face;
	  get_element_ply(ply, (void *) &ply_face);
	  
	  // add face to my mesh
	  Face *f_p = new Face();
	  for(int k=0; k < ply_face.nr_vertices; k++) f_p->add_vertex(ply_face.vertices[k]);
	  mesh.add_face(f_p);
	  
	}  // for, read faces
      }  // if face
      //
      // unsupported elements are skipped
      //
      else {
	cout << "skipping " << nr << " instances of element " << elem_name << endl;

	// first compute how big an element is
	int element_size = 0;
	for(int j=0; j < nr_props; j++) {
	  int external_type = plist[j]->external_type;
	  int type_size = get_ply_type_size(external_type);
	  element_size += type_size;
	  if (plist[j]->is_list) {
	    cout << "  error: one of the properties is a list, no code here yet to properly skip this element" << endl;
	    exit(1);
	  }
	}
	cout << "  single element size: " << element_size << " bytes" << endl;
	byte *buffer = new byte[element_size];
	for (int j=0; j < nr; j++) get_element_ply(ply, (void *) buffer);
	delete[] buffer;
	    
      }  // else, unknown element
      
    }  // else
      
  }  // for each type of element

  close_ply(ply);
  cout << "Done, read " << mesh.get_nr_vertices() << " vertices and " << mesh.get_nr_faces() << " faces" << endl;

  return 1;

}  // PlyMeshFile::load



