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
// $Id: Tokenfile.h,v 1.3 2001/10/21 03:33:06 min Exp min $
//

#ifndef __TOKENFILE_
#define __TOKENFILE_

#include "binvox/math/common.h"
#include "binvox/file/Datafile.h"

static const char OPEN_PAREN = '(';
static const char CLOSE_PAREN = ')';
static const char OPEN_BRACE = '{';
static const char CLOSE_BRACE = '}';
static const char OPEN_BRACKET = '[';
static const char CLOSE_BRACKET = ']';
static const char DOUBLE_QUOTE = '"';
static const char SINGLE_QUOTE = '\'';
static const char BACKSLASH = '\\';
static const char SEMI_COLON = ';';

static const int MAX_TOKEN_LENGTH = 64;

static const char *number_chars = "+-0123456789.abcdefABCDEFeExX";
static const char *default_whitespace_chars = ", \t\n\r";

static char default_tokens[][MAX_TOKEN_LENGTH] = { "\"", "'", "#", "[", "]",{0x5c}, "{", "}", ".", ";", "\0" };
static const int MAX_DEFAULT_TOKEN_LENGTH = 2;



namespace binvox {

class Tokenfile : public Datafile
{

public:

  Tokenfile() : Datafile() {
    standard_tokens = default_tokens;
    max_standard_token_length = MAX_DEFAULT_TOKEN_LENGTH;
	whitespace_chars = const_cast<char*> (default_whitespace_chars);
    looking_ahead = 0;
  }  // constructor

  ~Tokenfile() {}


  void get_token(char *token, int until_whitespace = 0);
  void lookahead_token(char *token);
  int accept_token(const char *wanted);
  int accept_token(const char wanted);
  int get_standard_token(char *token);
  int is_standard_token(int k);

  void read_until(char until_char);

  void get_number(char *number_string);

  int get_int();
  Float get_float();

  int skip_whitespace();
  int is_whitespace(int k);

  int is_id_char(int k);
  int is_id_firstchar(int k);
  int is_number_char(int k);

  void set_standard_tokens(char (*new_standard_tokens)[MAX_TOKEN_LENGTH]);
  void set_whitespace_chars(const char *new_whitespace_chars) {
	  whitespace_chars = const_cast<char*> (new_whitespace_chars);
  }

  void skip_comments();

  int get_parse_error() { return parse_error; }

  
private:

  char (*standard_tokens)[MAX_TOKEN_LENGTH];
  int max_standard_token_length;

  char *whitespace_chars;

  int looking_ahead;
  int parse_error;
  

};  // Tokenfile class
}

#endif

