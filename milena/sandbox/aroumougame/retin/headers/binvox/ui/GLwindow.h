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
// $Id: GLwindow.h,v 1.6 2002/10/30 19:20:13 min Exp min $
//

#ifndef __GLWINDOW_H
#define __GLWINDOW_H

#ifdef IRIX
#include <locale>
#endif
#include <iostream>
#include <string>
#include <vector>
#include "binvox/ui/opengl.h"
#include "binvox/math/common.h"
#include "binvox/math/Vector.h"
#include "binvox/ui/CameraList.h"

static const int DEFAULT_X = 128;
static const int DEFAULT_Y = 128;
static const int DEFAULT_WIDTH = 640;
static const int DEFAULT_HEIGHT = 480;

using namespace std;

namespace binvox {



class GLwindow {

public:

  GLwindow(const char *name, int x = DEFAULT_X, int y = DEFAULT_Y,
	   int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
  virtual ~GLwindow();

  static void set_win_ptr(GLwindow *new_ptr);

  char *get_name() { return my_name; }
  void get_dimensions(int *width_ptr, int *height_ptr) {
    *width_ptr = width, *height_ptr = height;
  }
  void make_current();
 static GLwindow *get_cur_win_p();

  void set_title(char *title);
  
  void clear();
  void swap_buffers();

  void set_idle(void (*idle)(void));

  void set_camera(Vector new_eye, Vector new_lookat, Vector new_up);
  Camera *get_cam_p() { return cam_list.get_cur_camera(); }
  CameraList *get_cam_list_p() { return &cam_list; }
  void set_projection(Float fovy, Float aspect, Float znear, Float zfar);

  int is_visible(Vector point);
  
  void set_background(float r, float g, float b);
  void get_background(float *r_p, float *g_p, float *b_p);


  virtual void redraw();
  void set_size(int width, int height);
  void set_position(int x, int y);
  virtual void resize(int width, int height);
  virtual void mouse_func(int button, int state, int x, int y) {};
  virtual void mouse_motion_func(int x, int y) {};

  int get_window_id() { return window_id; }

  static int orthographic;
  void set_ortho_bounds(Float bl_x, Float bl_y, Float tr_x, Float tr_y);
  
  
protected:

  CameraList cam_list;
  int width, height;

  int window_id;
  static vector<GLwindow *> windows;
  
  int class_level;  // 0 for GLwindow, 1 for derived class, etc.

  Float fovy, aspect, znear, zfar;  // for perspective projections
  Float bl_x, bl_y, tr_x, tr_y;  // for orthographic projections

  
private:

  void init_window(const char *name, int x, int y, int width, int height);
  char *my_name;
  
  static void static_resize(int width, int height);
  static void static_redraw();

  
};  // class GLwindow
}

#endif


