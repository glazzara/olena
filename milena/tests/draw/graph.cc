// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

/*! \file tests/draw/graph.cc
 *
 *  \brief Tests on mln::draw::graph.
 *
 *  Build a graph, convert it to an image, and compare it with a
 *  reference images.
 */

#include <vector>
#include <utility>

#include <mln/core/image2d.hh>
#include <mln/core/point2d.hh>
#include <mln/debug/println.hh>
#include <mln/util/graph.hh>
#include <mln/core/p_graph.hh>
#include <mln/core/graph_psite.hh>
#include <mln/draw/graph.hh>
#include <mln/core/graph_image.hh>
#include <mln/level/compare.hh>


/// Set of 2-D points.
typedef std::vector< mln::point2d > points_type;
/// Set of edges expressed using the identifiers of their adjecent nodes.
typedef std::vector< std::pair<int,int> > edges_type;

using namespace mln;

// FIXME: We might want to extract NROWS and NCOLS from REF instead of
// getting them from the caller.
void
test (points_type& points, const edges_type& edges,
      unsigned nrows, unsigned ncols, const mln::image2d<int>& ref)
{
  // Graph.
  util::graph<mln::point2d> g;
  // Populate the graph with nodes.
  for (unsigned i = 0; i < points.size(); ++i)
    g.add_node (points[i]);
  // Populate the graph with edges.
  for (edges_type::const_iterator i = edges.begin(); i != edges.end(); ++i)
    g.add_edge (i->first, i->second);

  mln::p_graph<point2d> pg(g);

  image2d<int> ima(nrows, ncols);
  draw::graph (ima, pg, 2, 1);
  mln_assertion (ima == ref);
}

int
main ()
{
  /*---------.
  | Test 1.  |
  `---------*/

  {
    // Reference image.
    int vs[3][3] = {
      {2, 0, 0},
      {0, 1, 0},
      {0, 0, 2}
    };
    image2d<int> ref (make::image2d(vs));

    // Points associated to nodes.
    points_type points;
    points.push_back (make::point2d (0,0)); // Point associated to node 0.
    points.push_back (make::point2d (2,2)); // Point associated to node 1.

    // Edges.
    edges_type edges;
    edges.push_back (std::make_pair (0, 1));

    test (points, edges, 3, 3, ref);
  }


  /*---------.
  | Test 2.  |
  `---------*/

  {
    int vs[5][5] = {
      {2, 0, 0, 0, 2},
      {0, 1, 0, 1, 1},
      {0, 0, 2, 0, 1},
      {0, 0, 0, 1, 1},
      {0, 0, 0, 2, 2},
    };
    image2d<int> ref (make::image2d(vs));

    // Points associated to nodes.
    points_type points;
    points.push_back (make::point2d (0,0)); // Point associated to node 0.
    points.push_back (make::point2d (2,2)); // Point associated to node 1.
    points.push_back (make::point2d (0,4)); // Point associated to node 2.
    points.push_back (make::point2d (4,3)); // Point associated to node 3.
    points.push_back (make::point2d (4,4)); // Point associated to node 4.

    // Edges.
    edges_type edges;
    edges.push_back (std::make_pair (0, 1));
    edges.push_back (std::make_pair (1, 2));
    edges.push_back (std::make_pair (1, 3));
    edges.push_back (std::make_pair (3, 4));
    edges.push_back (std::make_pair (4, 2));

    test (points, edges, 5, 5, ref);
  }
}
