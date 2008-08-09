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
/// \brief A simple wrapping of mln::image2d<mln::value::int_u8> along
/// with some algorithms.

%module image2d_int_u8

%import "int_u8.i"
%include "image2d.ixx"
instantiate_image2d(image2d_int_u8, mln::value::int_u<8>)

%include "pgm.ixx"
%template(load) mln::io::pgm::load< mln::value::int_u<8> >;
%template(save) mln::io::pgm::save< mln::image2d< mln::value::int_u<8> > >;

%include "fill.ixx"
%template(fill) mln::level::fill< mln::image2d< mln::value::int_u<8> > >;

%include "println.ixx"
%template(println) mln::debug::println< mln::image2d< mln::value::int_u<8> > >;

%import "window2d.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/core/window2d.hh"
%}

%import "neighb2d.i"
// FIXME: The import directive does not include the `%{ ... %}' clauses.
%{
#include "mln/core/neighb2d.hh"
%}

%include "morpho.ixx"

// Explicit instantiation of this trait for the return type of
// mln::morpho::dilation and mln::morpho::erosion.
%template() mln::trait::concrete< mln::image2d< mln::value::int_u<8> > >;
/* FIXME: Can't we use `mln::value::int8' directlt here (as we can
   use `mln::window2d', which is a really just typedef for
   `mln::window< mln::dpoint_<mln::grid::square, int> >')?  */
%template(dilation) mln::morpho::dilation< mln::image2d< mln::value::int_u<8> >,
					   mln::window2d >;
%template(erosion) mln::morpho::erosion< mln::image2d< mln::value::int_u<8> >,
					 mln::window2d >;

// Explicit instantiation of this trait for the return type of
// mln::morpho::meyer_wst.
%template() mln::trait::ch_value< mln::image2d< mln::value::int_u<8> >,
				  mln::value::int_u<32> >;
%template(meyer_wst) mln::morpho::meyer_wst<
  mln::value::int_u<32>,
  mln::image2d< mln::value::int_u<8> >,
  mln::neighb2d
  >;


/*-------------------.
| image2d<int_u32>.  |
`-------------------*/

// FIXME: Rearrange and move this elsewhere.
instantiate_image2d(image2d_int_u32, mln::value::int_u<32>)

%template(println32) mln::debug::println<
  mln::image2d< mln::value::int_u<32> >
  >;
