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
// $Id: Buffer.h,v 1.1 2001/10/21 03:33:06 min Exp min $
//

#ifndef __Buffer_h
#define __Buffer_h



namespace binvox {


class Datafile;

class Buffer
{

public:

  Buffer();
  ~Buffer();


  char& operator[](int index) { return buffer[index]; }
  char get_next_char() { return buffer[buffer_index++]; }
  char get_current_char() { return buffer[buffer_index]; }
  
  void shift_to_front();
  
  
  static const int MAX_BUFFERSIZE;

  friend class Datafile;
  friend class Tokenfile;
  
    
private:

  char *buffer;
  int buffer_index;

  char *string_source;
  int string_source_length;

  
};  // Buffer class
}

#endif


