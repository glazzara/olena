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
// $Id: CameraList.h,v 1.1 2002/10/30 19:20:13 min Exp min $
//

#ifndef __CameraList_h
#define __CameraList_h

#ifdef IRIX
#include <locale>
#endif
#include <vector>
#include "binvox/ui/Camera.h"


namespace binvox {

class CameraList
{

public:

  CameraList();
  ~CameraList();

  void clear();
  
  void add_camera(Camera *new_cam);
  int get_nr_cameras() { return cameras.size(); }
  Camera *get_camera(int index) { return cameras[index]; }
  Camera *get_cur_camera() { return cameras[cur_index]; }
  void set_cur_camera(int index);
  
  Camera *set_next_cam();
  Camera *set_prev_cam();
  
  void transform(Matrix T);

  
private:

  vector<Camera *> cameras;
  int cur_index;

  
};  // CameraList class
}

#endif


