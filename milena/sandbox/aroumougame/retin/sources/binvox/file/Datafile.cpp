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
// $Id: Datafile.cc,v 1.5 2001/10/21 03:33:01 min Exp min $
//

#include "binvox/file/Datafile.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#ifdef MPATROL
#include <mpatrol.h>
#endif

using namespace std;
using namespace binvox;
#define DEBUG_STORAGE(x)





Datafile::Datafile()
{
  cur_b = new Buffer();
  while (!buffers.empty()) buffers.pop();
  
  my_filespec = NULL;
  line_nr = 1;
  string_dest.clear();
  string_dest_length.clear();
  current_char = ' ';

  allow_string_store.clear();
  
}  // constructor



Datafile::~Datafile()
{
  ifstream::close();

  while(!buffers.empty()) {
    Buffer *buffer_p = buffers.top();
    delete buffer_p;
    buffers.pop();
  }
  delete cur_b;
  
}  // destructor



int
Datafile::open(char *filespec)
{
  ifstream::open(filespec, ios::in);
  int open_good = good();
  
  my_filespec = filespec;
  precision(12);
  if (open_good) fill_buffer();  // file may be so small that EOF is reached...
  current_char = 0x20;
  return (open_good);  // that's why we now use open_good...
  
}  // Datafile::open



int
Datafile::open(string filespec)
{
  return open((char *) filespec.c_str());

}  // Datafile::open with string parameter



void
Datafile::fill_buffer()
{
  cur_b->buffer_index = 0;  // !!! before the for loop...
  
  for(int i=0; i < Buffer::MAX_BUFFERSIZE; i++) {
    char k = (char) get_source_char();
    (*cur_b)[i] = k;
    if (k == MY_EOF) return;
    
  }  // for
  
}  // Datafile::fill_buffer



void
Datafile::print_error_line(int warning)
{
  if (warning) cout << "  Warning";
  else cout << "  Error";

  cout << " in " << my_filespec << " at line " << get_line_nr()
       << ":" << endl << "    ";
  
}  // Datafile::print_error_line



void
Datafile::print_nr_lines()
{
  // show number of lines in file
  string command = "wc -l ";
  command += my_filespec;
  system(command.c_str());
  
}  // Datafile::print_nr_lines



int
Datafile::get_source_char()
{
  // first check if there's a string source
  if (cur_b->string_source != NULL) {
    if (cur_b->string_source_length) {
      current_char = cur_b->string_source[0];
      cur_b->string_source++;
      cur_b->string_source_length--;
    }
    else
      current_char = MY_EOF;
  }
  // else get a character from the actual file
  else {
    current_char = get();
  }
  if (current_char == EOF) current_char = MY_EOF;
  return current_char;
  
}  // Datafile::get_source_char



void
Datafile::refill_buffer()
{
  if (cur_b->buffer_index > 1) {  // 1, not 0...
    int prev_char = current_char;

    cur_b->shift_to_front();
    
    for(int i = cur_b->buffer_index; i < Buffer::MAX_BUFFERSIZE; i++) {
      char k = get_source_char();
      cur_b->buffer[i] = k;
      if (k == MY_EOF) break;
    }
    cur_b->buffer_index = 0;
    current_char = prev_char;
  }
  
}  // Datafile::refill_buffer



int
Datafile::get_buffered_char()
{
  if (cur_b->buffer_index == Buffer::MAX_BUFFERSIZE / 2) refill_buffer();
  current_char = cur_b->get_next_char();
  if (current_char == '\n') line_nr++;
  if (current_char == MY_EOF) cur_b->buffer_index--;

  // moved this from get_source_char to here...
  int nr_dests = string_dest.size();
  if (nr_dests && allow_string_store[nr_dests - 1]) {
    int i = nr_dests - 1;
//      if (string_dest_length[i] == MAX_STRING_BUFFERSIZE) {
//        cout << "Datafile::get_source_char error: string buffer (" << MAX_STRING_BUFFERSIZE
//  	   << " bytes) not large enough" << endl;
//      }
//      else {
#if WINDOWS_NT
    string_dest[i] += current_char;
#else
    string_dest[i]->push_back(current_char);
#endif
    string_dest_length[i]++;
      //    }
  }  // if there are destination strings

  return current_char;
  
}  // Datafile::get_buffered_char



