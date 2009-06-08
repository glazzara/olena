//								-*- C++ -*-
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
    names having commas (e.g. mln::point<grid::square, def::coord) as
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
