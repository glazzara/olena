// Copyright (C) 2007, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#include <mln/fun/i2v/all_to.hh>
#include <mln/algebra/mat.hh>
#include <mln/algebra/h_mat.hh>
#include <mln/make/h_mat.hh>


template <typename T>
bool about_equal(const T& f, const T& q)
{
  return mln::math::abs(q - f) <= 0.000001;
}


int main()
{
  using namespace mln;

  // Reading in h_mat.
  algebra::h_mat<3,float> hm2(all_to(2.5));
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      mln_assertion(hm2(i, j) == 2.5f);

  // Conversion from mat to h_mat
  algebra::mat<2,2,float> m2 = literal::identity;
  algebra::h_mat<1,float> hm1(m2);
  mln_assertion(m2.size() == hm1.size());
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      mln_assertion(m2(i, j) == hm1(i, j));

  // Conversion from h_mat to mat.
  algebra::h_mat<3,float> hm3(all_to(1.5));
  algebra::mat<4,4,float> m4 = hm3;
  mln_assertion(m4.size() == hm3.size());
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      mln_assertion(m4(i, j) == hm3(i, j));


  // Check from_to_ overloads.
  {
    algebra::quat q_ref(0.92388, 0.186238, 0.310397, 0.124159);
    double vals[9] = { 0.776477, -0.113801, 0.619785,
		       0.345031, 0.8998, -0.267046,
		       -0.527293, 0.4212, 0.737938 };
    algebra::h_mat<3,double> m = make::h_mat(vals);
    algebra::quat q2;
    algebra::from_to_(m, q2);

    for (int i = 0; i < 4; ++i)
      mln_assertion(about_equal(q_ref.to_vec()[i], q2.to_vec()[i]));
  }

}
