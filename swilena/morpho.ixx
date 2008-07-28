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

/// \file fill.i
/// \brief Wrappers of morphological algorithms from mln::morpho.

%module morpho

%{
#include "mln/morpho/dilation.hh"
#include "mln/morpho/erosion.hh"
%}

// FIXME: Wrap mln::morpho::erosion by hand, for mln_concrete(I)
// disturbs swig.  Annotate the original source code instead?
namespace mln
{
  namespace morpho
  {

    /* FIXME: How can we handle concrete in Swilena?  Simplify this
     for the moment, and use I directly as return type.  */

    template <typename I, typename W>
    I
    dilation(const Image<I>& input, const Window<W>& win);

    template <typename I, typename W>
    I
    erosion(const Image<I>& input, const Window<W>& win);

  } // end of namespace mln::morpho

} // end of namespace mln
