// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/algebra/mat2.cc
 *
 * \brief Test on mln::algebra::mat.
 */

#include <mln/algebra/mat.hh>
#include <mln/value/int_u8.hh>


int main()
{
  using namespace mln;

  // FIXME: A test should not print!

  //   const int
  //     tab1[18] = {3, 6, 5, 2, 4, 8,
  // 		5, 7, 4, 6, 9, 2,
  // 		2, 7, 1, 1, 5, 3},
  //     tab2[6]  = {2, 5, 1, 0, 7, 2},
  //     tab3[6]  = {3, 1, 6, 2, 1, 0};

  //   algebra::mat<3,6,int> mat36   = make::mat<3,6,18>(tab1);
  //   algebra::mat<2,3,int> mat23_1 = make::mat<2,3,6>(tab2);
  //   algebra::mat<2,3,int> mat23_2 = make::mat<2,3,6>(tab3);

  //   algebra::mat<2,3,float> mat23_3 = mat23_1 - mat23_2;

  //   std::cout << mat23_3 << std::endl << mat23_3 * mat36 << std::endl;

  using algebra::vec;
  vec<2,int> v = make::vec(5,1);

  using algebra::mat;
  mat<2,2, vec<2,int> > mv;
  mv.set_all(v);
  // std::cout << mv << std::endl;

  {
    mat<2,2,float> tmp = mv * mv;
    // std::cout << tmp << std::endl;
    tmp(0,0) = 0;
  }
  {
    vec<2, vec<2,float> > tmp = mv * v;
    // std::cout << (mv * v) << std::endl;
    tmp[0] = v;
  }
  {
    mat<2,2,float> tmp = value::scalar(v) * mv;
    // std::cout << (value::scalar(v) * mv) << std::endl;
    tmp(0,0) = 0;
  }
  {
    vec<2, mat<2,2, vec<2,float> > > tmp = v * value::scalar(mv);
    // std::cout << (v * value::scalar(mv)) << std::endl;
    tmp[0](0,0) = v;
  }
  {
    value::int_u8 i = 0;
    mat<2,2, vec<2,int> > tmp = mv * i;
    // std::cout << mv * i << std::endl;
    tmp(0,0) = v;
  }

}
