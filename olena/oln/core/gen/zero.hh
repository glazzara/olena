// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_CORE_GEN_ZERO_HH
# define OLN_CORE_GEN_ZERO_HH

# include <oln/core/concept/literal.hh>


namespace oln
{

  struct zero_t;


  void assignment_(int&      i, const zero_t&) { i = 0;   }
  void assignment_(unsigned& u, const zero_t&) { u = 0u;  }
  void assignment_(float&    f, const zero_t&) { f = 0.f; }
  void assignment_(double&   d, const zero_t&) { d = 0.;  }


  struct zero_t : public Literal< zero_t >
  {
  }

  zero;



  // Default unary minus operator.

  template <typename T>
  T
  operator - (const Any<T>& rhs); // Use binary -.



# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  T
  operator - (const Any<T>& rhs)
  {
    return zero - exact(rhs);
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_ZERO_HH
