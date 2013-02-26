//								-*- C++ -*-
// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
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
/// \brief A simple wrapping of mln::image2d<mln::value::int_u8> along
/// with some algorithms.

%module image2d_int_u8

%import "int_u8.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/value/int_u8.hh"
%}

%include "image2d.ixx"
instantiate_image2d(image2d_int_u8, mln::value::int_u<8>)

%include "pgm.ixx"
%template(load) mln::io::pgm::load< mln::value::int_u<8> >;
%template(save) mln::io::pgm::save< mln::image2d< mln::value::int_u<8> > >;

%include "fill.ixx"
%template(fill) mln::data::fill< mln::image2d< mln::value::int_u<8> > >;

%include "println.ixx"
%template(println) mln::debug::println< mln::image2d< mln::value::int_u<8> > >;

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

%include "morpho.ixx"

/* FIXME: Can't we use `mln::value::int8' directly here (as we can
   use `mln::window2d', which is a really just typedef for
   `mln::window< mln::dpoint_<mln::grid::square, int> >')?

   2008-08-11: Probably, but we should %include "mln/value/int_u8.hh"
   then.  */
instantiate_morpho(mln::image2d< mln::value::int_u<8> >,
		   mln::window2d,
		   mln::neighb2d)

/*---------------------------------------.
| image2d<int_u8> and image2d<int_u24>.  |
`---------------------------------------*/

instantiate_meyer_wst(meyer_wst24,
		      mln::value::int_u<24>,
		      mln::image2d< mln::value::int_u<8> >,
		      mln::neighb2d)

// FIXME: Rearrange and move this elsewhere.
instantiate_image2d(image2d_int_u24, mln::value::int_u<24>)

%template(println24) mln::debug::println<
  mln::image2d< mln::value::int_u<24> >
  >;
