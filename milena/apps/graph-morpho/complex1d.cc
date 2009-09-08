// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

/// \file
/// \brief Morphological operators in graph space implemented on
/// 1-complex images.
///
/// \todo There should be a second version of this program, where the
/// graph structure is implemented with an actual graph.

#include <mln/core/image/complex_image.hh>
#include <mln/geom/complex_geometry.hh>
#include <mln/core/image/complex_neighborhoods.hh>
#include <mln/core/image/complex_neighborhood_piter.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/box2d.hh>

#include <mln/core/site_set/p_set.hh>
#include <mln/util/site_pair.hh>


using namespace mln;

// A graph is considered as a 1-complex here.
const unsigned dim = 1;
typedef topo::complex<dim> complex_t;

// Binary graph-based image with vertices aligned on a discrete 2D grid.
typedef point2d site_t;
typedef geom::complex_geometry<dim, site_t> geom_t;
typedef complex_image<dim, geom_t, bool> ima_t;

// Iterator type on faces (vertices and edges).
typedef p_n_faces_fwd_piter<dim, geom_t> face_iter;
// Edge-to-vertices neighborhood.
typedef complex_lower_neighborhood<dim, geom_t> e2v_t;
const e2v_t e2v;
// Vertex-to-edges neighborhood.
typedef complex_higher_neighborhood<dim, geom_t> v2e_t;
const v2e_t v2e;


// FIXME: We should turn this routine into something much more
// generic, and move it to the library (into make/).
ima_t
build_regular_complex1d_image(const box2d& support)
{
  unsigned nrows = support.pmax().row() - support.pmin().row() + 1;
  unsigned ncols = support.pmax().col() - support.pmin().col() + 1;

  typedef topo::n_face<0, dim> vertex_t;

  complex_t c;
  geom_t geom;

  // Vertices.
  for (unsigned row = 0; row < nrows; ++row)
    for (unsigned col = 0; col < ncols; ++col)
      {
	c.add_face();
	geom.add_location(point2d(row,col));
      }

  // Edges.
  for (unsigned row = 0; row < nrows; ++row)
  {
    // Horizontal edges.
    for (unsigned col = 1; col < ncols; ++col)
      {
	// First vertex.
	vertex_t v1(c, row * ncols + col - 1);
	// Second vertex.
	vertex_t v2(c, row * ncols + col);
	// Edge bewteen V1 and V2.
	c.add_face(v1 + v2);
      }

    // Vertical edges.
    if (row != 0)
      for (unsigned col = 0; col < ncols; ++col)
	{
	  // First vertex.
	  vertex_t v1(c, (row - 1) * ncols + col);
	  // Second vertex.
	  vertex_t v2(c, row * ncols + col);
	  // Edge bewteen V1 and V2.
	  c.add_face(v1 + v2);
	}
  }

  // Site set (domain) of the image.
  p_complex<dim, geom_t> pc(c, geom);

  // Image based on this site set.
  ima_t ima(pc);
  return ima;
}


