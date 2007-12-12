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

/*! \file tests/mesh_elt_window.cc
 *
 *  \brief Tests on mln::win::mesh_elt_window.
 */

#include <vector>

#include <mln/core/point2d.hh>
#include <mln/core/mesh_elt_window.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  typedef point2d p_t;

  /*--------.
  | Graph.  |
  `--------*/

  // Points associated to nodes.
  std::vector<p_t> points;
  points.push_back(make::point2d(0,0)); // Point associated to node 0.
  points.push_back(make::point2d(2,2)); // Point associated to node 1.
  points.push_back(make::point2d(0,4)); // Point associated to node 2.
  points.push_back(make::point2d(4,3)); // Point associated to node 3.
  points.push_back(make::point2d(4,4)); // Point associated to node 4.

  // Edges.
  mln::util::graph<void> g;
  // Populate the graph with nodes.
  for (unsigned i = 0; i < points.size(); ++i)
    g.add_node ();
  // Populate the graph with edges.
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 2);

  /*------------------.
  | Mesh and window.  |
  `------------------*/

  // Mesh psite set.
  mesh_p<p_t> mesh(g, points);
  // Mesh point site.
  mesh_psite<p_t> psite(mesh, 0);
  // ``Sliding'' window (in fact, neighborhood) of a psite of MESH.
  mesh_elt_window<p_t> win;
}
