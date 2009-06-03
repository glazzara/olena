// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

/// \file tests/algebra/op_times.cc
///
/// Tests on mln::algebra::operator *.

#include <iostream>
#include <mln/algebra/mat.hh>



int main()
{
  using namespace mln;

  using algebra::vec;
  using algebra::mat;

  // Debase tests.
  {
    algebra::vec<3,  float> v3;
    algebra::mat<2,3,float> m23;
    algebra::mat<3,2,float> m32;

    algebra::mat<2,2,float> m22 = m23 * m32;
    algebra::vec<2,  float> v2  = m23 * v3;

    m22 = v2 * v2.t();
  }

  // Tests with horizontal and/or vertical matrices and/or vectors.
  {
    float f;
    algebra::vec<3,  float> v3;
    algebra::mat<1,3,float> m13;
    algebra::mat<3,1,float> m31;
    
    f = m13 * m31;
    f = m31.t() * m13.t();
    f = m13 * v3;
    f = v3.t() * v3;
    
    v3 = m31;
    m31 = v3;
  }

}
