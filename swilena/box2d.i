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

%import box2d_piter.i

%{
#include "mln/core/site_set/box.hh"
#include "mln/core/alias/box2d.hh"
%}

%include "mln/core/macros.hh";
%include "mln/metal/is_unqualif.hh";

%include "mln/core/site_set/box.hh";
%include "mln/core/alias/box2d.hh";

/* Swig tries to wrap everything by default; prevent it from wrapping
   invalid methods (1D and 3D ctors for an mln::box2d).  Moreover, as
   Python does not automatically convert `int's to `short's, we cannot
   use the ctor taking two `mln::def::coord's (i.e. `short's)
   either.  */
/* FIXME: Can't we simplify these directives, i.e. use `point2d'
   directly?  Maybe we could use mln_coord()?  */
%ignore mln::box< mln::point<mln::grid::square, mln::def::coord> >
::box(typename mln::point<mln::grid::square, mln::def::coord>::coord);

%ignore mln::box< mln::point<mln::grid::square, mln::def::coord> >
::box(typename mln::point<mln::grid::square, mln::def::coord>::coord,
      typename mln::point<mln::grid::square, mln::def::coord>::coord);

%ignore mln::box< mln::point<mln::grid::square, mln::def::coord> >
::box(typename mln::point<mln::grid::square, mln::def::coord>::coord,
      typename mln::point<mln::grid::square, mln::def::coord>::coord,
      typename mln::point<mln::grid::square, mln::def::coord>::coord);

%extend mln::box< mln::point<mln::grid::square, mln::def::coord> >
{
  // Provide a ctor creating an `mln::box2d' from two `int's.
  mln::box< mln::point< mln::grid::square, mln::def::coord> > (int nrows,
							       int ncols)
  {
    return new mln::box<mln::point2d>(nrows, ncols);
  }

  // FIXME: Shorten using macros...
  mln::box_fwd_piter_< mln::point<mln::grid::square, mln::def::coord> >
  __iter__() const
  {
    mln::box_fwd_piter_<mln::point2d> p(*$self);
    p.start();
    return p;
  }

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

%template(box2d) mln::box< mln::point<mln::grid::square, mln::def::coord> >;
