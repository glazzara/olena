							// -*- C++ -*-

/* Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)

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

/// \file
/// \brief A wrapper of libdyn.

%module dyn

%include std_string.i

%{
#include "dyn-all.hh"
%}

// Ignore global objects causing trouble to swig.
%ignore dyn::logger;

%include "dyn-all.hh";

/*---------------.
| Construction.  |
`---------------*/

/* The natural, single-argument ctors of dyn::data manipulate the
   encapsulated member by reference, which is wrong when these data
   are of builtin types of the target language (e.g., Pythons'
   `int's), because we have no control on them and they can vanish at
   any moment.
    
   To prevent this, provide constructors helpers creating dyn::data
   objects manipulating data by value (copy).  */
%define generate_data_ctor(Arg, Type)
%extend dyn::data
{
  data(Arg v)
  {
    dyn::proxy_tag* dummy = 0;
    // This dummy pointer passed as second argument is required to
    // call the right ctor.
    return new dyn::data(new dyn::data_proxy_by_cpy< Type >(v), dummy);
  }
}
%enddef // !generate_data_ctor

// Shortcut to generate a ctor taking its argument by value (copy).
%define generate_data_ctor_val(Type)
generate_data_ctor(Type, Type)
%enddef

// Shortcut to generate a ctor taking its argument by (const) reference.
%define generate_data_ctor_ref(Type)
generate_data_ctor(const Type &, Type)
%enddef

// Generate dyn::data ctors using a proxy-by-value for classic types.
generate_data_ctor_val(int)
generate_data_ctor_ref(std::string)

/*--------------.
| Conversions.  |
`--------------*/

// Instantiate dyn::data explicit conversion routines for some types.
%extend dyn::data
{
  %template(convert_to_int) convert_to<int>;
  %template(convert_to_string) convert_to<std::string>;
}
