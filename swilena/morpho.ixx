//								-*- C++ -*-
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

/// \file morpho.i
/// \brief Wrappers of morphological algorithms from mln::morpho.

%module morpho

/*-----------------------------------.
| Morphological dilation & erosion.  |
`-----------------------------------*/

%include "concrete.ixx"

%{
#include "mln/morpho/dilation.hh"
#include "mln/morpho/erosion.hh"
%}

%include "mln/morpho/dilation.hh"
%include "mln/morpho/erosion.hh"

%define instantiate_dilation(Name, I, W)
  // Explicit instantiation of this trait for the return type.
  %template() mln::trait::concrete< I >;
  %template(Name) mln::morpho::dilation< I, W >;
%enddef

%define instantiate_erosion(Name, I, W)
  %template() mln::trait::concrete< I >;
  %template(Name) mln::morpho::erosion< I, W >;
%enddef

/*----------------------------------.
| Morphological opening & closing.  |
`----------------------------------*/

// FIXME: Add them.

/*------------.
| Gradients.  |
`------------*/

// FIXME: Add other gradient.

%{
#include "mln/morpho/gradient.hh"
%}

%include "mln/morpho/gradient.hh"

%define instantiate_gradient(Name, I, W)
  // Explicit instantiation of this trait for the return type.
  %template() mln::trait::concrete< I >;
  %template(Name) mln::morpho::gradient< I, W >;
%enddef

/*-------------------------.
| Area closing & opening.  |
`-------------------------*/

// FIXME: Add area opening.

%{
#include "mln/morpho/closing_area.hh"
%}

%include "mln/morpho/closing_area.hh"

%define instantiate_closing_area(Name, I, N)
  %template(Name) mln::morpho::closing_area< I, N, I >;
%enddef

/*------------------------------------.
| Meyer's Watershed Transform (WST).  |
`------------------------------------*/

%include "ch_value.ixx"

%{
#include "mln/morpho/meyer_wst.hh"
%}

%include "mln/morpho/meyer_wst.hh"

%define instantiate_meyer_wst(Name, L, I, N)
  // Explicit instantiation of this trait for the return type.
  %template() mln::trait::ch_value< I, L >;
  %template(Name) mln::morpho::meyer_wst< L, I, N >;
%enddef

/*-------------------------.
| Instantiate everything.  |
`-------------------------*/

%define instantiate_morpho(I, W, N, L)
  instantiate_dilation(dilation, I, W)
  instantiate_erosion(erosion, I, W)
  instantiate_gradient(gradient, I, W)
  instantiate_closing_area(closing_area, I, N)
  // FIXME: Could we used `typename I::value' instead of `L' here, and
  // remove L from the list of arguments?
  instantiate_meyer_wst(meyer_wst, L, I, N)
%enddef
