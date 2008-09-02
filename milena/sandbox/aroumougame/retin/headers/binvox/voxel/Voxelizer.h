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
// $Id: Voxelizer.h,v 1.7 2004/07/06 10:54:03 min Exp $
//

#ifndef __Voxelizer_h
#define __Voxelizer_h

#include "binvox/voxel/VoxelFilter.h"
#include "binvox/geom/Mesh.h"
#include "binvox/geom/MeshView.h"
#include "binvox/ui/GLwindow.h"


namespace binvox {

class Voxelizer : public VoxelFilter
{

public:

  Voxelizer(Voxels& voxels_ref, Mesh& mesh_ref, GLwindow *win_p);
  ~Voxelizer();

  void parity_vote_voxelize(int skip_simple = 0);
  void carve_voxelize();

  void set_dilated(int new_value) { dilated = new_value; }
  
  void invert() { voxels.invert(); }
  
  string get_name() { return "Voxelizer"; }
  

  static const int DEFAULT_WIDTH = 128;
  static const int MAX_WIDTH = 1024;
  static const int MAX_HEIGHT = 1024;

  
private:

  Mesh& mesh;
  GLwindow *win_p;
  MeshView *mesh_view_p;
  
  //
  // z buffer stuff
  //
  void read_z_buffer(GLfloat *buffer_p);

  GLfloat z_buffer_x_front[MAX_WIDTH * MAX_HEIGHT];
  GLfloat z_buffer_y_front[MAX_WIDTH * MAX_HEIGHT];
  GLfloat z_buffer_z_front[MAX_WIDTH * MAX_HEIGHT];
  GLfloat z_buffer_x_back[MAX_WIDTH * MAX_HEIGHT];
  GLfloat z_buffer_y_back[MAX_WIDTH * MAX_HEIGHT];
  GLfloat z_buffer_z_back[MAX_WIDTH * MAX_HEIGHT];

  GLubyte *blue_buffer;
  void read_screen_buffer(GLubyte *buffer_p);

  void fill_voxels();  // for carve_voxelize

  int step_by_step;
  int dilated;

  
};  // Voxelizer class
}

#endif


