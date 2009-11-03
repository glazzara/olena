/* Copyright (C) 2005 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#ifndef DYN_NAME_OF_CC
# define DYN_NAME_OF_CC

# include "name_of.hh"

# ifndef NO_COMPILER_DEMANGLING

extern "C" {
#  include "demangle.h"
}

#  define DEMANGLE_OPTIONS DMGL_VERBOSE | DMGL_TYPES | DMGL_ANSI | DMGL_PARAMS

std::string demangle(const char* name)
{
  std::string result(cplus_demangle(name, DEMANGLE_OPTIONS));
  size_t pos;
  if ((pos = result.find("char_traints")) != std::string::npos)
    result.replace(pos, 12, "char_traits ");
  return result;
}

# endif

#endif // ! DYN_NAME_OF_CC
