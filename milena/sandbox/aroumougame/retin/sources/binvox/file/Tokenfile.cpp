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
// $Id: Tokenfile.cc,v 1.3 2001/10/21 03:33:01 min Exp min $
//

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include "binvox/file/Tokenfile.h"

#define DEBUG(x)
#define DEBUG_LINE_NR(x)

using namespace std;
using namespace binvox;




//
// 'until_whitespace' is a new parameter:
//   0: read regular token, i.e. standard token, or id
//   1: read until whitespace
//
void
Tokenfile::get_token(char *token, int until_whitespace)
{
  token[0] = '\0';
  parse_error = 0;
  
  current_char = skip_whitespace();
  if (current_char == MY_EOF) return;

  // first check for a standard token
  if (get_standard_token(token)) return;

  // now we have an id or a number
  //  token[0] = current_char;
  int index = 0;
  int done = 0;
  
  do {
    current_char = get_buffered_char();
    if (is_id_char(current_char) ||
	(until_whitespace && !is_whitespace(current_char)
	 && !is_standard_token(current_char))) {
      token[index] = current_char;
      index++;
      if (index == MAX_TOKEN_LENGTH) {
	print_error_line();
	cerr << "Token too long (max length is " << MAX_TOKEN_LENGTH
	     << ")" << endl;
	parse_error = 1;
	return;
      }
    }
    else {
      done = 1;
    }
    
  } while (!done);
  token[index] = '\0';
  // if index == 0 then we read a character we don't know,
  // and want to skip it (i.e. not go back)
  if (index && (current_char != MY_EOF)) go_back_char();

  DEBUG_LINE_NR(cout << "\rLine nr " << get_line_nr() << flush);
  DEBUG(cout << ">> TOKEN: [" << token << "]" << endl);

}  // Tokenfile::get_token



void
Tokenfile::lookahead_token(char *token)
{
  refill_buffer();
  int prev_index = cur_b->buffer_index;
  int prev_char = current_char;
  int prev_line_nr = line_nr;
  int nr_dests = string_dest.size();

  int prev_string_dest_length;
  if (nr_dests) prev_string_dest_length = string_dest_length[nr_dests - 1];
  
  DEBUG(cout << "Lookahead: ");

  looking_ahead = 1;
  get_token(token);
  
  looking_ahead = 0;
  line_nr = prev_line_nr;
  current_char = prev_char;
  cur_b->buffer_index = prev_index;
  if (nr_dests) {
    //    cout << "Tokenfile::lookahead_token, got [" << token << "]" << endl;
    int cur_string_dest_length = string_dest_length[nr_dests - 1];
    int delta = (cur_string_dest_length - prev_string_dest_length);
    //    cout << "  delta = " << delta << endl;
    string *str_p = string_dest[nr_dests - 1];
    //    cout << "  before erase: " << *str_p << endl;
    str_p->erase(str_p->end() - delta, str_p->end());
    //    cout << "  after erase: " << *str_p << endl;
    string_dest_length[nr_dests - 1] = prev_string_dest_length;
  }
  
}  // Tokenfile::lookahead_token



int
Tokenfile::accept_token(const char *wanted)
{
  char token[MAX_TOKEN_LENGTH];
  get_token(token);

  if (strcmp(token, wanted) == 0) return 1;
  else {
    print_error_line();
    cerr << "Expected [" << wanted << "], read [" << token
	 << "]" << endl;
    parse_error = 1;
    return 0;
  }
}  // Tokenfile::accept_token with string parameter



int
Tokenfile::accept_token(const char wanted)
{
  char wanted_string[2];
  wanted_string[0] = wanted;
  wanted_string[1] = '\0';
  return accept_token(wanted_string);
}  // Tokenfile::accept_token with char parameter



void
Tokenfile::set_standard_tokens(char (*new_standard_tokens)[MAX_TOKEN_LENGTH])
{
  standard_tokens = new_standard_tokens;
  int index = 0;
  // find the longest token
  while (standard_tokens[index][0] != '\0') {
    int length = strlen(standard_tokens[index]);
    if (length > max_standard_token_length) max_standard_token_length = length;
    index++;
  }  // while
}  // Tokenfile::set_standard_tokens



