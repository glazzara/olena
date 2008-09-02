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
// $Id: MyRandom.cc,v 1.1 2000/10/12 19:52:39 min Exp $
//

#include <math.h>
#include "binvox/math/MyRandom.h"

using namespace binvox;



Float
MyRandom::uniform(Float min, Float max)
{
  Float r = ((Float)(rand() % (RAND_MAX + 1))) / ((Float) RAND_MAX);
  return (min + r * (max - min));
}  // MyRandom::uniform



Float
MyRandom::gaussian()
{
  // from Numerical Recipes, paragraph 7.2
  Float v1, v2, length_sq;

  do {
    v1 = uniform(-1, 1);
    v2 = uniform(-1, 1);
    length_sq = v1*v1 + v2*v2;
  } while ((length_sq > 1) || (length_sq == 0));

  Float fac = sqrt((-2 * log(length_sq))/length_sq);

  return v2*fac;
}  // MyRandom::gaussian


