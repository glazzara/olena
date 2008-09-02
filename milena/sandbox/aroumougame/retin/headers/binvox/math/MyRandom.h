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
// $Id: MyRandom.h,v 1.1 2000/10/12 19:52:41 min Exp $
//

#ifndef __MYRANDOM_H
#define __MYRANDOM_H

#include <stdlib.h>
#include "binvox/math/common.h"


namespace binvox {


class MyRandom {

public:

  MyRandom() {};
  ~MyRandom() {};

  static Float uniform(Float min, Float max);
  static Float gaussian();

  static void seed(unsigned int new_seed) { srand(new_seed); };


private:


};  // class MyRandom
}

#endif


