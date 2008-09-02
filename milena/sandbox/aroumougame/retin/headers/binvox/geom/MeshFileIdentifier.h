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

#ifndef __MeshFileIdentifier_h
#define __MeshFileIdentifier_h

#include "binvox/geom/MeshFile.h"


namespace binvox {


class MeshFileIdentifier : public MeshRef
{

public:

  MeshFileIdentifier(Mesh& mesh_ref);
  ~MeshFileIdentifier();

  MeshFile *create_mesh_file(string filespec);  



  
  static const int UG;
  static const int OFF;
  static const int RAW;
  static const int DXF;
  static const int XGL;
  static const int TRI;
  static const int MESH;
  static const int INFO;
  static const int OBJ;
  static const int POV;
  static const int VTS;
  static const int VRML;
  static const int BREP;
  static const int PLY;
  static const int PTS;
  static const int JOT;
  static const int POLY;
  static const int RAY;
  static const int DEF;
  static const int TGL;
  static const int BPTS;
  static const int GTS;
  static const int VTK;
  

private:

  static const string extensions[];

  int get_filetype(string extension);
  

};  // MeshFileIdentifier class
}

#endif


