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
// $Id: opengl.cc,v 1.1 2002/10/30 19:20:06 min Exp min $
//

#include <stdio.h>
#include "binvox/ui/opengl.h"



int gui_gl_error(const char *note)
{
  GLenum gle = glGetError();

  if (gle == GL_NO_ERROR)
    return 0;

  if (note != NULL) {
    fprintf(stderr, note);
    fprintf(stderr, " ");
  }
    
  switch (gle) {
    case GL_INVALID_ENUM:
      fprintf(stderr, "GL Error: Invalid enum\n");
      break;
    case GL_INVALID_VALUE:
      fprintf(stderr, "GL Error: Invalid value\n");
      break;
    case GL_INVALID_OPERATION:
      fprintf(stderr, "GL Error: Invalid operation\n");
      break;
    case GL_STACK_OVERFLOW:
      fprintf(stderr, "GL Error: Stack overflow\n");
      break;
    case GL_STACK_UNDERFLOW:
      fprintf(stderr, "GL Error: Stack underflow\n");
      break;
    case GL_OUT_OF_MEMORY:
      fprintf(stderr, "GL Error: Out of memory\n");
      break;
    default:
      fprintf(stderr, "GL Error: unknown error code\n");
  }
  return 1;
  
}  // gui_gl_error



