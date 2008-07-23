//						       		-*- C++ -*-
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
#include "mln/core/point.hh"
#include "mln/core/point2d.hh"
%}

%include "mln/core/point.hh";
%include "mln/core/point2d.hh";
/* FIXME: Ignore `mln::point_<M,C>::origin' to circumvent a swig bug.
   Without this ignore clause, the generated code would trigger this
   error :

     image2d-wrap.cc:3115:144: error: macro "SWIG_as_voidptr" passed 2
     arguments, but takes just 1
     image2d-wrap.cc: In function 'PyObject* point2d_origin_get()':
     image2d-wrap.cc:3115: error: 'SWIG_as_voidptr' was not declared in this
     scope

   Check whether this bug has been fixed in a recent release of SWIG
   or if it has been reported.  */
%ignore mln::point_<mln::grid::square,int>::origin;
// Ignoring to_h_vec saves us the wrapping of 
%ignore mln::point_<mln::grid::square,int>::to_h_vec;
// Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D ctors for a point2d).
%ignore mln::point_<mln::grid::square,int>::point_(const literal::zero_t&);
%ignore mln::point_<mln::grid::square,int>::point_(const literal::one_t&);
%ignore mln::point_<mln::grid::square,int>::point_(int);
%ignore mln::point_<mln::grid::square,int>::point_(int, int, int);

%template(point2d) mln::point_<mln::grid::square,int>;
