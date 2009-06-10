//						       		-*- C++ -*-
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
