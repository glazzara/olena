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
// $Id: Camera.cc,v 1.2 2002/10/30 19:20:06 min Exp min $
//

#include <fstream>
#include "binvox/ui/Camera.h"
#include "binvox/ui/GLwindow.h"
#include "binvox/ui/opengl.h"


using namespace binvox;


Camera::Camera()
{
  init(Vector(1, 0, 0),
       Vector(0, 0, 0),
       Vector(0, 0, 1));

}  // default constructor



Camera::Camera(Vector position, Vector lookat, Vector up)
{
  init(position, lookat, up);
  
}  // constructor



Camera::Camera(Camera& other)
{
  name = other.name;

  position = other.position;
  lookat = other.lookat;
  up = other.up;

}  // Camera::Camera



Camera::~Camera()
{

}  // destructor



void
Camera::init(Vector position, Vector lookat, Vector up)
{
  set_position(position);
  this->lookat = lookat;
  this->up = up;

}  // Camera::init



void
Camera::set_position(Vector new_pos)
{
  position = new_pos;

  
}  // Camera::set_position



void
Camera::load_matrix()
{
  //  cout << "Camera::load_matrix" << endl;
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  GLfloat light_pos[4] = {0.0, 0.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  
  gluLookAt(position[X], position[Y], position[Z],
	    lookat[X], lookat[Y], lookat[Z],
	    up[X], up[Y], up[Z]);

}  // Camera::load_matrix

namespace binvox{

ostream& operator<<(ostream& out_stream, Camera& C)
{
  Vector P = C.position;
  Vector D = C.lookat - C.position;
  Vector U = C.up;

  out_stream << P[X] << " " << P[Y] << " " << P[Z] << "  "
	     << D[X] << " " << D[Y] << " " << D[Z] << "  "
	     << U[X] << " " << U[Y] << " " << U[Z];

  return out_stream;
  
}  // operator<<



istream& operator>>(istream& in_stream, Camera& C)
{
  in_stream >> C.position >> C.lookat >> C.up;
  return in_stream;
  
}  // operator>>
}


void
Camera::store(string filename)
{
  string filespec = "./camera/";
  filespec += filename;
  filespec += ".camera";

  ofstream *out = new ofstream(filespec.c_str());
  if (!out->good()) {
    cout << "Camera::store error opening [" << filespec << "]" << endl;
    return;
  }

  *out << *this;

  out->close();
  
}  // Camera::store



void
Camera::restore(string filename)
{
  cout << "Camera::restore(" << filename << ")" << endl;
  
  string filespec = "./camera/";
  filespec += filename;
  filespec += ".camera";

  ifstream *inp = new ifstream(filespec.c_str());
  if (!inp->good()) {
    //    cout << "Camera::restore error opening [" << filespec << "]" << endl;
    delete inp;
    return;
  }

  *inp >> *this;
  inp->close();
  delete inp;

  cout << "Camera::restore, set camera to [" << *this << "]" << endl;
  
}  // Camera::restore



void
Camera::print()
{
  cout << "Camera name: " << name << endl;
  cout << "  position: " << position << endl;
  cout << "  lookat: " << lookat << endl;
  cout << "  up: " << up << endl;
  
}  // Camera::print



