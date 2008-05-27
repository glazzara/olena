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
// $Id: View.h,v 1.6 2002/10/30 19:20:13 min Exp min $
//

#ifndef __View_h
#define __View_h

#include "binvox/ui/GLwindow.h"
#include "binvox/math/common.h"

class ViewControl;



namespace binvox {

class View
{

public:

  View();
  ~View();

  void set_win_ptr(GLwindow *new_win_ptr) { win_ptr = new_win_ptr; }
  void update();
  
  virtual void redraw();
  
  void enable() { enabled = 1; }
  void disable() { enabled = 0; }
  void toggle() { enabled = !enabled; }
  int is_enabled() { return enabled; }
  int *get_enabled_p() { return &enabled; }
  
  void set_colour(float red, float green, float blue);
  void set_colour(byte red, byte green, byte blue);
  

protected:

  GLwindow *win_ptr;
  
  int enabled;
  
  float colour[3];
  float *colour_ptr;
  
  
};  // View class
}

#endif


