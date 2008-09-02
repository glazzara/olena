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
// $Id: Buffer.cc,v 1.1 2001/10/21 03:33:01 min Exp min $
//

#include <string.h>
#include "binvox/file/Buffer.h"
using namespace binvox;

const int Buffer::MAX_BUFFERSIZE = 1024;


Buffer::Buffer()
{
  buffer = new char[MAX_BUFFERSIZE];
  buffer_index = 0;

  string_source = 0;
  string_source_length = 0;
  
}  // constructor



Buffer::~Buffer()
{
  delete[] buffer;
  
}  // destructor



void
Buffer::shift_to_front()
{
  int nr_to_move = MAX_BUFFERSIZE - buffer_index;
  memmove(buffer, buffer + buffer_index, nr_to_move);
  buffer_index = nr_to_move;
  
}  // Buffer::shift_to_front




