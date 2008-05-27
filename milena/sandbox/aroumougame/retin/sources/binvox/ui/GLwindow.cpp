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
// $Id: GLwindow.cc,v 1.6 2002/10/30 19:20:06 min Exp min $
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include "binvox/ui/opengl.h"
#include <math.h>
#include "binvox/ui/GLwindow.h"

using namespace std;
using namespace binvox;
static const Float DEFAULT_FOVY = 45.0;

int GLwindow::orthographic = 0;

vector<GLwindow *> GLwindow::windows;

#define DEBUG(x)





GLwindow::GLwindow(const char *name, int x, int y, int width, int height)
{
  windows.push_back(this);
  init_window(name, x, y, width, height);

  DEBUG(cout << "GLwindow constructor(" << name << ", " << x << ", " << y << ", " << width
	<< ", " << height << "), this = " << this << ", # windows = " << windows.size() << endl);
  
}  // constructor



GLwindow::~GLwindow()
{
	vector<GLwindow *>::iterator win_it;
	for(win_it = windows.begin(); win_it != windows.end(); win_it++) {
		if (*win_it == this) {
			windows.erase(win_it);
			return;
		}
	}
}  // destructor



void
GLwindow::init_window(const char *name, int x, int y, int width, int height)
{
	my_name = const_cast<char*> (name);
  class_level = 0;
  
  this->width = width;
  this->height = height;

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(x, y);
  glutInitWindowSize(width, height);
  if(!glutGetWindow())
  {
	  window_id = glutCreateWindow(name);
  }
  else
  {
	  window_id = glutGetWindow();
	  glutSetWindow(window_id);
  }

  glutReshapeFunc(GLwindow::static_resize);
  glutDisplayFunc(GLwindow::static_redraw);
  
  cam_list.add_camera(new Camera());
  set_projection(DEFAULT_FOVY, (Float) width / (Float) height, 0.1, 50.0);
  set_camera(Vector(1.0, 1.0, 0.25),
	     Vector(0.0, 0.0, 0.0),
	     Vector(0.0, 0.0, 1.0));
  bl_x = bl_y = -0.8;  // default ortho settings
  tr_x = tr_y = 0.8;
  
  glViewport(0, 0, width, height);

  glDrawBuffer(GL_BACK);
  set_background(0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glutSwapBuffers();
  swap_buffers();

  glPolygonMode(GL_FRONT, GL_FILL);

  glDisable(GL_CULL_FACE);
  glShadeModel(GL_FLAT);
  
  glEnable(GL_DEPTH_TEST);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

}  // GLwindow::init_window



void
GLwindow::make_current()
{
  glutSetWindow(window_id);

}  // GLwindow::make_current



void
GLwindow::set_title(char *title)
{
  make_current();
  glutSetWindowTitle(title);
  
}  // GLwindow::set_title



void
GLwindow::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
}  // GLwindow::clear



void
GLwindow::swap_buffers()
{
  glutSwapBuffers();
  
}  // GLwindow::refresh



void 
GLwindow::set_camera(Vector new_eye, Vector new_lookat, Vector new_up)
{
  Camera *cur_cam = cam_list.get_cur_camera();
  cur_cam->init(new_eye, new_lookat, new_up);
  cur_cam->load_matrix();
  
}  // GLwindow::set_camera



void
GLwindow::set_projection(Float fovy, Float aspect, Float znear, Float zfar)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy, aspect, znear, zfar);

  this->fovy = fovy;
  this->aspect = aspect;
  this->znear = znear;
  this->zfar = zfar;
  
}  // GLwindow::set_projection



void
GLwindow::set_size(int width, int height)
{
  glutReshapeWindow(width, height);

}  // GLwindow::set_size



void
GLwindow::set_position(int x, int y)
{
  glutPositionWindow(x, y);
  
}  // GLwindow::set_position



void
GLwindow::resize(int width, int height)
{
  DEBUG(cout << "Glwindow::resize(" << width << ", " << height << ")" << endl;
	cout << "  this pointer: " << this << endl);

  this->width = width;
  this->height = height;

}  // GLwindow::resize



void
GLwindow::redraw()
{
  DEBUG(cout << "GLwindow::redraw" << endl);
  
  make_current();

  clear();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (orthographic) glOrtho(bl_x, tr_x, bl_y, tr_y, 0, 6);
  else gluPerspective(fovy, aspect, znear, zfar);
  gui_gl_error("GLwindow redraw 0 ");

  cam_list.get_cur_camera()->load_matrix();
  gui_gl_error("GLwindow redraw 1 ");

  if (class_level == 0) {
    swap_buffers();
  }
  
}  // GLwindow::redraw()



void
GLwindow::set_ortho_bounds(Float bl_x, Float bl_y, Float tr_x, Float tr_y)
{
  this->bl_x = bl_x;
  this->bl_y = bl_y;
  this->tr_x = tr_x;
  this->tr_y = tr_y;
    
}  // GLwindow::set_ortho_bounds



GLwindow * GLwindow::get_cur_win_p()
{
  int cur_window_id = glutGetWindow();
  DEBUG(cout << "GLwindow::get_cur_win_p for id " << cur_window_id);
  
  int nr_windows = windows.size();
  for(int i=0; i < nr_windows; i++) {
    GLwindow *win_p = windows[i];
    if (win_p->get_window_id() == cur_window_id) {
      DEBUG(cout << ", found win_p " << win_p << endl);
      return win_p;
    }
  }  // for

  DEBUG(cout << ", found no pointer, returning 0" << endl);
  return 0;

}  // GLwindow::get_cur_win_p


  
void
GLwindow::static_resize(int width, int height)
{
  GLwindow *win_p = GLwindow::get_cur_win_p();
  DEBUG(cout << "GLwindow::static_resize(" << width << ", " << height << ") for win_p " << win_p << endl);
  win_p->resize(width, height);

  glViewport(0, 0, width, height);

}  // GLwindow::resize



void
GLwindow::static_redraw()
{
  DEBUG(cout << "GLwindow::static_redraw for window [" << GLwindow::get_cur_win_p()->get_name()
	<< "] id " << GLwindow::get_cur_win_p()->get_window_id() << endl);
  GLwindow::get_cur_win_p()->redraw();
  
}  // GLwindow::static_redraw



void
GLwindow::set_idle(void (*idle)(void))
{
  glutIdleFunc(idle);
  
}  // GLwindow::set_idle



void
GLwindow::set_background(float r, float g, float b)
{
  glClearColor(r, g, b, 0);

}  // GLwindow::set_background



void
GLwindow::get_background(float *r_p, float *g_p, float *b_p)
{
  float bg[4];
  glGetFloatv(GL_COLOR_CLEAR_VALUE, bg);
  *r_p = bg[0];
  *g_p = bg[1];
  *b_p = bg[2];
  
}  // GLwindow::get_background


