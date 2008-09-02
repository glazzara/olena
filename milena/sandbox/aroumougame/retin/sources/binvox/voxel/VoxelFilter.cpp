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
// $Id: VoxelFilter.cc,v 1.1 2000/12/16 23:12:52 min Exp $
//

#include <iostream>
#include "binvox/voxel/VoxelFilter.h"

using namespace std;

using namespace binvox;



VoxelFilter::VoxelFilter(Voxels& voxels_ref) :
  VoxelRef(voxels_ref)
{

}  // constructor



VoxelFilter::~VoxelFilter()
{

}  // destructor



void
VoxelFilter::apply()
{
  cout << "VoxelFilter::apply" << endl;

}  // VoxelFilter::apply


