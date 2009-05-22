// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/make/p_vertices_with_mass_centers.cc
///
/// Tests on mln::make::p_vertices_with_mass_centers.


#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/util/graph.hh>

#include <mln/value/label_8.hh>

#include <mln/make/p_vertices_with_mass_centers.hh>

int main()
{
  using namespace mln;

  typedef value::label_8 L;

  L data[] = {
    1, 1, 1, 0, 2, 2, 2,
    1, 1, 1, 0, 2, 2, 2,
    1, 1, 1, 0, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0,
    3, 3, 3, 0, 4, 4, 4,
    3, 3, 3, 0, 4, 4, 4,
    3, 3, 3, 0, 4, 4, 4
  };

  image2d<L> wst = make::image2d(data);
  typedef util::graph G;
  G g;
  g.add_vertices(5);
  g.add_edge(1,2);
  g.add_edge(1,3);
  g.add_edge(4,3);
  g.add_edge(4,2);

  L nbasins = 4;
  typedef p_vertices<G,fun::i2v::array<point2d> > pv_t;
  pv_t pv = make::p_vertices_with_mass_centers(wst, g);

  typedef p_array<point2d> arr_t;
  arr_t arr;
  arr.insert(point2d(3,3));
  arr.insert(point2d(1,1));
  arr.insert(point2d(1,5));
  arr.insert(point2d(5,1));
  arr.insert(point2d(5,5));

  mln_piter_(pv_t) p1(pv);
  mln_piter_(arr_t) p2(arr);
  for_all_2(p1,p2)
    mln_assertion(p1 == p2);
}

