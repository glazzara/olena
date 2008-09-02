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
// A class for identifying a mesh file by its extension,
// and creating the appropriate <type>MeshFile class
//

#include "binvox/geom/MeshFileIdentifier.h"
#include "binvox/geom/ObjMeshFile.h"
#include "binvox/geom/OffMeshFile.h"
#include "binvox/geom/PlyMeshFile.h"
#include "binvox/geom/WrlMeshFile.h"

using namespace binvox;



const int MeshFileIdentifier::UG = 0;
const int MeshFileIdentifier::OFF = 1;
const int MeshFileIdentifier::RAW = 2;
const int MeshFileIdentifier::DXF = 3;
const int MeshFileIdentifier::XGL = 4;
const int MeshFileIdentifier::TRI = 5;
const int MeshFileIdentifier::MESH = 6;
const int MeshFileIdentifier::INFO = 7;
const int MeshFileIdentifier::OBJ = 8;
const int MeshFileIdentifier::POV = 9;
const int MeshFileIdentifier::VTS = 10;
const int MeshFileIdentifier::VRML = 11;
const int MeshFileIdentifier::BREP = 12;
const int MeshFileIdentifier::PLY = 13;
const int MeshFileIdentifier::PTS = 14;
const int MeshFileIdentifier::JOT = 15;
const int MeshFileIdentifier::POLY = 16;
const int MeshFileIdentifier::RAY = 17;
const int MeshFileIdentifier::DEF = 18;
const int MeshFileIdentifier::TGL = 19;
const int MeshFileIdentifier::BPTS = 20;
const int MeshFileIdentifier::GTS = 21;
const int MeshFileIdentifier::VTK = 22;
static const int NR_EXTENSIONS = 23;

const string MeshFileIdentifier::extensions[] = {"ug", "off", "raw",
						 "dxf", "xgl", "tri",
						 "mesh", "info", "obj",
						 "pov", "vts", "wrl",
						 "brep", "ply", "pts",
						 "jot", "poly", "ray",
						 "def", "tgl", "bpts",
						 "gts", "vtk"};


MeshFileIdentifier::MeshFileIdentifier(Mesh& mesh_ref) :
  MeshRef(mesh_ref)
{

}  // constructor



MeshFileIdentifier::~MeshFileIdentifier()
{

}  // destructor



int
MeshFileIdentifier::get_filetype(string extension)
{
  int filetype = -1;
  for(int i=0; i < extension.length(); i++) extension[i] |= 0x20;  // lowercase it first
  
  for(int i=0; (i < NR_EXTENSIONS) && (filetype == -1); i++) {
    if (extension.compare(extensions[i]) == 0) filetype = i;
  }
  
  return filetype;
  
}  // MeshFileIdentifier::get_filetype




MeshFile *
MeshFileIdentifier::create_mesh_file(string filespec)
{
  //  cout << "MeshFileIdentifier::*create_mesh_file" << endl;
  
  string::size_type dot_pos = filespec.rfind('.');
  string extension = filespec.substr(dot_pos + 1);
  //  filespec.erase(dot_pos);
  
  cout << "  filename: [" << filespec << "], extension: [" << extension << "]" << endl;

  MeshFile *meshfile_p = 0;
  int filetype = get_filetype(extension);
  switch(filetype) {
    //    case UG: result = load_ug(); break;
    case OBJ: meshfile_p = new ObjMeshFile(mesh, filespec); break;
	case OFF: meshfile_p = new OffMeshFile(mesh, filespec); break;
//     case RAW: result = load_raw(); break;
//     case DXF: result = load_dxf(); break;
//     case XGL: result = load_xgl(); break;
//     case TRI: result = load_tri(); break;
//     case POV: result = load_pov(); break;
//     case MESH: result = load_mesh(); break;
//     case VTS: result = load_vts(); break;
	case VRML: meshfile_p = new WrlMeshFile(mesh, filespec); break;
//     case BREP: result = load_brep(); break;
    case PLY: meshfile_p = new PlyMeshFile(mesh, filespec); break;
//     case JOT: result = load_jot(); break;
//     case POLY: result = load_poly(); break;
//     case TGL: result = load_tgl(); break;
//     case BPTS: result = load_bpts(); break;
//     case GTS: result = load_gts(); break;
//     case VTK: result = load_vtk(); break;
    default:
		cout << "error: cannot load this filetype " << filetype << endl;
      return 0;
  }

  return meshfile_p;
  
}  // MeshFileIdentifier::*create_mesh_file
