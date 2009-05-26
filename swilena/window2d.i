//                                                              -*- C++ -*-
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

/// \file window2d.i
/// \brief A wrapper of mln::window2d.

%module window2d

%import "dpoint2d.i"

%{
#include "mln/core/alias/window2d.hh"
%}

%include "mln/core/macros.hh"

%include "mln/core/concept/window.hh"
%include "mln/core/internal/window_base.hh"
%include "mln/core/window.hh"

%include "mln/core/alias/window2d.hh"

// Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D insertions for an mln::window2d).
%ignore mln::window< mln::dpoint<mln::grid::square, mln::def::coord > >::
insert(const typename mln::dpoint<mln::grid::square, mln::def::coord>::coord&);
%ignore mln::window< mln::dpoint<mln::grid::square, mln::def::coord > >::
insert(const typename mln::dpoint<mln::grid::square, mln::def::coord>::coord&,
       const typename mln::dpoint<mln::grid::square, mln::def::coord>::coord&,
       const typename mln::dpoint<mln::grid::square, mln::def::coord>::coord&);

/* Milena's `mln::def::coord' is a typedef on `short'.  However, this
   is unconvenient when using Python's builtin integers, since it will
   not implicitly convert them to `short'.  Therefore, we provide an
   extra method to allow the insertion of an mln::window2d from two
   `int's.  */
%extend mln::window< mln::dpoint<mln::grid::square,mln::def::coord > >
{
  void insert(int row, int col)
  {
    /* Explicitly convert ROW and COL to `mln::def::coord' (`short')
       to delegate the call.  */
    $self->insert(static_cast<mln::def::coord>(row),
		  static_cast<mln::def::coord>(col));
  }
}

// Instantiate base classes of mln::window2d so that Swig knows it
// derives from mln::Window.
%template() mln::internal::window_base<
  mln::dpoint<mln::grid::square, mln::def::coord >,
  mln::window< mln::dpoint<mln::grid::square, mln::def::coord > >
  >;
 // Instantiate mln::window2d.
%template(window2d)
mln::window< mln::dpoint<mln::grid::square,mln::def::coord > >;
