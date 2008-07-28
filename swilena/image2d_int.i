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

/// \file image2d.i
/// \brief A simple wrapping of mln::image2d<int> along with some
/// algorithms.

%module image2d_int

%include "intp.ixx"
%include "image2d.ixx"
%template(image2d_int) mln::image2d<int>;

%include "fill.ixx"
%template(fill) mln::level::fill< mln::image2d<int> >;

%include "println.ixx"
%template(println) mln::debug::println< mln::image2d<int> >;

%import "window2d.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/core/window2d.hh"
%}

%include "morpho.ixx"
%template(dilation) mln::morpho::dilation< mln::image2d<int>, mln::window2d >;
%template(erosion) mln::morpho::erosion< mln::image2d<int>, mln::window2d >;
