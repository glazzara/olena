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
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file point2d.i
/// \brief A wrapper of mln::point2d.

%module point2d

%{
#include "mln/core/def/coord.hh"
#include "mln/core/point.hh"
#include "mln/core/alias/point2d.hh"
%}

%include "mln/core/point.hh";
%include "mln/core/alias/point2d.hh";

/* FIXME: Ignore `mln::point<M,C>::origin' to circumvent a swig bug.
   Without this ignore clause, the generated code would trigger this
   error :

     image2d-wrap.cc:3115:144: error: macro "SWIG_as_voidptr" passed 2
     arguments, but takes just 1
     image2d-wrap.cc: In function 'PyObject* point2d_origin_get()':
     image2d-wrap.cc:3115: error: 'SWIG_as_voidptr' was not declared in this
     scope

   Check whether this bug has been fixed in a recent release of SWIG
   or if it has been reported.  */
%ignore mln::point<mln::grid::square,mln::def::coord>::origin;
// Ignoring to_h_vec saves us the wrapping of h_vec.
%ignore mln::point<mln::grid::square,mln::def::coord>::to_h_vec;
// Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D ctors for a point2d).
%ignore mln::point<mln::grid::square,mln::def::coord>::point(const literal::zero_t&);
%ignore mln::point<mln::grid::square,mln::def::coord>::point(const literal::one_t&);
%ignore mln::point<mln::grid::square,mln::def::coord>::point(mln::def::coord);
%ignore mln::point<mln::grid::square,mln::def::coord>::point(mln::def::coord,
							     mln::def::coord,
							     mln::def::coord);

/* Milena's `mln::def::coord' is a typedef on `short'.  However, this
   is unconvenient when using Python's `range()', since this function
   produces `int' values, and Python will not implicitly convert them
   to `short'.  Therefore, we provide an extra ctor to allow the
   construction of an mln::point2d from two `int's.  */
%extend mln::point< mln::grid::square, mln::def::coord >
{
  point< mln::grid::square, mln::def::coord >(int row, int col)
  {
    // ROW and COL are implicitly converted to `short'
    return new mln::point2d(row, col);
  }
}

/* Swig doesn't wrap operator[]...  Provide row() and col() accessors
   instead (we should wrap internal::mutable_coord_impl_ instead).
   Note that these methods return `int' values instead of
   `mln::def::coord' (i.e., `short') values, to facilitate the
   communication with Python (as above).   There might be a work
   around based on typemaps.  */
%extend mln::point< mln::grid::square, mln::def::coord >
{
  int row() const { return $self->operator[](0); }
  int col() const { return $self->operator[](1); }
}

%extend mln::point< mln::grid::square, mln::def::coord >
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

%template(point2d) mln::point<mln::grid::square, mln::def::coord>;
