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

/*! \file tests/core/alias/dpoint1d.cc
 *
 * \brief Tests on mln::dpoint1d.
 */

#include <mln/core/alias/dpoint1d.hh>



int main()
{
  using namespace mln;

  point1d p, q;
  dpoint1d dp;

   p =  point1d(1);
   q =  point1d(4);
  dp = dpoint1d(3);

  mln_assertion(dp == q - p);
  mln_assertion(q == p + dp);

  algebra::vec<1, float> v = dp;
  mln_assertion(v[0] == 3);
}
