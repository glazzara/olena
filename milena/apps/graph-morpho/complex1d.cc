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

#include <mln/core/routine/duplicate.hh>

#include <mln/math/abs.hh>

#include <mln/io/pbm/load.hh>

#include "apps/data.hh"

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

template <typename I>
ima_t
make_regular_complex1d_image(const Image<I>& input_)
{
  const I& input = exact(input_);
  const box2d& input_box = input.domain();
  // The input image must have an odd number of rows and columns.
  mln_precondition(input_box.nrows() % 2 == 1);
  mln_precondition(input_box.ncols() % 2 == 1);

  // The domain of the graph image is twice as small, since we
  // consider only vertices (edges are set ``between'' vertices).
  box2d output_box(input_box.nrows() / 2 + 1,
		   input_box.ncols() / 2 + 1);
  ima_t output = build_regular_complex1d_image(output_box);

  // Add values on vertices.
  p_n_faces_fwd_piter<dim, geom_t> v(output.domain(), 0);
  for_all(v)
  {
    mln_site_(geom_t) s(v);
    // Site S is point2d multi-site and should be a singleton (since V
    // iterates on vertices).
    mln_invariant(s.size() == 1);
    point2d p = s.front();
    point2d q(p.row() * 2, p.col() * 2);
    output(v) = input(q);
  }

  // Add values on edges.
  p_n_faces_fwd_piter<dim, geom_t> e(output.domain(), 1);
  for_all(e)
  {
    mln_site_(geom_t) s(e);
    // Site S is point2d multi-site and should be a pair (since E
    // iterates on vertices).
    mln_invariant(s.size() == 2);
    point2d p1 = s[0];
    point2d p2 = s[1];
    mln_invariant(math::abs(p1.row() - p2.row()) == 1
		  || math::abs(p1.col() - p2.col()) == 1);
    point2d q (p1.row() + p2.row(), p1.col() + p2.col());
    output(e) = input(q);
  }

  return output;
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

// ------------------------ //
// Dilations and erosions.  //
// ------------------------ //

/* FIXME: By constraining the domain of the input and passing the
   neighborhood, one should be able to use a truly generic dilation
   (resp. erosion), or even use Milena's standard morpho::dilation
   (resp. morpho::erosion).  */

/// Dilation from edges to vertices (\f$\delta^\bullet\f$).
template <typename I>
mln_concrete(I)
dilation_e2v(const Image<I>& input_)
{
  const I& input = exact(input_);
  mln_concrete(I) output;
  initialize(output, input);
  /* FIXME: It'd be better to write something like this:

       mln_piter(...) v(output | vertices);

     We can actually write this, but `vertices' has to be a predicate
     on sites (p2b function), which is not efficient, since both
     vertices and edges will be browsed.

     It would be very nice if `vertices' could be an actual site set,
     so that `output | vertices' creates a morpher smart enough to
     browse /only/ vertices.  */
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

/// Erosion from vertices to edges (\f$\epsilon^\times\f$).
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

/// Erosion from edges to vertices (\f$\epsilon^\bullet\f$).
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

/// Dilation from vertices to edges (\f$\delta^\times\f$).
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


/// Vertex dilation (\f$delta\f$).
template <typename I>
mln_concrete(I)
dilation_vertex(const Image<I>& input)
{
  return dilation_e2v(dilation_v2e(input));
}

/// Vertex erosion (\f$epsilon\f$).
template <typename I>
mln_concrete(I)
erosion_vertex(const Image<I>& input)
{
  return erosion_e2v(erosion_v2e(input));
}


/// Edge dilation (\f$Delta\f$).
template <typename I>
mln_concrete(I)
dilation_edge(const Image<I>& input)
{
  return dilation_v2e(dilation_e2v(input));
}

/// Edge erosion (\f$Epsilon\f$).
template <typename I>
mln_concrete(I)
erosion_edge(const Image<I>& input)
{
  return erosion_v2e(erosion_e2v(input));
}


/// Combine the vertices and the edges of two images to create a new
/// graph image.
template <typename I>
mln_concrete(I)
combine(const Image<I>& vertices_, const Image<I>& edges_)
{
  const I vertices = exact(vertices_);
  const I edges = exact(edges_);
  mln_precondition(vertices.domain() == edges.domain());

  mln_concrete(I) output;
  initialize(output, vertices);
  p_n_faces_fwd_piter<dim, geom_t> v(output.domain(), 0);
  for_all(v)
    output(v) = vertices(v);
  p_n_faces_fwd_piter<dim, geom_t> e(output.domain(), 1);
  for_all(e)
    output(e) = edges(e);
  return output;
}


/// Graph dilation (\f$delta \ovee Delta\f$).
template <typename I>
mln_concrete(I)
dilation_graph(const Image<I>& input)
{
  return combine(dilation_vertex(input), dilation_edge(input));
}

/// Graph erosion (\f$epsilon \ovee Epsilon\f$).
template <typename I>
mln_concrete(I)
erosion_graph(const Image<I>& input)
{
  return combine(erosion_vertex(input), erosion_edge(input));
}


// ------------------------ //
// Additional adjunctions.  //
// ------------------------ //

template <typename I>
mln_concrete(I)
alpha1(const Image<I>& input)
{
  mln_concrete(I) vertices;
  initialize(vertices, input);
  data::fill(vertices, true);
  return combine(vertices, input);
}

template <typename I>
mln_concrete(I)
beta1(const Image<I>& input)
{
  return combine(dilation_e2v(input), input);
}

template <typename I>
mln_concrete(I)
alpha2(const Image<I>& input)
{
  return combine(input, erosion_v2e(input));
}

template <typename I>
mln_concrete(I)
beta2(const Image<I>& input)
{
  mln_concrete(I) edges;
  initialize(edges, input);
  data::fill(edges, false);
  return combine(input, edges);
}

template <typename I>
mln_concrete(I)
alpha3(const Image<I>& input)
{
  return combine(erosion_e2v(input), erosion_v2e(erosion_e2v(input)));
}

template <typename I>
mln_concrete(I)
beta3(const Image<I>& input)
{
  return combine(dilation_e2v(dilation_v2e(input)), dilation_v2e(input));
}


// ----------------------- //
// Openings and closings.  //
// ----------------------- //

/// Vertex opening (\f$\gamma_1\f$).
template <typename I>
mln_concrete(I)
opening_vertex(const Image<I>& input)
{
  return dilation_vertex(erosion_vertex(input));
}

/// Vertex closing (\f$\phi_1\f$).
template <typename I>
mln_concrete(I)
closing_vertex(const Image<I>& input)
{
  return erosion_vertex(dilation_vertex(input));
}


/// Edge opening (\f$\Gamma_1\f$).
template <typename I>
mln_concrete(I)
opening_edge(const Image<I>& input)
{
  return dilation_edge(erosion_edge(input));
}

/// Edge closing (\f$\Phi_1\f$).
template <typename I>
mln_concrete(I)
closing_edge(const Image<I>& input)
{
  return erosion_edge(dilation_edge(input));
}


/// Graph opening (\f${\gamma \ovee \Gamma}_1\f$).
template <typename I>
mln_concrete(I)
opening_graph(const Image<I>& input)
{
  return combine(opening_vertex(input), opening_edge(input));
}

/// Graph closing (\f${\phi \ovee \Phi}_1\f$).
template <typename I>
mln_concrete(I)
closing_graph(const Image<I>& input)
{
  return combine(closing_vertex(input), closing_edge(input));
}


// --------------------------------- //
// Half-openings and half-closings.  //
// --------------------------------- //

/// Vertex half-opening (\f$\gamma_{1/2}\f$).
template <typename I>
mln_concrete(I)
half_opening_vertex(const Image<I>& input)
{
  return dilation_e2v(erosion_v2e(input));
}

/// Vertex half-closing (\f$\phi_{1/2}\f$).
template <typename I>
mln_concrete(I)
half_closing_vertex(const Image<I>& input)
{
  return erosion_e2v(dilation_v2e(input));
}


/// Edge half-opening (\f$\Gamma_{1/2}\f$).
template <typename I>
mln_concrete(I)
half_opening_edge(const Image<I>& input)
{
  return dilation_v2e(erosion_e2v(input));
}

/// Edge half-closing (\f$\Phi_{1/2}\f$).
template <typename I>
mln_concrete(I)
half_closing_edge(const Image<I>& input)
{
  return erosion_v2e(dilation_e2v(input));
}


/// Graph half-opening (\f${\gamma \ovee \Gamma}_{1/2}\f$).
template <typename I>
mln_concrete(I)
half_opening_graph(const Image<I>& input)
{
  return combine(half_opening_vertex(input), half_opening_edge(input));
}

/// Graph half-closing (\f${\phi \ovee \Phi}_{1/2}\f$).
template <typename I>
mln_concrete(I)
half_closing_graph(const Image<I>& input)
{
  return combine(half_closing_vertex(input), half_closing_edge(input));
}


// ------------------------------------------------------ //
// Parameterized openings and closings (granulometries).  //
// ------------------------------------------------------ //

/// Opening (\f${\gamma \ovee \Gamma}_{\lambda/2}\f$).
template <typename I>
mln_concrete(I)
opening(const Image<I>& input, unsigned lambda)
{
  unsigned i = lambda / 2;
  unsigned j = lambda % 2;
  mln_concrete(I) output = duplicate(input);
  for (unsigned m = 0; m < i; ++m)
    output = erosion_graph(output);
  for (unsigned m = 0; m < j; ++m)
    output = half_opening_graph(output);
  for (unsigned m = 0; m < i; ++m)
    output = dilation_graph(output);
  return output;
}

/// Opening (\f${\phi \ovee \Phi}_{\lambda/2}\f$).
template <typename I>
mln_concrete(I)
closing(const Image<I>& input, unsigned lambda)
{
  unsigned i = lambda / 2;
  unsigned j = lambda % 2;
  mln_concrete(I) output = duplicate(input);
  for (unsigned m = 0; m < i; ++m)
    output = dilation_graph(output);
  for (unsigned m = 0; m < j; ++m)
    output = half_closing_graph(output);
  for (unsigned m = 0; m < i; ++m)
    output = erosion_graph(output);
  return output;
}

// ----------------------------- //
// Alternate Sequential Filter.  //
// ----------------------------- //

/// Alternate Sequential Filter (ASF) (\f${ASF}_{\lambda/2}\f$).
template <typename I>
mln_concrete(I)
asf(const Image<I>& input, unsigned lambda)
{
  mln_concrete(I) output = duplicate(input);
  for (unsigned m = 0; m < lambda; ++m)
    output = half_opening_graph(half_closing_graph(output));
  return output;
}



/*-----------------------------------.
| Applying morphological operators.  |
`-----------------------------------*/

int main()
{
  /* Build a ``regular'' graph image.  Of course, it would have been
     better, simpler and faster to use a cubical 1-complex here, but
     they are not yet available (as of 2009-09-10).  */

  // ------------------------ //
  // Dilations and erosions.  //
  // ------------------------ //

  /* Set the values so that X corresponds to the graph X of the ISMM
     2009 paper from Jean Cousty et al.  */
  image2d<bool> x_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/x.pbm");
  ima_t x = make_regular_complex1d_image(x_pbm);
  box2d x_box(x_pbm.nrows() / 2 + 1, x_pbm.ncols() / 2 + 1);
  std::cout << "x:" << std::endl;
  println(x, x_box);
  
  ima_t dil_e2v_ima = dilation_e2v(x);
  std::cout << "dil_e2v_ima:" << std::endl;
  println(dil_e2v_ima, x_box);

  ima_t ero_v2e_ima = erosion_v2e(x);
  std::cout << "ero_v2e_ima:" << std::endl;
  println(ero_v2e_ima, x_box);


  ima_t ero_e2v_ima = erosion_e2v(x);
  std::cout << "ero_e2v_ima:" << std::endl;
  println(ero_e2v_ima, x_box);

  ima_t dil_v2e_ima = dilation_v2e(x);
  std::cout << "dil_v2e_ima:" << std::endl;
  println(dil_v2e_ima, x_box);


  ima_t dil_ima = dilation_graph(x);
  std::cout << "dil_ima:" << std::endl;
  println(dil_ima, x_box);

  ima_t ero_ima = erosion_graph(x);
  std::cout << "ero_ima:" << std::endl;
  println(ero_ima, x_box);


  ima_t alpha3_ima = alpha3(x);
  std::cout << "alpha3_ima:" << std::endl;
  println(alpha3_ima, x_box);

  ima_t beta3_ima = beta3(x);
  std::cout << "beta3_ima:" << std::endl;
  println(beta3_ima, x_box);

  // --------- //
  // Filters.  //
  // --------- //

  image2d<bool> y_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/y.pbm");
  ima_t y = make_regular_complex1d_image(y_pbm);
  box2d y_box(y_pbm.nrows() / 2 + 1, y_pbm.ncols() / 2 + 1);
  std::cout << "y:" << std::endl;
  println(y, y_box);

  ima_t ope_ima = opening_graph(y);
  std::cout << "ope_ima:" << std::endl;
  println(ope_ima, y_box);

  ima_t half_ope_ima = half_opening_graph(y);
  std::cout << "half_ope_ima:" << std::endl;
  println(half_ope_ima, y_box);

  ima_t beta3_o_alpha3_ima = beta3(alpha3(y));
  std::cout << "beta3_o_alpha3_ima:" << std::endl;
  println(beta3_o_alpha3_ima, y_box);


  image2d<bool> z_pbm = io::pbm::load(MLN_APPS_DIR "/graph-morpho/z.pbm");
  ima_t z = make_regular_complex1d_image(z_pbm);
  box2d z_box(z_pbm.nrows() / 2 + 1, z_pbm.ncols() / 2 + 1);
  std::cout << "z:" << std::endl;
  println(z, z_box);

  ima_t clo_ima = closing_graph(z);
  std::cout << "clo_ima:" << std::endl;
  println(clo_ima, z_box);

  ima_t half_clo_ima = half_closing_graph(z);
  std::cout << "half_clo_ima:" << std::endl;
  println(half_clo_ima, z_box);

  ima_t alpha3_o_beta3_ima = alpha3(beta3(z));
  std::cout << "alpha3_o_beta3_ima:" << std::endl;
  println(alpha3_o_beta3_ima, z_box);
}
