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

/// \file concrete.i
/// \brief A wrapper of mln::trait::concrete.
///
/// This is a very limited workaround for the difficult wrapping of
/// mln::trait::concrete.

%module concrete

%{
#include "mln/trait/concrete.hh"
%}

# define mln_concrete(I)  typename mln::trait::concrete< I >::ret

namespace mln
{
  namespace trait
  {
    /* Swig is not powerful enough to parse difficult templates.  For
       instance, it won't match this specialization.

       mln::trait::concrete depends on mln::trait::ch_value.ixx that
       swig cannot wrap yet (see ch_value.ixx).  So we just give it
       simple ``inlined'' equivalent traits that are compatible with
       the ones in mln/trait/ch_value.hh.  */

    // By default, assume the concrete type is the paramater itself.
    template <typename I>
    struct concrete
    {
      typedef I ret;
    };

  } // end of namespace mln::morpho

} // end of namespace mln
