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

/// \file box2d.i
/// \brief A wrapper of mln::box2d.

%module box2d

%include "box.ixx"

%import box2d_piter.i

%{
#include "mln/core/alias/box2d.hh"
%}

%include "mln/core/alias/box2d.hh"

%extend mln::box< mln::point<mln::grid::square, mln::def::coord> >
{
  // Provide a ctor creating an `mln::box2d' from two `int's.
  mln::box< mln::point< mln::grid::square, mln::def::coord> > (int nrows,
							       int ncols)
  {
    return new mln::box<mln::point2d>(nrows, ncols);
  }

  unsigned nrows() const
  {
    /* FIXME: This is the exact content of box_impl_<2, C, E>::nrows.
       Wrapping box_impl_<2, C, E> might save us this extension.  */
    return mln::internal::force_exact<mln::box2d>(*$self).bbox().len(0);
  }

  unsigned ncols() const
  {
    /* FIXME: This is the exact content of box_impl_<2, C, E>::ncols.
       Wrapping box_impl_<2, C, E> might save us this extension.  */
    return mln::internal::force_exact<mln::box2d>(*$self).bbox().len(1);
  }
}

instantiate_box(box2d, mln::point<mln::grid::square, mln::def::coord>)
