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
// $Id: VoxelRef.cc,v 1.4 2001/06/10 20:14:41 min Exp $
//

#include <iostream>
#include "binvox/voxel/VoxelRef.h"
using namespace binvox;
vector<VoxelRef *> VoxelRef::refs;

using namespace std;





VoxelRef::VoxelRef(Voxels& voxels_ref) :
  voxels(voxels_ref)
{
  VoxelRef::refs.push_back(this);

  width = height = depth = 0;
  init_dimensions();
  
}  // constructor



VoxelRef::~VoxelRef()
{
  vector<VoxelRef *>::iterator ref_it;
  for(ref_it = refs.begin(); ref_it != refs.end(); ref_it++) {
    if (*ref_it == this) {
      refs.erase(ref_it);
      return;
    }
  }  // for, find ref in refs list

  cout << "VoxelRef destructor error for " << get_name() << ", not found in refs list" << endl;

}  // destructor



void
VoxelRef::init_dimensions()
{
  size = voxels.get_size();
  wxh = voxels.get_wxh();
  prev_width = width, prev_height = height, prev_depth = depth;
  voxels.get_dimensions(&width, &height, &depth);
  if ((width != prev_width) || (height != prev_height) || (depth != prev_depth))
    dimensions_changed = 1;
  else
    dimensions_changed = 0;
  
}  // VoxelRef::init_dimensions



void
VoxelRef::check_update()
{
  //  cout << "VoxelRef::check_update" << endl;
  
  int nr_refs = refs.size();
  for(int i=0; i < nr_refs; i++) {
    if (refs[i]->needs_update()) {
      //      cout << i << ": " << refs[i]->get_name() << " needs update" << endl;
      refs[i]->init_dimensions();
      refs[i]->update();
    }
  }  // for

}  // (static) VoxelRef::check_update



void
VoxelRef::update()
{
  
}  // VoxelRef::update

