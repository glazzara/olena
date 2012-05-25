// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#include <mln/make/h_mat.hh>
#include <mln/math/abs.hh>


static const int result[4][4] = { { 2, 3,  4, 0 },
				  { 5, 6,  7, 0 },
				  { 8, 9, 10, 0 },
				  { 0, 0,  0, 1 } };

static const double result_q[4][4] = { { 0.776477, -0.113801, 0.619785, 0 },
				       { 0.345031, 0.8998, -0.267046, 0 },
				       { -0.527293, 0.4212, 0.737938, 0 },
				       { 0, 0, 0, 1 } };


template <typename T>
bool about_equal(const T& f, const T& q)
{
  return mln::math::abs(q - f) <= 0.000001;
}


int main()
{
  using namespace mln;

  int vals[9] = { 2, 3,  4,
		  5, 6,  7,
		  8, 9, 10 };
  algebra::h_mat<3,int> m = make::h_mat(vals);

  for (unsigned i = 0; i < 4; ++i)
    for (unsigned j = 0; j < 4; ++j)
      mln_assertion(m(i,j) == result[i][j]);


  // Checking creation from quaternions.
  {
    algebra::vec<3,double> v;
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    v.normalize();

    algebra::quat q(0.92388, 0.186238, 0.310397, 0.124159);
    q.set_unit();

    algebra::h_mat<3,double> mat = make::h_mat(double(), q);

    for (unsigned i = 0; i < 4; ++i)
      for (unsigned j = 0; j < 4; ++j)
	mln_assertion(about_equal(mat(i,j), result_q[i][j]));
  }
}
