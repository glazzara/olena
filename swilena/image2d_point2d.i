//								-*- C++ -*-
// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

/// \file image2d_point2d.i
/// \brief A simple wrapping of mln::image2d<mln::point2d> along
/// with some algorithms.

%module image2d_point2d

%import "point2d.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/core/alias/point2d.hh"
%}

%include concat.ixx

%include "image2d.ixx"
instantiate_image2d(image2d_point2d,
		    concat2(mln::point< mln::grid::square, mln::def::coord >))

%include "fill.ixx"
%template(fill)
mln::data::fill< mln::image2d< mln::point< mln::grid::square,
					   mln::def::coord> > >;

%include "println.ixx"
%template(println)
mln::debug::println< mln::image2d< mln::point< mln::grid::square,
					       mln::def::coord> > >;
