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
// $Id: Globals.h,v 1.1 2004/12/21 18:04:15 min Exp $
//

#ifndef __Globals_h
#define __Globals_h


namespace binvox {


class Globals
{

public:

  Globals();
  ~Globals();

  static char *UG_SUBDIR;
  static char *GRAPH_SUBDIR;
  static char *OFF_SUBDIR;
  static char *RAW_SUBDIR;
  static char *DXF_SUBDIR;
  static char *PTS_SUBDIR;
  static char *INFO_SUBDIR;
  
  
private:


};  // Globals class
}

#endif


