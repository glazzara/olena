//								-*- C++ -*-
// Copyright (C) 2008, 2009, 2013, 2014 EPITA Research and Development
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

/// \file morpho.i
/// \brief Wrappers of morphological algorithms from mln::morpho.

%module morpho

%include "concrete.ixx"
%include "ch_value.ixx"

/*-----------------------------------.
| Morphological dilation & erosion.  |
`-----------------------------------*/

%{
#include "mln/morpho/dilation.hh"
#include "mln/morpho/erosion.hh"
%}


/* Files mln/morpho/dilation.hh and mln/morpho/erosion.hh contains too
   much implementation details.  Using just
   
     %include "mln/morpho/dilation.hh"
     %include "mln/morpho/erosion.hh"

   would require a lot of efforts.  Use explicit prototypes
   instead.  */
namespace mln
{
  namespace morpho
  {
    template <typename I, typename W>
    mln_concrete(I)
    dilation(const Image<I>& input, const Window<W>& win);

    template <typename I, typename W>
    mln_concrete(I)
    erosion(const Image<I>& input, const Window<W>& win);
  }
}


/* FIXME: We should try to factor the multiple instantiations of
   `mln::trait::concrete< I >'.  */

%define instantiate_dilation(Name, I, W)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _concrete_trait) mln::trait::concrete< I >;
  %template(Name) mln::morpho::dilation< I, W >;
%enddef

%define instantiate_erosion(Name, I, W)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _concrete_trait) mln::trait::concrete< I >;
  %template(Name) mln::morpho::erosion< I, W >;
%enddef

/*-----------------------------------------------.
| Morphological (elementary) closing & opening.  |
`-----------------------------------------------*/

%{
#include "mln/morpho/elementary/closing.hh"
#include "mln/morpho/elementary/opening.hh"
%}

%include "mln/morpho/elementary/closing.hh"
%include "mln/morpho/elementary/opening.hh"

%define instantiate_closing(Name, I, N)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _concrete_trait) mln::trait::concrete< I >;
  %template(Name) mln::morpho::elementary::closing< I, N >;
%enddef

%define instantiate_opening(Name, I, N)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _concrete_trait) mln::trait::concrete< I >;
  %template(Name) mln::morpho::elementary::opening< I, N >;
%enddef

/*------------.
| Gradients.  |
`------------*/

%{
#include "mln/morpho/gradient.hh"
%}

%include "mln/morpho/gradient.hh"

%define instantiate_gradient(Name, I, W)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _concrete_trait) mln::trait::concrete< I >;
  %template(Name) mln::morpho::gradient< I, W >;
%enddef

%define instantiate_gradient_internal(Name, I, W)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _concrete_trait) mln::trait::concrete< I >;
  %template(Name) mln::morpho::gradient_internal< I, W >;
%enddef

%define instantiate_gradient_external(Name, I, W)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _concrete_trait) mln::trait::concrete< I >;
  %template(Name) mln::morpho::gradient_external< I, W >;
%enddef

/*-------------------------.
| Area closing & opening.  |
`-------------------------*/

%{
#include "mln/morpho/closing/area.hh"
#include "mln/morpho/opening/area.hh"
%}

%include "mln/morpho/closing/area.hh"
%include "mln/morpho/opening/area.hh"

%define instantiate_closing_area(Name, I, N)
  %template(Name) mln::morpho::closing::area< I, N >;
%enddef

%define instantiate_opening_area(Name, I, N)
  %template(Name) mln::morpho::opening::area< I, N >;
%enddef

/*----------------------------.
| Watershed Transform (WST).  |
`----------------------------*/

%{
#include "mln/morpho/watershed/flooding.hh"
%}

%include "mln/morpho/watershed/flooding.hh"

%define instantiate_watershed_flooding(Name, L, I, N)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _ch_value_trait) mln::trait::ch_value< I, L >;
  %template(Name) mln::morpho::watershed::flooding< L, I, N >;
%enddef

/*-----------.
| Max-tree.  |
`-----------*/

%{
#include "mln/morpho/tree/max.hh"
%}

%include "mln/morpho/tree/max.hh"

%define instantiate_max_tree(Name, I, N)
  // Explicit instantiation of this trait for the return type.
  %template(Name ## _ch_value_trait) mln::trait::ch_value< I, mln_psite(I) >;
  %template(Name) mln::morpho::tree::max< I, N >;
%enddef

/*-------------------------.
| Instantiate everything.  |
`-------------------------*/

%define instantiate_morpho(I, W, N)
  instantiate_dilation(dilation, I, W)
  instantiate_erosion(erosion, I, W)

  instantiate_closing(closing, I, N)
  instantiate_opening(opening, I, N)

  instantiate_gradient(gradient, I, W)
  instantiate_gradient_internal(gradient_internal, I, W)
  instantiate_gradient_external(gradient_external, I, W)

  instantiate_closing_area(closing_area, I, N)
  instantiate_opening_area(opening_area, I, N)

  instantiate_watershed_flooding(watershed_flooding, mln_value(I), I, N)

  instantiate_max_tree(max_tree, I, N)
%enddef
