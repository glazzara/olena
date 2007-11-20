// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/draw/mesh.cc
 *
 * \brief Tests on mln::draw::mesh.
 */

#include <mln/core/image2d.hh>
#include <mln/core/point2d.hh>
#include <mln/debug/println.hh>
#include <mln/util/graph.hh>
#include <mln/core/mesh_p.hh>
#include <mln/core/mesh_psite.hh>
#include <mln/draw/mesh.hh>
#include <mln/core/mesh_image.hh>
#include <mln/level/compare.hh>

int
main (void)
{
  using namespace mln;

  int vs[3][3] = {
    {2, 0, 0},
    {0, 1, 0},
    {0, 0, 2}
  };

  image2d<int> ref (make::image2d(vs));

  util::graph<void> g;

  g.add_node ();
  g.add_node ();
  g.add_edge (0, 1);
  g.consistency ();

  std::vector<point2d> v;
  v.push_back (make::point2d (0,0));
  v.push_back (make::point2d (2,2));

  mesh_p<point2d> m(g, v);

  image2d<int> ima (3, 3);
  draw::mesh (ima, m, 2, 1);

  mln_assertion (ima == ref);
}
