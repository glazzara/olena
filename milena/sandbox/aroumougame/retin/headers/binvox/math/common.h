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
#ifndef __COMMON_H
#define __COMMON_H





typedef double Float;
typedef short Short;
typedef unsigned char byte;



#if WINDOWS_NT

#include <float.h>
#include <limits.h>

const double MaxFloat = DBL_MAX;
const double MinFloat = DBL_MIN;
// const double M_PI = 3.14159265358979323846;
// const double M_E = 2.7182818284590452354;
const int MaxInt = INT_MAX;

//
// trick to make Windows accept ANSI for scoping rule
//
#define for if(0) ; else for

#endif

#if LINUX 

#ifndef MAC_OSX

#include <values.h>
const double MaxFloat = MAXDOUBLE;
const double MinFloat = MINDOUBLE;
const int MaxInt = MAXINT;

#else

#include <float.h>
#include <i386/limits.h>

const double MaxFloat = __DBL_MAX__;
const double MinFloat = __FLT_MAX__;
const int MaxInt = INT_MAX;

#endif
#endif



//
// qsort support function for Float
//
int compare(const void *a_ptr, const void *b_ptr);

int my_round(Float value);

unsigned short swap_byte_order(unsigned short n);
double swap_byte_order(double d);

inline int minimum(int a, int b) { if (a < b) return a; else return b; }

Float delta_wrap(Float a, Float b, Float limit);
Float fix_angle(Float angle);
Float my_atan(Float dx, Float dy);


#endif


