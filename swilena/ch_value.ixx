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

/// \file ch_value.i
/// \brief A wrapper of mln::trait::ch_value.
///
/// This is a very limited workaround for the difficult wrapping of
/// mln::trait::ch_value (or even of a subset of it).

%module ch_value

%{
#include "mln/trait/ch_value.hh"
%}

# define mln_ch_value(I, V)  typename mln::trait::ch_value< I, V >::ret
# define mln_ch_value_(I, V)          mln::trait::ch_value< I, V >::ret

namespace mln
{
  namespace trait
  {

    template <typename I, typename V>
    struct ch_value
    {
      // Nothing by default.
    };

    /* Swig is not powerful enough to parse difficult templates (yet).
       For instance, it won't match this specialization:

         template <typename T, typename U>
         struct ch_value< mln::image2d< T >, U >
         {
           typedef mln::image2d< U > ret;
         };

       which is even simpler than what mln/trait/ch_value.hh contains.

       Hence we just give it simple ``inlined'' equivalent traits that
       are compatible with the ones in mln/trait/ch_value.hh.  */

    template <typename V>
    struct ch_value< mln::image2d< mln::value::int_u<8> >, V >
    {
      typedef mln::image2d< V > ret;
    };

  } // end of namespace mln::morpho

} // end of namespace mln
