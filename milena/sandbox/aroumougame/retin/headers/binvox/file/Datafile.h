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
// $Id: Datafile.h,v 1.4 2001/10/21 03:33:06 min Exp min $
//


#ifndef __DATAFILE_H
#define __DATAFILE_H

#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "binvox/math/common.h"
#include "binvox/file/Buffer.h"

// static const int MAX_STRING_BUFFERSIZE = 16384;

static const int MY_EOF = 0;

using namespace std;

namespace binvox {



class Datafile : public ifstream {

public:

  Datafile();
  ~Datafile();


  int open(char *filespec);
  int open(string filespec);
  void set_filespec(char *new_filespec) { my_filespec = new_filespec; }
  char *get_filespec() { return my_filespec; }
  void print_error_line(int warning = 0);
  
  int get_current_char() { return current_char; }
  void set_current_char(char new_char) { current_char = new_char; }

  void set_parse_source(char *new_string_source);

  void start_storage(string *new_dest);
  void suspend_storage();
  void resume_storage();
  void stop_storage();
  string *get_cur_string_dest_p() { if (string_dest.size()) return string_dest[string_dest.size() - 1]; else return 0; }
  int get_nr_string_dests() { return string_dest.size(); }
  
  void reset_line_nr() { line_nr = 0; }
  int get_line_nr() { return line_nr; }
  void set_line_nr(int new_nr) { line_nr = new_nr; }
  void print_nr_lines();

  void fill_buffer();
  void refill_buffer();
  int get_buffered_char();
  int get_source_char();
  void go_back_char();

  void read_line(char *line, int max_length = 256);
  void read_line(string& line, int max_length = 256);
  
  int eof() { return (current_char == MY_EOF); }

  int get_error_flag() { return error_flag; }
  
  void store_state();
  void restore_state();


protected:

  char *my_filespec;
  int line_nr;

  int error_flag;
  
  stack<Buffer *> buffers;
  Buffer *cur_b;
  void push_current_buffer();
  void pop_buffer();

  vector<string *> string_dest;
  vector<int> string_dest_length;
  vector<int> allow_string_store;
  
  int current_char;


};  // Datafile
}

#endif

