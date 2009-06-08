//						       		-*- C++ -*-
// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file intp.ixx
/// \brief A wrapper of int pointers.

%module intp

%include "cpointer.i"

/* FIXME: Wrapping a class interface around an "int *" using

     %pointer_class(int, intp);

   would be better (see http://www.swig.org/Doc1.3/Library.html#Library_nn4).
   We might be able to add extra methods (like __str__) this way.  */

/* Create some functions for working with "int *" */
%pointer_functions(int, intp);
