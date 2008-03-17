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

/*! \file tests/h_vec.cc
 *
 * \brief Tests on mln::h_vec.
 */

#include <mln/core/h_vec.hh>
#include <mln/core/point3d.hh>

using namespace mln;


void run_in_3d(const algebra::vec<3, int>&)
{
}

void run_in_3d_h(const h_vec<3, int>&)
{
}


void foo(const mln::h_vec<3,float>&)
{
}


int main()
{

  algebra::vec<3, int> x;
  h_vec<3, int> w = x.to_h_vec();

  typedef h_vec<3, int> p3d;
  p3d p;
  run_in_3d(p.to_vec());

  point3d k;
  run_in_3d(k);
  run_in_3d_h(k.to_h_vec());

  {
    algebra::vec<3,float> v;
    h_vec<3,float> w(v.to_h_vec());
    w = v.to_h_vec();
    foo(v.to_h_vec());
  }
}
