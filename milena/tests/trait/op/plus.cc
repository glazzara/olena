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

/*! \file tests/trait/op/plus.cc
 *
 * \brief Tests on mln::trait::op::plus.
 */

#include <mln/core/concept/image.hh>
#include <mln/value/concept/all.hh>
#include <mln/trait/op/plus.hh>
#include <mln/value/builtin/all.hh>


namespace mln
{


  template <typename T>
  struct my_image2d : Image< my_image2d<T> >
  {
    void m() {}
  };


  namespace trait
  {

    // int + float -> float

    template <>
    struct set_precise_binary_< op::plus, int, float >
    {
      typedef float ret;
    };


    // Image I + Image J -> bool (demo type!)

    template <typename I, typename J>
    struct set_binary_< op::plus, Image, I,  Image, J >
    {
      typedef bool ret;
    };

    // precise definition: my_image2d<T> + my_image2d<U> -> my_image2d<V>
    // ('&' is to avoid compiling an empty class)

    template <typename T, typename U>
    struct set_precise_binary_< op::plus, my_image2d<T>, my_image2d<U> >
    {
      typedef mln_trait_op_plus(T, U) V; 
      typedef my_image2d<V>& ret;
    };

  }
  
}

int main()
{
  using namespace mln;
  {
    mln_trait_op_plus_(int, float) tmp;
    tmp = 5.1f;
  }
  {
    my_image2d<float>* ptr;
    mln_trait_op_plus_(my_image2d<int>, my_image2d<float>) tmp = *ptr;
    tmp.m();
  }
}
