// Copyright (C) 2008 EPITA Research and Development Laboratory
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

/*! \file tests/core/p_array.cc
 *
 * \brief Tests on mln::p_array.
 */

#include <mln/core/alias/point2d.hh>
#include <mln/core/site_set/p_array.hh>


int main()
{
  using namespace mln;

  typedef p_array<point2d> Arr;

  point2d x(5, 1);

  Arr arr;
  arr.append(x).append(x);
  mln_assertion(arr.nsites() == 2);

  mlc_equal( mln_psite_(Arr)::site, point2d )::check();

  mln_psite_(Arr) p(arr, 0);
  mln_assertion(p.to_site() == x);
  mln_assertion(p.row() == 5);
  mln_assertion(point2d(p) == x);
}
