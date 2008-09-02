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
// $Id: mc.cc,v 1.13 2002/10/29 20:53:23 min Exp min $
//


#ifndef IRIX
#include <cstdlib>
#endif
#include <math.h>
#include <assert.h>
#include "binvox/libvox.h"
#include "binvox/geom/MeshFileIdentifier.h"
#include "binvox/voxel/Voxelizer.h"
#include "binvox/voxel/VoxelFile.h"
#include "binvox/time/WallTimer.h"
#include "binvox/ui/GLwindow.h"
using namespace binvox;

static string model_filespec = "";
static string model_prefix = "";
static string model_extension = "";
static string voxel_extension = "";
static string model_save = "";
static int voxel_type;

static int carving_only = 0;
static int dilated_carving = 0;
static int dilate = 0;
static int voting_only = 0;
static int voxel_dim = Voxelizer::DEFAULT_WIDTH;

static Mesh *mesh_p;

// used to compute reconstruction transform from
// voxel coordinates to mesh coordinates
Vector bmin, bmax;
Vector norm_translate;
Float norm_scale;

static GLwindow *ortho_win;

void voxelize()
{
	Voxels *voxels = new Voxels(voxel_dim, voxel_dim, voxel_dim, 0);
	if (!voxels) 
	{
		cout << "Error: not enough memory for voxel array" << endl;
		return;
	}
	
	Voxelizer *voxelizer = new Voxelizer(*voxels, *mesh_p, ortho_win);
	voxelizer->set_dilated(dilated_carving);
 	
	if (carving_only)
		voxelizer->carve_voxelize();
	else {
		// voting_only = 'skip_simple' parameter, so this function will not call carve_voxelize
		voxelizer->parity_vote_voxelize(voting_only);
	}

	// dilate if requested
	if (dilate) {
		for(int i=0; i < dilate; i++) {
		cout << "  dilation step " << i+1 << endl;
		voxels->dilate();
		}
	}
	
	// set mesh coord correspondence data
	voxels->set_norm_translate(norm_translate);
	voxels->set_norm_scale(norm_scale);
	
	cout << endl << "writing voxel file..." << endl;
	VoxelFile *voxel_file = new VoxelFile(*voxels, model_save);
	voxel_file->open_for_write(voxel_type);
	voxel_file->write_file();
	cout << "done" << endl << endl;
	
	delete voxel_file;
	delete voxelizer;
	delete voxels;
	return;
}  // voxelize

int load_model_file()
{

  mesh_p = new Mesh();
  mesh_p->clear();
  mesh_p->init();
  mesh_p->set_name(model_prefix);
  
  MeshFileIdentifier meshfile_identifier(*mesh_p);
  MeshFile *meshfile_p = meshfile_identifier.create_mesh_file(model_filespec);
  int result = meshfile_p->load();
  if (!result) 
  {
	  delete meshfile_p;
	  delete mesh_p;
	  return 0;
  }
  
  if (mesh_p->get_nr_faces() == 0)
  {
    cout << "Error: mesh has no polygons" << endl;
	delete meshfile_p;
	delete mesh_p;
    return 0;
  }

  if (mesh_p) {
    if (mesh_p->compute_bounding_box()) {
      norm_scale = 1.0 / mesh_p->normalize();

      mesh_p->get_bounding_box(&bmin, &bmax);

      norm_translate[X] = -0.025 * norm_scale + bmin[X];
      norm_translate[Y] = -0.025 * norm_scale + bmin[Y];
      norm_translate[Z] = -0.025 * norm_scale + bmin[Z];
      
    }
    else {
      cout << "Error: could not compute bounding box (coordinates out of range)" << endl;
	  delete meshfile_p;
	  delete mesh_p;
      return 0;  // no vertices to compute bbox from
    }
    
  }  // if a mesh was loaded

  delete meshfile_p;
  return result;

}  // load_model_file


int vox (string model_extension, int dim, string file, string prefix, string save)
{
	
	voxel_type = VoxelFile::get_filetype(model_extension);
	if (voxel_type == -1)
	{
		cout << "Error: unknown voxel file type [" << model_extension << "]" << endl;
		return 0;
	}
	
	voxel_dim = dim;
	if (voxel_dim > Voxelizer::MAX_WIDTH)
	{
		cout << "Error: max voxel grid size is " << Voxelizer::MAX_WIDTH << endl << endl;
		return 0;
	}
	
	model_filespec = file;
	model_prefix = prefix;
	model_save = save;
	cout << "loading model file..." << endl;
	if (!load_model_file()) 
	{
		cout << endl;
		return 0;
	}
	
	cout << endl << "voxel model dimension: " << voxel_dim << endl;
	ortho_win = new GLwindow("binvox", 0, 0, voxel_dim, voxel_dim);
	voxelize();

	delete mesh_p;
	delete ortho_win;
	return 1;
}

