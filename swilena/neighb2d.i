//						       		-*- C++ -*-
// Copyright (C) 2008, 2009, 2014 EPITA Research and Development
// Laboratory (LRDE).
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

/// \file neighb2d.i
/// \brief A wrapper of mln::neighb2d.

%module neighb2d

%import "dpoint2d.i"
%import "window2d.i"

%{
#include "mln/core/alias/neighb2d.hh"
%}

%include "mln/metal/is_a.hh"

%include "mln/core/macros.hh"

%include "mln/core/concept/neighborhood.hh"
%include "mln/core/internal/neighb_base.hh"
%include "mln/core/internal/neighborhood_base.hh"
%include "mln/core/neighb.hh"

%include "mln/core/alias/neighb2d.hh"

// Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D insertions for a neighb2d).
%ignore mln::neighb< mln::window< mln::dpoint< mln::grid::square,mln::def::coord > > >
::insert(const typename mln::dpoint<mln::grid::square,mln::def::coord>::coord&);

%ignore mln::neighb< mln::window< mln::dpoint< mln::grid::square, mln::def::coord > > >
::insert(const typename mln::dpoint<mln::grid::square,mln::def::coord>::coord&,
	 const typename mln::dpoint<mln::grid::square,mln::def::coord>::coord&,
	 const typename mln::dpoint<mln::grid::square,mln::def::coord>::coord&);

// Instantiate mln::neighb2d and its (directly useful) base classes.
%template(Neighborhood_neighb2d) mln::Neighborhood<
  mln::neighb<
    mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >
    >
  >;
%template(neighborhood_extra_impl_neighb2d) mln::internal::neighborhood_extra_impl<
  mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >,
  mln::neighb<
    mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >
    >
  >;
%template(neighborhood_impl_neighb2d) mln::internal::neighborhood_impl<
  mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >,
  mln::neighb<
    mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >
    >
  >;
%template(neighborhood_base_neighb2d) mln::internal::neighborhood_base<
  mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >,
  mln::neighb<
    mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >
    >
  >;
%template(neighb_base_neighb2d) mln::internal::neighb_base<
  mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >,
  mln::neighb<
    mln::window< mln::dpoint< mln::grid::square, mln::def::coord > >
    >
  >;
%template(neighb2d)
mln::neighb<
  mln::window < mln::dpoint< mln::grid::square, mln::def::coord > >
>;
