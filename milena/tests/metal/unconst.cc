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

/*! \file tests/metal/unconst.cc
 *
 * \brief Test on mln::metal::unconst.
 */

#include <typeinfo>
#include <string>

#include <mln/metal/unconst.hh>
#include <mln/core/exact.hh>



int main()
{
  using namespace mln;
  
  {
    mlc_unconst_(int) var; // int
    var = 0;
    // void* v = var;
  }
  {
    mlc_unconst_(const int) var; // int
    var = 0;
    // void* v = var;
  }

  {
    mlc_unconst_(int*) var; // int*
    var = 0;
    // void v = var;
  }
  {
    mlc_unconst_(const int*) var; // const int*
    var = 0;
    // void v = var;
  }
  {
    mlc_unconst_(int *const) var; // int*
    var = 0;
    // void v = var;
  }
  {
    mlc_unconst_(const int *const) var; // const int*
    var = 0;
    // void v = var;
  }


  {
    int tmp = 0;
    mlc_unconst_(int&) var = tmp; // int&
    tmp = 51;
    mln_assertion(var == 51);
    // void* v = var;
  }
  {
    int tmp = 0;
    mlc_unconst_(const int&) var = tmp; // int&
    tmp = 51;
    mln_assertion(var == 51);
    // void* v = var;
  }

}
