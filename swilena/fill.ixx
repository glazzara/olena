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

/// \file fill.ixx
/// \brief A wrapper of mln::data::fill.

%module fill

%{
#include "mln/data/fill.hh"
%}

%include "mln/core/macros.hh"

/* Wrap mln::data::fill by hand to help swig choose the right overload
   of the algorithm.  We might just try to use %ignore statements
   instead in a future version.  */
namespace mln
{
  namespace data
  {
    template <typename I>
    void fill(mln::Image<I>& ima, const mln_value(I)& v);
  }
}
