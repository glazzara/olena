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
// $Id: VoxelRef.h,v 1.7 2001/06/10 20:14:49 min Exp $
//

#ifndef __VoxelRef_h
#define __VoxelRef_h

#include <vector>
#include <string>
#include "binvox/voxel/Voxels.h"

using namespace std;


namespace binvox {


class VoxelRef
{

public:

  VoxelRef(Voxels& voxels_ref);
  ~VoxelRef();

  void init_dimensions();
  
  static void check_update();
  int needs_update() { return voxels.needs_update(); }
  virtual void update();

  virtual string get_name() { return "VoxelRef"; }

  Voxels& get_voxels_ref() { return voxels; }
  void set_voxels_ref(Voxels& new_ref) { voxels = new_ref; }
  
  
protected:

  Voxels& voxels;

  int size, wxh;
  int width, depth, height;
  int prev_width, prev_depth, prev_height;
  int dimensions_changed;
  
private:

  static vector<VoxelRef *> refs;

  
};  // VoxelRef class
}

#endif


