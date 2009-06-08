//						       		-*- C++ -*-
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

/// \file image2d.i
/// \brief A simple wrapping of mln::image2d<int> along with some
/// algorithms.

%module image2d_int

%include "intp.ixx"
%include "image2d.ixx"
instantiate_image2d(image2d_int, int)

%include "fill.ixx"
%template(fill) mln::data::fill< mln::image2d<int> >;

%include "println.ixx"
%template(println) mln::debug::println< mln::image2d<int> >;

%import "window2d.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/core/alias/window2d.hh"
%}

%include "morpho.ixx"
instantiate_erosion(erosion, mln::image2d<int>, mln::window2d)
instantiate_dilation(dilation, mln::image2d<int>, mln::window2d)
