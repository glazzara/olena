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
// $Id: Template.h,v 1.4 2001/02/04 19:39:17 min Exp $
//

#ifndef __TEMPLATE_H
#define __TEMPLATE_H

#include <iostream>
#include "binvox/voxel/typedefs.h"

static const int WHITE = 0;
static const int BLACK = 1;
static const int DONT_CARE = 2;
static const int POTENTIAL_BLACK = 3;

using namespace std;



namespace binvox {

class Template
{

public:

  Template();
  ~Template() {}


  int match(VoxelType *nb);
  
  void reflect_sw_ne();
  void reflect_us_dn();
  void reflect_uw_de();

  void flip_ud();
  void flip_ns();
  void flip_we();
  
  int operator==(Template& other);
  
  // get value
  VoxelType operator[](int index) const { return t[index]; };
  // set value
  VoxelType& operator[](int index) { return t[index]; };

  // output operator
  friend ostream& operator <<(ostream& out_stream, const Template& T);

  
private:

  void swap(int i1, int i2);
  
  VoxelType t[27];


};  // Template class
}

#endif

