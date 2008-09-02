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
// $Id: VoxelFile.h,v 1.7 2004/10/19 11:43:27 min Exp min $
//

#ifndef __VoxelFile_h
#define __VoxelFile_h

#include <fstream>
#include <string>
#include "binvox/voxel/VoxelRef.h"

using namespace std;


namespace binvox {



class VoxelFile : public VoxelRef
{

public:

  VoxelFile(Voxels& voxels_ref, string filename);
  ~VoxelFile();

  static int get_filetype(string extension);
  
  int load(string filespec);
  int load(string filename, string extension);
  
  int open_for_read(int type);
  int open_for_write(int type);

  int write_file();
  
  void close();

  void set_with_types(int new_value) { with_types = new_value; }
  
  void set_vtk_comment_string(string new_string) { vtk_comment_string = new_string; }

  string get_name() { return "VoxelFile"; }

  
  static const int BINVOX;
  static const int VT;
  static const int MIRA;
  static const int RAWVOX;
  static const int VTK;
  
  static const string extensions[];

  
private:

  void set_filespec();
  
  int read_binvox();
  int read_vt();
  int read_mira();
  
  int write_binvox();
  int write_vt();
  int write_mira();
  int write_raw_vox();
  int write_vtk();

  int with_types;
  
  string vtk_comment_string;
  static const string DEFAULT_VTK_COMMENT_STRING;
  
  int my_filetype;
  string my_filename;
  string total_filespec;
  
  ifstream *input;
  ofstream *output;

  
};  // VoxelFile class
}

#endif