int
Tokenfile::get_standard_token(char *token)
{
  int index = 0;
  char candidate[MAX_TOKEN_LENGTH];
  candidate[0] = '\0';
  int candidate_length = 0;
  char *buf_p = cur_b->buffer + cur_b->buffer_index;
  while (standard_tokens[index][0] != '\0') {
    int length = strlen(standard_tokens[index]);
    if (length > candidate_length) {
      if (strncmp(standard_tokens[index], buf_p, length) == 0) {
	strcpy(candidate, standard_tokens[index]);
	candidate_length = strlen(candidate);
      }
    }
    index++;
  }  // while
  if (candidate_length) {
    strcpy(token, candidate);
    for(int i=0; i<candidate_length; i++) get_buffered_char();
    current_char = ' ';

    DEBUG(cout << ">> STANDARD TOKEN: [" << token << "]" << endl);

    return 1;
  }
  return 0;
}  // Tokenfile::get_standard_token



void
Tokenfile::read_until(char until_char)
{
  char k;
  do {
    k = get_buffered_char();
  } while ((k != until_char) && (k != MY_EOF));
  
}  // Tokenfile::read_until



int
Tokenfile::is_number_char(int k)
{
  return (strchr(number_chars, k) != NULL);
}  // Tokenfile::is_number_char



void
Tokenfile::get_number(char *number_string)
{
  parse_error = 0;
  current_char = skip_whitespace();
  if (current_char == MY_EOF) return;

  number_string[0] = '\0';
  
  int index = 0;
  do {
    current_char = get_buffered_char();
    if (is_number_char(current_char)) {
      number_string[index] = current_char;
      index++;
    }
  } while (is_number_char(current_char) && !eof());
  number_string[index] = '\0';

  if (index == 0) {
    print_error_line();
    cerr << "expected number, but didn't find one" << endl;
    parse_error = 1;
    return;  // !!! ??? to avoid go_back_char?
  }
  go_back_char();

  DEBUG(cout << ">> NUMBER: [" << number_string << "]" << endl);

}  // Tokenfile::get_number



int
Tokenfile::get_int()
{
  char number_string[64];

  get_number(number_string);

  int i;
  sscanf(number_string, "%d", &i);

  return i;
  
}  // Tokenfile::get_int



Float
Tokenfile::get_float()
{
  Float f;
  char number_string[64];

  get_number(number_string);
  sscanf(number_string, "%lg", &f);

  return f;
  
}  // Tokenfile::get_float



int
Tokenfile::is_whitespace(int k)
{
  return (strchr(whitespace_chars, k) != NULL);
}  // Tokenfile::is_whitespace



int
Tokenfile::skip_whitespace()
{
  //  if (!is_whitespace(current_char)) return current_char;

  DEBUG(cout << "Tokenfile::skip_whitespace, buffer_index = " << cur_b->buffer_index << endl);

  do {
    current_char = get_buffered_char();

    if (current_char == '#') skip_comments();  // needs to improve
    if (current_char == '/') {
      char next_char = get_buffered_char();
      if (next_char == '/') skip_comments();
      else {
	go_back_char();
	current_char = '/';
      }
    }
  } while (is_whitespace(current_char) && (current_char != MY_EOF));

  if (current_char != MY_EOF) go_back_char();

  DEBUG(cout << "accepted: " << (int) current_char << " = [" << (char) current_char << "]" << endl);
  return current_char;
  
}  // Tokenfile::skip_whitespace


int
Tokenfile::is_id_char(int k)
{
  return (isalnum(k) || (k == '_') || (k == '%'));
}  // Tokenfile::is_id_char



int
Tokenfile::is_id_firstchar(int k)
{
  return (is_id_char(k) && !isdigit(k));
}  // Tokenfile::is_id_firstchar



void
Tokenfile::skip_comments()
{
  //  cout << "Tokenfile::skip_comments [";
  do {
    current_char = get_buffered_char();
    //    cout << (char) current_char;
  } while ((current_char != '\n') && (current_char != MY_EOF));
  //  cout << "]" << endl;
  
}  // Tokenfile::skip_comments



int
Tokenfile::is_standard_token(int k)
{
  int index = 0;
  while(standard_tokens[index][0] != '\0') {
    if (standard_tokens[index][0] == k) return 1;
    index++;
  }
  return 0;
  
}  // Tokenfile::is_standard_token


