// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef METALIC_ARRAY_OBJS_HH
# define METALIC_ARRAY_OBJS_HH

// FIXME: rename, objs is not precise enough.

//
//  Defines common objects useful for array declarations.
//

# include <mlc/objs.hh>

namespace mlc
{

  namespace internal
  {

    class lbrk_  {};
    class pbrk_  {};
    
    template<class T>
    struct x_ {
      x_(T val) : ue(val) {}
      T ue;
    };
    template<> struct x_<void> {};
    template<> struct x_<lbrk_>;
    template<> struct x_<end_type>;
    
  } // end of internal

  // cross means origin

  template<class T> inline
  internal::x_<T>
  x(T val) { return internal::x_<T>(val); }

  inline
  internal::x_<void>
  x() { return internal::x_<void>(); }

  static const internal::lbrk_  lbrk  = internal::lbrk_();
  static const internal::pbrk_  pbrk  = internal::pbrk_();

} // end of mlc

#endif // METALIC_ARRAY_OBJS_HH