template <typename T>
void
println(const complex_image<dim, geom_t, T>& ima, const box2d& support)
{
  // These are admittedly loose preconditions, but we cannot check
  // much.
  mln_precondition(ima.nsites() == support.nsites());

  image2d<bool> vertices(support);
  image2d<bool> h_edges(box2d(support.pmin(), support.pmax() - dpoint2d(0, 1)));
  image2d<bool> v_edges(box2d(support.pmin(), support.pmax() - dpoint2d(1, 0)));

  // Iterator on vertices.
  p_n_faces_fwd_piter<dim, geom_t> v(ima.domain(), 0);
  for_all(v)
  {
    mln_site_(geom_t) s(v);
    // Site S is point2d multi-site and should be a singleton (since V
    // iterates on vertices).
    mln_invariant(s.size() == 1);
    point2d p = s.front();
    vertices(p) = ima(v);
  }

  // Iterator on edges.
  p_n_faces_fwd_piter<dim, geom_t> e(ima.domain(), 1);
  for_all(e)
  {
    mln_site_(geom_t) s(e);
    // Site S is point2d multi-site and should be a pair (since E
    // iterates on vertices).
    mln_invariant(s.size() == 2);
    point2d p1 = s[0];
    point2d p2 = s[1];
    if (p1.row() == p2.row())
      {
	// Horizontal edge.
	h_edges(p1) = ima(e);
      }
    else
      {
	// Vertical edge.
	mln_assertion(p1.col() == p2.col());
	v_edges(p1) = ima(e);
      }
  }

  for (int row = vertices.domain().pmin().row();
       row <= vertices.domain().pmax().row(); ++row)
    {
      for (int col = vertices.domain().pmin().col();
	   col <= vertices.domain().pmax().col(); ++col)
	{
	  point2d p(row, col);
	  // Vertex.
 	  std::cout << (vertices(p) ? "O" : ".");
	  // Potential horizontal edge on the right of the vertex.
	  if (col != vertices.domain().pmax().col())
	    std::cout << (h_edges(p) ? " - " : "   ");
	}
      std::cout << std::endl;

      // Potential vertical edge below the vertices of the current ROW.
      if (row != vertices.domain().pmax().row())
	for (int col = vertices.domain().pmin().col();
	     col <= vertices.domain().pmax().col(); ++col)
	  {
	    point2d p(row, col);
	    std::cout << (v_edges(p) ? "|   " : "    ");
	  }
      std::cout << std::endl;
    }
}

/*------------------------------------.
| Morphological operators on graphs.  |
`------------------------------------*/

/* FIXME: By constraining the domain of the input and passing the
   neighborhood, one should be able to use a truly generic dilation
   (resp. erosion), or even use Milena's standard morpho::dilation
   (resp. morpho::erosion).  */

/// Dilation from edges to vertices (delta^dot).
template <typename I>
mln_concrete(I)
dilation_e2v(const Image<I>& input_)
{
  const I& input = exact(input_);
  mln_concrete(I) output;
  initialize(output, input);
  p_n_faces_fwd_piter<dim, geom_t> v(input.domain(), 0);
  mln_niter_(v2e_t) e(v2e, v);
  for_all(v)
  {
    output(v) = false;
    for_all(e)
      if (input(e))
	{
	  output(v) = true;
	  break;
	}
  }
  return output;
}

/// Erosion from vertices to edges (erosion^cross).
template <typename I>
mln_concrete(I)
erosion_v2e(const Image<I>& input_)
{
  const I& input = exact(input_);
  mln_concrete(I) output;
  initialize(output, input);
  p_n_faces_fwd_piter<dim, geom_t> e(input.domain(), 1);
  mln_niter_(e2v_t) v(e2v, e);
  for_all(e)
  {
    output(e) = true;
    for_all(v)
      if (!input(v))
	{
	  output(e) = false;
	  break;
	}
  }
  return output;
}

/// Erosion from edges to vertices (erosion^dot).
template <typename I>
mln_concrete(I)
erosion_e2v(const Image<I>& input_)
{
  const I& input = exact(input_);
  mln_concrete(I) output;
  initialize(output, input);
  p_n_faces_fwd_piter<dim, geom_t> v(input.domain(), 0);
  mln_niter_(v2e_t) e(v2e, v);
  for_all(v)
  {
    output(v) = true;
    for_all(e)
      if (!input(e))
	{
	  output(v) = false;
	  break;
	}
  }
  return output;
}

/// Dilation from vertices to edges (dilation^cross).
template <typename I>
mln_concrete(I)
dilation_v2e(const Image<I>& input_)
{
  const I& input = exact(input_);
  mln_concrete(I) output;
  initialize(output, input);
  p_n_faces_fwd_piter<dim, geom_t> e(input.domain(), 1);
  mln_niter_(e2v_t) v(e2v, e);
  for_all(e)
  {
    output(e) = false;
    for_all(v)
      if (input(v))
	{
	  output(e) = true;
	  break;
	}
  }
  return output;
}


