// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/algebra/h_vec.hh>
#include <mln/core/alias/point3d.hh>

using namespace mln;


void run_in_3d(const algebra::vec<3, float>&)
{
}

void run_in_3d_h(const algebra::h_vec<3, float>&)
{
}


void foo(const mln::algebra::h_vec<3,float>&)
{
}


int main()
{

  algebra::vec<3, def::coord> x;
  algebra::h_vec<3, def::coord> w = x.to_h_vec();
  (void) w;

  typedef algebra::h_vec<3, def::coord> p3d;
  p3d p;
  run_in_3d(p.to_vec());

  point3d k;
  run_in_3d(k);
  run_in_3d_h(k.to_h_vec());

  {
    algebra::vec<3,float> v;
    algebra::h_vec<3,float> w(v.to_h_vec());
    w = v.to_h_vec();
    foo(v.to_h_vec());
  }
}
