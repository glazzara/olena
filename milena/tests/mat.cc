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

/*! \file tests/image2d.cc
 *
 * \brief Tests on mln::image2d.
 */


#include <iostream>
#include <mln/fun/i2v/all.hh>
#include <mln/metal/mat.hh>
#include <mln/core/h_mat.hh>



int main()
{
  using namespace mln;

  metal::mat<1,3,float> m1(all(4.));
  metal::mat<2,2,float> m2 = metal::mat<2,2,float>::Id;

  h_mat<1,float> hm1(m2);
  h_mat<2,float> hm2;
  h_mat<3,float> hm3(all(1.5));

  metal::mat<4,4,float> m4 = hm3;

  std::cout << "m1 = " << m1 << ";" << std::endl;
  std::cout << "m2 = " << m2 << ";" << std::endl;
  std::cout << "m4 = " << m4 << ";" << std::endl;
  std::cout << "hm1 = " << hm1 << ";" << std::endl;
  std::cout << "hm2 = " << hm2 << ";" << std::endl;
  std::cout << "hm3 = " << hm3 << ";" << std::endl;

  {
    h_mat<2,float> m, m2;
    m = m2;
    // FIXME: Test *many* => runs ok...
  }

}
