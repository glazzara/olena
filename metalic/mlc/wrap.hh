// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006  EPITA Research and Development Laboratory
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

#ifndef METALIC_WRAP_HH
# define METALIC_WRAP_HH


namespace mlc
{

  /*! \class mlc::wrap_<T>
  **
  ** This class is a workaround to the problem of implicit typename
  ** that appears when writting something like:
  **   outer_struct::inner_struct::value
  ** 
  ** Instead of solving manually this problem with:
  **   typedef typename outer_struct::inner_struct temp;
  **   temp::value
  ** you can write directly:
  **   wrap_< typename outer_struct::inner_struct >::value
  **
  ** This class is used by the mlc_is_a macro.
  **
  ** Design note: a detailed example can be read at the end of
  ** the file mlc/wrap.hh
  **
  **
  ** \see mlc::value_wrap_<T>, mlc::bexpr_wrap_<T>
  */

  template <class T>
  struct wrap_ : public T
  {
  };



} // end of namespace mlc


/*

// FIXME: the code below is weird!  I didn't succeed in reproducing
// the limitation of the "implicit typename" error; see the comment in
// code...

#include <iostream>
#include <mlc/bool.hh>

template <class T> struct wrap_ : public T {};

namespace my 
{
  template <class T, T val>
  struct value_
  {
    static const T value = val;
  };

  template <bool b>
  struct value_ <bool, b>
  {
    static const bool value = b;
  };

  template <bool b>
  struct bool_;

  template <>
  struct bool_<true> : public value_<bool, true>
  {
  };

  typedef bool_<true> true_;
}

template <class T>
struct outer
{
  struct inner_struct
    : public my::true_ {
    // replacing "my" by "mlc" gives an error (see KO below) and I
    // don't understand why!
  };
};

template <class T>
void foo_()
{
  std::cout << outer<T>::inner_struct::value << std::endl; // KO
}

int main()
{
  foo_<int>();
}

*/


#endif // ! METALIC_WRAP_HH
