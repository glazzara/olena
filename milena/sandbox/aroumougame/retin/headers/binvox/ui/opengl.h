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
// $Id: opengl.h,v 1.2 2002/10/30 19:20:13 min Exp min $
//

#ifndef __OPENGL_H
#define __OPENGL_H

#define OPENGL 1

#if (IRIX || LINUX)
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif
#if (WINDOWS_NT)
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif





int gui_gl_error(const char *note);


#endif
