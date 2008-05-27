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
// $Id: common.cc,v 1.2 2001/03/19 19:30:07 min Exp min $
//

#include "binvox/math/common.h"
#include <math.h>
#include <iostream>


//
// support function for qsort in Line::distance and Quad::distance
//
int compare(const void *a_ptr, const void *b_ptr)
{
  Float a = *((Float *) a_ptr);
  Float b = *((Float *) b_ptr);
  
  if (a < b) return -1;
  else if (a == b) return 0;
  else return 1;
}  // compare, support function for qsort



#if (!LINUX || (__GNUC__ < 3))

int my_round(Float value)
{
  return (int)(floor(value + 0.5));

}  // my_round

#else

int my_round(Float value)
{
  return (int) round(value);
}

#endif



unsigned short swap_byte_order(unsigned short n)
{
  byte lower = n & 0xff;
  byte higher = (n & 0xff00) >> 8;

  return lower * 256 + higher;

}  // swap_byte_order for 2 byte unsigned short



double swap_byte_order(double d)
{
  byte *d_p = (byte *) &d;
  byte *e_p = d_p + 7;

  for(int i=0; i < 4; i++) {
    byte t = *d_p;
    *d_p = *e_p;
    *e_p = t;
    d_p++;
    e_p--;
  }  // for, reverse bytes in double

  return d;
  
}  // swap_byte_order for 8 byte double



Float delta_wrap(Float a, Float b, Float limit)
{
  Float d1, d2;
  if (a > b) {
    d1 = a - b;
    d2 = (limit - a) + b;
  }
  else {
    d1 = b - a;
    d2 = (limit - b) + a;
  }
  if (d1 < d2) return d1;
  else return d2;

}  // delta_wrap

  
    
Float fix_angle(Float angle)
{
  while (angle > 2 * M_PI) angle -= (M_PI * 2);
  while (angle < 0) angle += (M_PI * 2);
  return angle;

}  // fix_angle



Float my_atan(Float dx, Float dy)
{
  Float angle;
  if (dx > 0) angle = atan(dy / dx);
  else {
    if (dx < 0) {
      if (dy > 0) angle = M_PI + atan(dy / dx);
      else angle = atan(dy / dx) - M_PI;
    }
    else {
      if (dy > 0) angle = M_PI / 2;
      else angle = -M_PI / 2;
    }
  }
  return angle;

}  // my_atan


      
