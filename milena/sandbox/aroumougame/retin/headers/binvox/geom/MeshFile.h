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
// $Id: MeshFile.h,v 1.6 2007/01/19 13:22:40 min Exp min $
//

#ifndef __MeshFile_h
#define __MeshFile_h

#ifdef IRIX
#include <locale>
#endif
#include <fstream>
#include <string>
#include <stack>
#include "binvox/file/Tokenfile.h"
#include "binvox/geom/MeshRef.h"


namespace binvox {


class MeshFile : public MeshRef
{

public:

  MeshFile(Mesh& mesh_ref, string filespec);
  ~MeshFile();

  int open_for_read(int type);
  int open_for_write(int type);

  virtual int load() {}
  virtual void save() {}
  virtual void close() {}

  
protected:

  int my_filetype;
  string my_filespec;

  ifstream *input;
  ofstream *output;

  
};  // MeshFile class
}

#endif


