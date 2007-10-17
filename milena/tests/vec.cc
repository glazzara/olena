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

/*! \file tests/vec.cc
 *
 * \brief Tests on mln::metal::vec.
 */


#include <iostream>
#include <mln/fun/i2v/all_to.hh>
#include <mln/metal/vec.hh>
#include <mln/core/h_vec.hh>



int main()
{
  using namespace mln;

  metal::vec<1,float> v1(all_to(4.));
  metal::vec<2,float> v2 = make::vec(6., 2.8);

  h_vec<1,float> hv1;
  h_vec<2,float> hv2 = v2.to_h_vec(); // Immersion into homogeneous.

  h_vec<3,float> hv3(all_to(1.5));

  hv3 += make::vec(0., 0., 0., 0.5);

  metal::vec<3,float> v3 = hv3.to_vec(); // Back from homogeneous.
  metal::vec<4,float> v4 = hv3;

  std::cout << "v1 = " << v1 << ";" << std::endl;
  std::cout << "v2 = " << v2 << ";" << std::endl;
  std::cout << "v3 = " << v3 << ";" << std::endl;
  std::cout << "v4 = " << v4 << ";" << std::endl;
  std::cout << "hv1 = " << hv1 << ";" << std::endl;
  std::cout << "hv2 = " << hv2 << ";" << std::endl;
  std::cout << "hv3 = " << hv3 << ";" << std::endl;
}
