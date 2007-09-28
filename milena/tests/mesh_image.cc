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

/*! \file tests/mesh_image.cc
 *
 * \brief Tests on mln::mesh_image.
 */

#include <mln/core/image2d_b.hh>
#include <mln/core/point2d.hh>
#include <mln/debug/println.hh>
#include <mln/util/graph.hh>
#include <mln/core/mesh_p.hh>
#include <mln/core/mesh_psite.hh>
#include <mln/draw/mesh.hh>
#include <mln/core/mesh_image.hh>

using namespace mln;

int
main (void)
{
 util::graph<void> g;

 g.add_node ();
 g.add_node ();
 g.add_node ();
 g.add_node ();
 g.add_node ();
 g.add_node ();

 g.add_edge (0, 1);
 g.add_edge (1, 2);
 g.add_edge (2, 3);
 g.add_edge (1, 3);
 g.add_edge (4, 5);
 g.add_edge (1, 4);

 g.consistency ();
 // g.print_debug ();

 std::vector<point2d> v;
 v.push_back (make::point2d (1,1));
 v.push_back (make::point2d (10,1));
 v.push_back (make::point2d (19,2));
 v.push_back (make::point2d (19,19));
 v.push_back (make::point2d (10,10));
 v.push_back (make::point2d (1,19));


 image2d_b<int> ima (20, 20, 1);

 mesh_p<point2d> m(g, v);

 draw::mesh (ima, m, 7, 1);

 std::vector<int> val;

 val.push_back (2);
 val.push_back (3);
 val.push_back (4);
 val.push_back (5);
 val.push_back (6);
 val.push_back (7);

 mesh_image<point2d, int> im (m, val);

 draw::mesh (ima, im);
 debug::println (ima);

 std::cout << im.domain() << std::endl;
}
