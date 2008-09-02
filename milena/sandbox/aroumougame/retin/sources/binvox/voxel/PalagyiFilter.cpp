//
// thinvox, a binary voxel thinning program
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
// $Id: PalagyiFilter.cc,v 1.5 2006/02/18 11:15:43 min Exp min $
//

#include "binvox/voxel/PalagyiFilter.h"
#include <iostream>
#include <vector>
using namespace binvox;
using namespace std;

int PalagyiFilter::order[8] = {2, 5, 3, 4, 1, 6, 0, 7};





PalagyiFilter::PalagyiFilter(Voxels& voxels_ref) :
  VoxelFilter(voxels_ref)
  //,  voxel_dt(voxel_dt_ref)
{
  set_index = 0;

  //
  // templates initialization (USW is already there)
  //
  for(int i=0; i < 8; i++) template_sets[i].convert_type(i);

}  // constructor



PalagyiFilter::~PalagyiFilter()
{

}  // destructor



void
PalagyiFilter::apply()
{
  cout << "PalagyiFilter::apply" << endl;

  int nr_deleted;
  int total_deleted = 0;
  int nr_steps = 0;
  set_index = 0;
  
  do {
    nr_deleted = 0;
    for(int i=0; i < 8; i++) {
      nr_deleted += apply_template_set(set_index);

      set_index = (set_index + 1) % 8;
      cout << "\rStep " << nr_steps << ", direction " << i << ", deleted " << nr_deleted << " voxels        " << flush;
      total_deleted += nr_deleted;
    }
    nr_steps++;
  } while (nr_deleted);
  cout << "\rDeleted a total of " << total_deleted << " voxels in " << nr_steps << " steps" << endl;

  voxels.init_types(SKELETON);
  //  voxel_dt.compute_skeleton_dt();  // is this necessary? check code below
  // maybe just step_boundary in VoxelBoundary is sufficient

}  // PalagyiFilter::apply



static vector<int>to_be_deleted;

int
PalagyiFilter::apply_template_set(int direction)
{
  Templates& ts = template_sets[order[direction]];
  
  VoxelType nb[27];
  int nr_templates = ts.get_nr_templates();
  int size = voxels.get_size();
  
  for(int i=0; i < size; i++) {
    if (voxels[i]) {
      // get neighbours
      for(int j=0; j < 27; j++) nb[j] = voxels.get_neighbour(i, j);

      int match = 0;
      int template_index = 0;
      while (!match && (template_index < nr_templates)) {
	match = ts[template_index].match(nb);
	if (!match) template_index++;
      }  // while

      if (match) to_be_deleted.push_back(i);
    }  // if voxel
  }  // for each voxel

  //
  // delete marked voxels, update dt
  //
//   ChamferMetric& cm = voxel_dt.get_chamfer_metric();

//   int nr_to_be_deleted = to_be_deleted.size();
//   for(int i=0; i < nr_to_be_deleted; i++) {
//     //    cout << i << "/" << nr_to_be_deleted << endl;
    
//     //
//     // update distance transform here
//     //
//     // check neighbours
//     // choose minimum of neighbour distance tranform + 3/4/5
//     //
//     int index = to_be_deleted[i];
//     int minimum = MaxInt;
//     for(int j=0; j < 6; j++) {
//       int nb_index = voxels.get_face_neighbour_index(index, j);
//       if (!voxels[nb_index]) {
// 	if ((voxel_dt[nb_index] + cm.face_distance()) < minimum)
// 	  minimum = voxel_dt[nb_index] + cm.face_distance();
//       }
//     }
//     for(int j=0; j < 12; j++) {
//       int nb_index = voxels.get_edge_neighbour_index(index, j);
//       if (!voxels[nb_index]) {
// 	if ((voxel_dt[nb_index] + cm.edge_distance()) < minimum)
// 	  minimum = voxel_dt[nb_index] + cm.edge_distance();
//       }
//     }
//     for(int j=0; j < 8; j++) {
//       int nb_index = voxels.get_vertex_neighbour_index(index, j);
//       if (!voxels[nb_index]) {
// 	if ((voxel_dt[nb_index] + cm.vertex_distance()) < minimum)
// 	  minimum = voxel_dt[nb_index] + cm.vertex_distance();
//       }
//     }
//     voxel_dt[index] = minimum;
//     voxels[index] = 0;
//   }  // for, delete marked voxels

  int nr_to_be_deleted = to_be_deleted.size();
  for(int i=0; i < nr_to_be_deleted; i++) voxels[to_be_deleted[i]] = 0;
  to_be_deleted.clear();
    
  return nr_to_be_deleted;
  
}  // apply_template_set



void
PalagyiFilter::apply_once()
{
  cout << "PalagyiFilter::apply_once, set_index: " << set_index << endl;
  apply_template_set(set_index);
  set_index = (set_index + 1) % 8;

}  // PalagyiFilter::apply_once






