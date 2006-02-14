// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef METALIC_TO_STRING_HH
# define METALIC_TO_STRING_HH

# include <iostream>
# include <string>
# include <typeinfo>


/// Macro
# define mlc_to_string(Type)  mlc::internal::demangle(typeid(Type).name())


namespace mlc
{

  namespace internal
  {

    const std::string demangle(const std::string& str)
    {
      if (str == "v") return "void";
      if (str == "b") return "bool";
      if (str == "c") return "char";
      if (str == "h") return "unsigned char";
      if (str == "s") return "short";
      if (str == "t") return "unsigned short";
      if (str == "i") return "int";
      if (str == "j") return "unsigned int";
      if (str == "l") return "long";
      if (str == "m") return "unsigned long";
      if (str == "f") return "float";
      if (str == "d") return "double";
      return str;
    }

  } // end of mlc::internal

}



#endif // ! METALIC_TO_STRING_HH
