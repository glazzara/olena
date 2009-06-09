// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <cstdlib>
#include <mln/accu/stat/var.hh>
#include <mln/fun/stat/mahalanobis.hh>


float my_rand(int c)
{
  return (1 + c) * float(std::rand()) / RAND_MAX;
}


int main()
{
  using namespace mln;

  typedef algebra::vec<3,float> vec3f;

  enum { n = 1000 };
  vec3f v[n];

  for (int i = 0; i < n; ++i)
    {
      v[i][0] = my_rand(0);
      v[i][1] = my_rand(1);
      v[i][2] = my_rand(2);
    }
  
  accu::stat::var<vec3f> a;
  for (int i = 0; i < n; ++i)
    a.take(v[i]);

  fun::stat::mahalanobis<vec3f> f(a.variance(), a.mean());
  mln_assertion(f(a.mean()) == 0.f);

  float sum = 0.f;
  for (int i = 0; i < n; ++i)
    {
      float f_ = f(v[i]);
      sum += f_ * f_;
    }
  mln_assertion(std::abs(sum / n - 3.f) < 0.00002f);
}
