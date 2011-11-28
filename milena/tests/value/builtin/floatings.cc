// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
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

#include <mln/value/ops.hh>
#include <mln/value/builtin/floatings.hh>

int main()
{
  // Checking sum value type name.
  mln_assertion(mln_trait_value_name(mln_sum_product_(float, float)) == std::string("float"));
  mln_assertion(mln_trait_value_name(mln_sum_product_(double, double)) == std::string("double"));

  // Make sure we return the minimum negative value but not the
  // minimum positive value.
  mln_assertion(mln_min(float) < 0.);
  mln_assertion(mln_min(double) < 0.);

  // Make sure the sum value type does not alter precision.
  {
    float a = mln_max(float) / 3,
      b = mln_max(float) / 3;
    volatile float c = a + b;
    volatile mln_sum_product_(float, float) d = a + b;
    mln_assertion(c == d);
  }
  {
    double a = mln_max(double) / 3,
      b = mln_max(double) / 3;
    volatile double c = a + b;
    volatile mln_sum_product_(double, double) d = a + b;
    mln_assertion(c == d);
  }
}
