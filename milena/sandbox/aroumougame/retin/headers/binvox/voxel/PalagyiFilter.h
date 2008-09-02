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
// $Id: PalagyiFilter.h,v 1.2 2001/01/20 02:38:28 min Exp $
//

#ifndef __PalagyiFilter_h
#define __PalagyiFilter_h

#include "binvox/voxel/VoxelFilter.h"
//#include "VoxelDT.h"
#include "binvox/voxel/Templates.h"

class PalagyiControl;

namespace binvox {



class PalagyiFilter : public VoxelFilter
{

public:

  PalagyiFilter(Voxels& voxels_ref);
  ~PalagyiFilter();

  void apply();
  void apply_once();
  
  friend class PalagyiControl;

  string get_name() { return "PalagyiFilter"; }
  
  
private:

  static int order[8];

  int apply_template_set(int direction);
  
  //  VoxelDT& voxel_dt;
  
  Templates template_sets[8];
  int set_index;
  
  
};  // PalagyiFilter class
}

#endif


