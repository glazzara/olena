// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/util/graph.hh>

#include <mln/value/label_8.hh>

#include <mln/make/p_edges_with_mass_centers.hh>

#include <mln/util/site_pair.hh>

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

  typedef p_edges<G,fun::i2v::array<util::site_pair<point2d> > > pe_t;
  pe_t pe = make::p_edges_with_mass_centers(wst, g);

  typedef util::site_pair<point2d> P;
  typedef p_array<P> arr_t;
  arr_t arr;
  arr.insert(P(point2d(1,1), point2d(1,5)));
  arr.insert(P(point2d(1,1), point2d(5,1)));
  arr.insert(P(point2d(5,1), point2d(5,5)));
  arr.insert(P(point2d(1,5), point2d(5,5)));

  mln_piter_(pe_t) p1(pe);
  mln_piter_(arr_t) p2(arr);
  for_all_2(p1,p2)
    mln_assertion(p1 == p2);
}