int main()
{
  /*--------------.
  | Input image.  |
  `--------------*/

  /* Build a ``regular'' graph image.  Of course, it would have been
     better, simpler and faster to use a cubical 1-complex here, but
     they are not yet available.  */
  box2d b(9, 6);
  ima_t ima = build_regular_complex1d_image(b);

  /* Set the values so that IMA corresponds to the graph G of the ISMM
     2009 paper from Jean Cousty et al.  */

  // The set of vertices of the graph.
  p_set<point2d> vertices;
  vertices.insert(point2d(1, 2));
  vertices.insert(point2d(2, 1));
  vertices.insert(point2d(2, 2));
  vertices.insert(point2d(3, 4));
  vertices.insert(point2d(5, 2));
  vertices.insert(point2d(5, 3));
  vertices.insert(point2d(5, 4));
  vertices.insert(point2d(6, 1));
  vertices.insert(point2d(6, 2));
  vertices.insert(point2d(6, 3));
  vertices.insert(point2d(6, 4));
  vertices.insert(point2d(7, 2));
  vertices.insert(point2d(7, 3));
  // Add vertices.
  p_n_faces_fwd_piter<dim, geom_t> v(ima.domain(), 0);
  for_all(v)
  {
    mln_site_(geom_t) s(v);
    // Site S is point2d multi-site and should be a singleton (since V
    // iterates on vertices).
    mln_invariant(s.size() == 1);
    point2d p = s.front();
    ima(v) = vertices.has(p);
  }

  // The set of edges of the graph.
  typedef util::site_pair<point2d> point2d_pair;
  p_set< util::site_pair<point2d> > edges;
  edges.insert(point2d_pair(point2d(2, 1), point2d(2, 2)));
  edges.insert(point2d_pair(point2d(5, 3), point2d(5, 4)));
  edges.insert(point2d_pair(point2d(5, 2), point2d(6, 2)));
  edges.insert(point2d_pair(point2d(5, 3), point2d(6, 3)));
  edges.insert(point2d_pair(point2d(6, 1), point2d(6, 2)));
  edges.insert(point2d_pair(point2d(6, 2), point2d(6, 3)));
  edges.insert(point2d_pair(point2d(6, 3), point2d(6, 4)));
  edges.insert(point2d_pair(point2d(6, 2), point2d(7, 2)));
  edges.insert(point2d_pair(point2d(6, 3), point2d(7, 3)));
  edges.insert(point2d_pair(point2d(7, 2), point2d(7, 3)));
  // Add edges.
  p_n_faces_fwd_piter<dim, geom_t> e(ima.domain(), 1);
  for_all(e)
  {
    mln_site_(geom_t) s(e);
    // Site S is point2d multi-site and should be a pair (since E
    // iterates on vertices).
    mln_invariant(s.size() == 2);
    point2d p1 = s[0];
    point2d p2 = s[1];
    ima(e) =
      edges.has(point2d_pair(p1, p2)) || edges.has(point2d_pair(p2, p1));
  }
  std::cout << "ima:" << std::endl;
  println(ima, b);

  /*-----------------------------------.
  | Applying morphological operators.  |
  `-----------------------------------*/

  ima_t dil_e2v_ima = dilation_e2v(ima);
  std::cout << "dil_e2v_ima:" << std::endl;
  println(dil_e2v_ima, b);

  ima_t ero_v2e_ima = erosion_v2e(ima);
  std::cout << "ero_v2e_ima:" << std::endl;
  println(ero_v2e_ima, b);

  ima_t ero_e2v_ima = erosion_e2v(ima);
  std::cout << "ero_e2v_ima:" << std::endl;
  println(ero_e2v_ima, b);

  ima_t dil_v2e_ima = dilation_v2e(ima);
  std::cout << "dil_v2e_ima:" << std::endl;
  println(dil_v2e_ima, b);
}
