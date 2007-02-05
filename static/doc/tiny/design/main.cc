// Copyright (C) 2007 EPITA Research and Development Laboratory.
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


/* \file doc/tiny/design/main.cc

   \brief Tiny sample use in a pseudo-language.  */

namespace abc
{


  Iterator : type =
  {
    value : type
    next : () -> void
    is_valid : const () -> bool
    set : (v : value const&) -> void
  }


  array_iterator : [T: type] -> class =
  {
    category : type = Iterator;

    value : type = T
    next : () -> void                 = { i_ := i_ + 1 }
    is_valid : const () -> bool       = { return i_ >= 0 and i_ < n_ }
    set : (v : value const&) -> void  = { v_ := v }

    make : (n : int)                  = { i_ := 0; n_ := n }
    protected {
      i_, n_ : int
      v_     : value
    }
  }


  algo : [I : type < Iterator] (iter : I&, val : I.value&) -> void =
  {
    if (iter.is_valid())
      {
	iter.set(val)
	iter.next()
      }
  }


} // abc


main : () -> void =
{
  i   : abc::array_iterator[int] := make(7)
  val : int := 51 

  abc::algo(i, val)
}
