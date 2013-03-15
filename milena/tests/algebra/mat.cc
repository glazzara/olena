// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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



int main()
{
  using namespace mln;

  {
    using namespace algebra;
    mat<3,3,int>
      m   = algebra::internal::make(1, 2, 3,
				    0, 1, 4,
				    5, 6, 0),
      m_1 = algebra::internal::make(-24, +18, +05,
				    +20, -15, -04,
				    -05, +04, +01);
    mln_assertion(m._1() == m_1);
    mln_assertion(m * m._1() == literal::identity);
  }

  {
    using namespace algebra;
    mat<2,2,int> m = algebra::internal::make(1, 0,
					     0, 1);
    mln_assertion(tr(m) == 2);
    mln_assertion(det(m) == 1);
    mln_assertion(m._1() == m);
  }

}
