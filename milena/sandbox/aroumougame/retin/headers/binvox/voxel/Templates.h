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
// $Id: Templates.h,v 1.2 2000/12/21 23:25:49 min Exp $
//

#ifndef __TEMPLATES_H
#define __TEMPLATES_H

#include "binvox/voxel/Template.h"
#include <vector>

static const int UNW = 0;
static const int UNE = 1;
static const int USW = 2;
static const int USE = 3;
static const int DNW = 4;
static const int DNE = 5;
static const int DSW = 6;
static const int DSE = 7;

static const int UD_MASK = 4;
static const int NS_MASK = 2;
static const int WE_MASK = 1;

using namespace std;

namespace binvox {


class Templates
{

public:

  Templates();
  ~Templates() {}

  void convert_type(int new_type);

  Template& operator[](int index) { return T[index]; }
  int get_nr_templates() { return nr_templates; }

  
private:

  int type;
  void init_usw();
  void init_usw_base_templates();
  int already_present(Template& t);
  void add(Template& t);
  
  vector<Template> T;
  int nr_templates;
  

};  // Templates class
}

#endif

  
