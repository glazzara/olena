//								-*- C++ -*-
// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
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

/// \file image2d.i
/// \brief A simple wrapping of mln::image2d<mln::value::rgb8> along
/// with some algorithms.

%module image2d_rgb8

%import "rgb8.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/value/rgb8.hh"
%}

%include "image2d.ixx"
instantiate_image2d(image2d_rgb8, mln::value::rgb<8>)

%include "ppm.ixx"
%template(load) mln::io::ppm::load< mln::value::rgb<8> >;
%template(save) mln::io::ppm::save< mln::image2d< mln::value::rgb<8> > >;

%include "fill.ixx"
%template(fill) mln::data::fill< mln::image2d< mln::value::rgb<8> > >;

%include "println.ixx"
%template(println) mln::debug::println< mln::image2d< mln::value::rgb<8> > >;

%import "window2d.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/core/alias/window2d.hh"
%}

%import "neighb2d.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/core/alias/neighb2d.hh"
%}
