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
// $Id: Camera.h,v 1.2 2002/10/30 19:20:13 min Exp min $
//

#ifndef __Camera_h
#define __Camera_h

#include <iostream>
#include <string>
#include "binvox/math/Vector.h"

using namespace std;


namespace binvox {


class Camera
{

public:

  Camera();
  Camera(Vector position, Vector lookat, Vector up);
  Camera(Camera& other);
  ~Camera();

  void init(Vector position, Vector lookat, Vector up);
  void set_position(Vector new_pos);
  void set_lookat(Vector new_lookat) { lookat = new_lookat; }
  void set_up(Vector new_up) { up = new_up; }
  
  Vector& get_position() { return position; }
  Vector& get_lookat() { return lookat; }
  Vector& get_up() { return up; }

  void set_name(char *new_name) { name = new_name; }
  char *get_name() { return (char *) name.c_str(); }
  
  void load_matrix();

  void print();
  friend ostream& operator<<(ostream& out_stream, Camera& C);
  friend istream& operator>>(istream& in_stream, Camera& C);

  void store(string filename);
  void restore(string filename);

  void adjust_camera(Vector bmin, Vector bmax);
  
  
private:

  string name;
  
  Vector position;
  Vector lookat;
  Vector up;

  
};  // Camera class
}

#endif


