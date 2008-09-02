//								-*- C++ -*-
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

/// \file box2d.i
/// \brief A wrapper of mln::box2d.

%module box2d

%{
#include "mln/core/box.hh"
#include "mln/core/box2d.hh"
%}

%include "mln/core/macros.hh";

%include "mln/core/box.hh";
%include "mln/core/box2d.hh";

// Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D ctors for a box2d).
/* FIXME: Can't we simplify these directives, i.e. use `point2d' and
   `int' directly?  */
%ignore mln::box_< mln::point_<mln::grid::square, int> >::
box_(typename mln::point_<mln::grid::square, int>::coord);
%ignore mln::box_< mln::point_<mln::grid::square, int> >::
box_(typename mln::point_<mln::grid::square, int>::coord,
     typename mln::point_<mln::grid::square, int>::coord,
     typename mln::point_<mln::grid::square, int>::coord);

%extend mln::box_
{
  unsigned nrows() const
  {
    // FIXME: This is the exact content of box_impl_<2, C, E>::nrows.
    return mln::internal::force_exact<mln::box2d>(*$self).bbox().len(0);
  }

  unsigned ncols() const
  {
    // FIXME: This is the exact content of box_impl_<2, C, E>::ncols.
    return mln::internal::force_exact<mln::box2d>(*$self).bbox().len(1);
  }
}


%template(box2d) mln::box_< mln::point_<mln::grid::square, int> >;
