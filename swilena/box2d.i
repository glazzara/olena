//								-*- C++ -*-
// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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
  box< mln::point<mln::grid::square, mln::def::coord> > (int nrows,
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
