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

#include "binvox/geom/ObjMeshFile.h"


using namespace binvox;


ObjMeshFile::ObjMeshFile(Mesh& mesh_ref, string filespec) :
  MeshFile(mesh_ref, filespec)
{

}  // constructor



ObjMeshFile::~ObjMeshFile()
{

}  // destructor



static char obj_tokens[][MAX_TOKEN_LENGTH] = { "\n", "/",{0x5c} };

int
ObjMeshFile::load()
{
  cout << "ObjMeshFile::load(" << my_filespec << ")" << endl;
  
  Tokenfile *in = new Tokenfile();
  if (!in->open(my_filespec)) {
    cout << "Error opening [" << my_filespec << "]" << endl;
    return 0;
  }
  
  char token[MAX_TOKEN_LENGTH];
  mesh.clear();

  in->print_nr_lines();
  int line_nr;

  //
  // trick to make newline character a token
  //
  in->set_whitespace_chars(", \t\r");  // found an obj file with backslashes in it...
  in->set_standard_tokens(obj_tokens);

  Vertex *new_vertex_p;
  Face *new_face_p;

  int group_index = -1;
  
  while(in->get_current_char() != MY_EOF) {
    in->get_token(token);

    if(strcmp(token, "v") == 0) {
      //
      // get (x, y, z) values, add Vertex to vertices array
      //
      float x, y, z;
      in->get_number(token), x = atof(token);
      in->get_number(token), y = atof(token);
      in->get_number(token), z = atof(token);
      new_vertex_p = new Vertex(x, y, z);
      in->accept_token('\n');
      mesh.vertices.push_back(new_vertex_p);
    }
    else if (strcmp(token, "f") == 0) {
      new_face_p = new Face();
      
      int done = 0;
      do {
	in->get_token(token);

	if (token[0] == '\\') {  // after a backslash, skip over the newline
	  in->get_token(token);
	  in->get_token(token);
	}
	
	if (token[0] == '\n') done = 1;
	else {
	  int v_id = atoi(token);
	  new_face_p->add_vertex(v_id - 1);  // OBJ !

	  in->lookahead_token(token);
	  if (token[0] == '/') {
	    in->get_token(token);
	    in->get_token(token);  // texture index

	    in->lookahead_token(token);
	    if(token[0] == '/') {
	      in->get_token(token);
	      in->get_token(token);  // normal index
	    }
	  }  // if
	
	}  // else, got a vertex
      } while (!done);

      //      if (group_index >= 0) new_face_p->set_face_group_index(group_index);
      mesh.faces.push_back(new_face_p);
			
    }  // face 'f'
    else if (strcmp(token, "g") == 0) {
      group_index++;
      in->get_token(token);
      cout << "  reading group [" << token << "]" << endl;
    }
    else if (strcmp(token, "vt") == 0) {
      // texture coordinate
      float s, t;
      in->get_number(token), s = atof(token);
      in->get_number(token), t = atof(token);
    }
    else if (strcmp(token, "vn") == 0) {
      // vertex normal
      float a, b, c;
      in->get_number(token), a = atof(token);
      in->get_number(token), b = atof(token);
      in->get_number(token), c = atof(token);
    }
    else if (strcmp(token, "mtllib") == 0) {
      // material library
      char filename[128];
      in->read_line(filename);
      //      in->get_token(token);  // get material library filename
      cout << "material library: " << filename << endl;
    }
    else if (strcmp(token, "usemtl") == 0) {
      // use material
      in->get_token(token);  // get material name
    }
    else {
      if (token[0] != '\n') {
	in->print_error_line();
	cout << "Warning: do not know how to handle [" << token << "]" << endl;
	in->read_until('\n');  // skip rest of the line
      }
    }  // unknown token, not a newline

    in->skip_whitespace();  // has to be here in case there's whitespace at the end

    line_nr = in->get_line_nr();
    if ((line_nr % 100) == 0) cout << "\r  " << line_nr << " lines read" << flush;
      
  }  // while not eof

  in->close();

  cout << "\r  " << line_nr << " lines read" << endl;
  cout << "Read " << mesh.faces.size() << " faces, " << mesh.vertices.size() << " vertices." << endl;

  return 1;
	
}  // ObjMeshFile::load



