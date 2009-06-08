//						       		-*- C++ -*-
// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.

/// \file python-utils.ixx
/// \brief Utilities for Python wrappers.

/// Generate a helper for the conversion to int.
%define generate__int__(Type)
%extend Type
{
  int __int__() const { return *$self; }
}
%enddef // !generate__int__

/// Generate a helper for the conversion to string.
%define generate__str__(Type)
%{
#include <cstring>
#include <string>
#include <sstream>
%}

%extend Type
{
  char* __str__() const
  {
    std::ostringstream s;
    s << *$self;
    // FIXME: This is admittedly ugly; can't we use std::string as
    // return type?  See Swig's manual.
    return strdup(s.str().c_str());
  }
}
%enddef // !generate__str__
