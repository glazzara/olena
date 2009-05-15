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

/// \file dpoint2d.i
/// \brief A wrapper of mln::dpoint2d.

%module dpoint2d

%{
#include "mln/core/dpoint.hh"
#include "mln/core/alias/dpoint2d.hh"
%}

%include "mln/core/dpoint.hh";
%include "mln/core/alias/dpoint2d.hh";

// Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D ctors for a point2d).
%ignore mln::dpoint<mln::grid::square,mln::def::coord>::dpoint(const literal::one_t&);
%ignore mln::dpoint<mln::grid::square,mln::def::coord>::operator=(const literal::one_t&);
%ignore mln::dpoint<mln::grid::square,mln::def::coord>::dpoint(mln::def::coord);
%ignore mln::dpoint<mln::grid::square,mln::def::coord>::dpoint(mln::def::coord,
							       mln::def::coord,
							       mln::def::coord);

%template(dpoint2d) mln::dpoint<mln::grid::square, mln::def::coord>;
