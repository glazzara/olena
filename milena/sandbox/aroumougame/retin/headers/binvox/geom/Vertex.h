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
// $Id: Vertex.h,v 1.10 2004/12/21 18:04:15 min Exp $
//

#ifndef __Vertex_h
#define __Vertex_h

#include <vector>
#include "binvox/math/Vector.h"
#include "binvox/geom/geom_defs.h"


namespace binvox {


class Vertex
{

public:

  Vertex();
  Vertex(Vector new_v);
  Vertex(Float x, Float y, Float z);
  ~Vertex();

  Vector& get_vector() { return v; }
  void set_vector(Vector new_v) { v = new_v; }
  void set_vector(Float x, Float y, Float z) { v.set(x, y, z); }


private:

  Vector v;
  
};  // Vertex class

}
#endif