void
Datafile::go_back_char()
{
  if (cur_b->buffer_index == 0) {
    print_error_line();
    cout << "attempt to move back in buffer at start of buffer" << endl;
  }
  else {
    cur_b->buffer_index--;
    int nr_dests = string_dest.size();
    if (nr_dests && allow_string_store[nr_dests - 1]) {
      int i = nr_dests - 1;
      //      string_dest[i]--;
      string_dest[i]->erase(string_dest[i]->end() - 1); //pop_back();
      string_dest_length[i]--;
    } // if there are destination strings
    
    if (cur_b->get_current_char() == '\n') line_nr--;
  }
  
}  // Datafile::go_back_char



void
Datafile::read_line(char *line, int max_length)
{
  char k;
  int index = 0;
  
  do {
    k = get_buffered_char();
    line[index] = k;
    index++;
  } while ((k != '\n') && (index < max_length));

  line[index - 1] = '\0';
  
  // could be a DOS file, check for a CR and remove it
  if ((index > 1) && (line[index - 2] == 0x0d)) line[index - 2] = '\0';

  return;

}  // Datafile::read_line



void
Datafile::read_line(string& line, int max_length)
{
  char *char_line = new char[max_length];

  read_line(char_line, max_length);

  line = char_line;
  delete[] char_line;

}  // Datafile::read_line



void
Datafile::push_current_buffer()
{
  //  cout << "Datafile::push_current_buffer" << endl;
  
  buffers.push(cur_b);
  cur_b = new Buffer();
  
}  // Datafile::push_current_buffer



void
Datafile::pop_buffer()
{
  //  cout << "Datafile::pop_buffer" << endl;

  if (buffers.empty()) {
    cout << "Datafile::pop_buffer error: buffer stack is empty" << endl;
  }
  else {
    cur_b = buffers.top();
    buffers.pop();
  }
  
}  // Datafile::pop_buffer



void
Datafile::set_parse_source(char *new_string_source)
{
  //  cout << "Datafile::set_parse_source" << endl;
  cur_b->string_source = new_string_source;
  if (cur_b->string_source) {
    cur_b->string_source_length = strlen(cur_b->string_source);
    fill_buffer();
  }
 set_current_char(' ');
  
}  // Datafile::set_parse_source



void
Datafile::store_state()
{
  //  cout << "Datafile::store_state" << endl;

  push_current_buffer();
  
}  // Datafile::store_state



void
Datafile::restore_state()
{
  //  cout << "Datafile::restore_state" << endl;

  pop_buffer();
  current_char = cur_b->get_current_char();
  
}  // Datafile::restore_state



void
Datafile::suspend_storage()
{
  DEBUG_STORAGE(cout << "Datafile::suspend_storage" << endl);
  int index = string_dest.size() - 1;
  if (index >= 0)
    allow_string_store[index] = 0;
  
}  // Datafile::suspend_storage



void
Datafile::resume_storage()
{
  DEBUG_STORAGE(cout << "Datafile::resume_storage" << endl);
  int index = string_dest.size() - 1;
  if (index >= 0)
    allow_string_store[index] = 1;  
  
}  // Datafile::resume_storage



void
Datafile::start_storage(string *new_dest)
{
  int nr_dests = string_dest.size();
  if (nr_dests) DEBUG_STORAGE(cout << "Datafile::start_storage, prev allowed: "
			      << allow_string_store[nr_dests - 1] << endl);

  string_dest.push_back(new_dest);
  string_dest_length.push_back(0);
  allow_string_store.push_back(1);
  
}  // Datafile::start_storage



void
Datafile::stop_storage()
{
  DEBUG_STORAGE(cout << "Datafile::stop_storage" << endl);

  int i = string_dest.size() - 1;
  assert(i >= 0);
  
  int stored_len = string_dest_length[i];
  char *stored_p = (char *) string_dest[i]->c_str(); // - stored_len;
  //  string_dest[i][0] = '\0';
  //  string_dest[i]->push_back('\0');
  //  cout << "stored: " << stored_p << endl;
  
  if (i) {
    //    memcpy(string_dest[i - 1], stored_p, stored_len);
    string_dest[i - 1]->append(*string_dest[i]);
    DEBUG_STORAGE(cout << "string_dest[" << i << "] = " << *string_dest[i] << endl;
		  cout << "string_dest[" << i-1 << "] = " << *string_dest[i - 1] << endl;
		  char k;
		  cin >> k);
    
    //    string_dest[i - 1] += stored_len;
    string_dest_length[i - 1] += stored_len;
//      if (string_dest_length[i - 1] > MAX_STRING_BUFFERSIZE) {
//        cout << "Datafile::stop_storage error: overran buffer" << endl;
//        exit(1);
//      }
  }
  
  string_dest.pop_back();
  string_dest_length.pop_back();
  allow_string_store.pop_back();
  
}  // Datafile::stop_storage


