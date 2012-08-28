//								-*- C++ -*-
// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file box.ixx
/// \brief A wrapper of mln::box<P>.

%{
#include "mln/core/site_set/box.hh"
%}

%include "mln/core/macros.hh";
%include "mln/metal/is_unqualif.hh";

%include "mln/core/site_set/box.hh";

%include "coord.ixx"

/** Instantiate an mln::box<P> type.

    The variadic argument ``P...'' is used to allow template type
    names having commas (e.g. mln::point<grid::square, def::coord >) as
    argument.  */
%define instantiate_box(Name, P...)
/* Swig tries to wrap everything by default; prevent it from wrapping
   methods that may be invalid (1D and 3D ctors for an mln::box2d).
   Anyway, as Python does not automatically convert `int's to
   `short's, these these ctors taking `mln::def::coord's are not
   really useful.  */
%ignore mln::box< P >::box(swl_coord(P));
%ignore mln::box< P >::box(swl_coord(P), swl_coord(P));
%ignore mln::box< P >::box(swl_coord(P), swl_coord(P), swl_coord(P));

// Python interface.
#if SWIGPYTHON
%extend mln::box< P >
{
  mln::box_fwd_piter_< P >
  __iter__() const
  {
    mln::box_fwd_piter_< P > p(*$self);
    p.start();
    return p;
  }
}
#endif // !SWIGPYTHON

%template(Name) mln::box< P >;
%enddef // !instantiate_box
