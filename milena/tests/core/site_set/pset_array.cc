// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

/*! \file tests/core/site_set/pset_array.cc
 *
 * \brief Tests for the pset array.
 */

#include <mln/core/pset_array.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/p_runs.hh>

#include <iostream>

int main()
{
  using namespace mln;

  typedef p_runs_<point2d> runs;

  p_runs_<point2d> pruns0;
  p_runs_<point2d> pruns1;
  p_runs_<point2d> pruns2;

  pruns0.insert(p_run<point2d>(make::point2d(0, 0), 2));

  pruns2.insert(p_run<point2d>(make::point2d(10,10), 5));

  pruns1.insert(p_run<point2d>(make::point2d(2, 4), 7));
  pruns1.insert(p_run<point2d>(make::point2d(18, 42), 5));
  pruns1.insert(p_run<point2d>(make::point2d(50, 76), 2));
  pruns1.insert(p_run<point2d>(make::point2d(17,40), 6));



  /// Declare the pset_array
  pset_array<runs> array;

  /// Add elements in the array
  assert(array.npsets() == 0);
  array.insert(pruns0);
  assert(array.npsets() == 1);
  assert(array.nsites() == 2);

  array.insert(pruns2);
  assert(array.npsets() == 2);
  assert(array.nsites() == 7);

  array.insert(pruns1);

  /// Psite tests
  typedef pset_array<runs>::psite psite;
  runs_psite<point2d> run_psite1(pruns0, 1, 0);
  runs_psite<point2d> run_psite2(pruns1, 5, 1);

  psite ps(run_psite1, 0);
  assert(array.has(ps));

  psite ps2(run_psite2, 2);
  assert(array.has(ps2));

  // Iterator test:
  pset_array<runs>::fwd_piter piter(array);

  for (piter.start(); piter.is_valid(); piter.next())
  {
    std::cout << piter << std::endl;
  }
}
