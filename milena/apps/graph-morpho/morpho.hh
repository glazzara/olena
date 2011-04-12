// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef APPS_GRAPH_MORPHO_MORPHO_HH
# define APPS_GRAPH_MORPHO_MORPHO_HH

/** \file apps/graph-morpho/morpho.hh
    \brief Morphological operators on graphs.

    Reference:

      Jean Cousty, Laurent Najman and Jean Serra.  Some morphological
      operators in graph spaces.  In: Proceedings of the Ninth
      International Symposium on Mathematical Morphology (ISMM), 2009,
      Groningen, The Netherlands.  */

# include <mln/core/alias/complex_image.hh>
# include <mln/core/image/image2d.hh>

# include <mln/core/image/dmorph/image_if.hh>

# include <mln/core/image/dmorph/extension_ima.hh>

# include <mln/core/routine/extend.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/core/site_set/p_n_faces_piter.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>

# include <mln/world/inter_pixel/dim2/is_pixel.hh>
# include <mln/world/inter_pixel/dim2/is_edge.hh>
# include <mln/world/inter_pixel/neighb2d.hh>

# include <mln/data/paste.hh>

# include <mln/morpho/dilation.hh>
# include <mln/morpho/erosion.hh>

# include <mln/topo/is_n_face.hh>


/*---------------.
| Graph traits.  |
`---------------*/

namespace trait
{
  /*!
    \internal
    \brief Graph traits.
  */
  template <typename I>
  struct graph
  {
    /* Empty by default.

       Specializations must provide these static member functions:

         static F1 is_vertex()
         static F2 is_edge()
         static W1 v2e()
         static W2 e2v()

       where F1, F2, W1 and W2 are types depending on I.
    */
  };

  // ----------------------------------------------------------- //
  // Graph traits for (mln::image2d-based) cubical 2-complexes.  //
  // ----------------------------------------------------------- //

  /*!
    \internal
    \brief Graph traits for mln::image2d.
  */
  template <typename T>
  struct graph< mln::image2d<T> >
  {
    // Return a functor saying whether a psite is a vertex or not.
    static
    const mln::world::inter_pixel::dim2::is_pixel& is_vertex()
    {
      static mln::world::inter_pixel::dim2::is_pixel is_vertex_fun;
      return is_vertex_fun;
    }

    // Return a functor saying whether a psite is a vertex or not.
    static
    const mln::world::inter_pixel::dim2::is_edge& is_edge()
    {
      static mln::world::inter_pixel::dim2::is_edge is_edge_fun;
      return is_edge_fun;
    }

    // Return a window containing the edges adjacent to a vertex.
    static
    const mln::window2d& v2e()
    {
      return mln::world::inter_pixel::v2e().win();
    }

    // Return a window containing the vertices adjacent to an edge.
    static
    const mln::world::inter_pixel::dbl_window2d& e2v()
    {
      return mln::world::inter_pixel::e2v().win();
    }
  };

  // ---------------------------------------- //
  // Graph traits for (general) 1-complexes.  //
  // ---------------------------------------- //

  /*!
    \internal
    \brief Graph traits for 1-complexes images.
  */
  template <typename G, typename V>
  struct graph< mln::complex_image<1, G, V> >
  {
    // Return a functor saying whether a psite is a vertex or not.
    static
    const mln::topo::is_n_face<mln::complex_psite<1, G>, 0>& is_vertex()
    {
      static mln::topo::is_n_face<mln::complex_psite<1, G>, 0> is_vertex_fun;
      return is_vertex_fun;
    }

    // Return a functor saying whether a psite is a vertex or not.
    static
    const mln::topo::is_n_face<mln::complex_psite<1, G>, 1>& is_edge()
    {
      static mln::topo::is_n_face<mln::complex_psite<1, G>, 1> is_edge_fun;
      return is_edge_fun;
    }

    // Return a window containing the edges adjacent to a vertex.
    static
    const mln::complex_higher_window<1, G>& v2e()
    {
      static mln::complex_higher_window<1, G> v2e_win;
      return v2e_win;
    }

    // Return a window containing the vertices adjacent to an edge.
    static
    const mln::complex_lower_window<1, G>& e2v()
    {
      static mln::complex_lower_window<1, G> e2v_win;
      return e2v_win;
    }
  };

} // end of namespace trait


/*----------------------------.
| Vertices-edges combinator.  |
`----------------------------*/

/* The original paper uses an operator `\ovee' to combine node and
   edge graphs.  However, this symbol requires the use of the
   `stmaryrd' package, and Doyxgen provides no means to require it.
   So, we use a more ``standard'' symbol instead (`\oplus').  */

/// Combine the vertices and the edges of two images to create a new
/// graph image (``operator'' \f$\oplus\f$).
template <typename I>
inline
mln_concrete(I)
combine(const mln::Image<I>& vertices_, const mln::Image<I>& edges_)
{
  typedef trait::graph<I> T;
  const I& vertices = mln::exact(vertices_);
  const I& edges = mln::exact(edges_);

  mln_precondition(vertices.domain() == edges.domain());
  mln_concrete(I) output;
  mln::initialize(output, vertices);
  mln::data::fill(output, false);
  mln::data::paste(vertices | T::is_vertex(), output);
  mln::data::paste(edges | T::is_edge(), output);
  return output;
}


/*-------------------------.
| Dilations and erosions.  |
`-------------------------*/

// ----------------------------- //
// Core dilations and erosions.  //
// ----------------------------- //

/* Note: When writing

     dilation(ima | vertices);

   `vertices' is a predicate on sites (p2b function), which
   is not efficient, since both vertices and edges will be browsed.

   It would be very nice if `vertices' could be an actual site set,
   so that `ima | vertices' creates a morpher smart enough to
   browse /only/ vertices.  */


/// Dilation from edges to vertices (\f$\delta^\bullet\f$).
template <typename I>
inline
mln_concrete(I)
dilation_e2v(const mln::Image<I>& input)
{
  typedef trait::graph<I> T;

  mln_concrete(I) output;
  mln::initialize(output, mln::exact(input));
  mln::data::fill(output, false);
  mln::data::paste(mln::morpho::dilation(mln::extend(input | T::is_vertex(),
						       input),
					   T::v2e()),
		     output);
  return output;
}

/// Erosion from vertices to edges (\f$\epsilon^\times\f$).
template <typename I>
inline
mln_concrete(I)
erosion_v2e(const mln::Image<I>& input)
{
  typedef trait::graph<I> T;

  mln_concrete(I) output;
  mln::initialize(output, mln::exact(input));
  mln::data::fill(output, false);
  mln::data::paste(mln::morpho::erosion(mln::extend(input | T::is_edge(),
						      input),
					  T::e2v()),
		     output);
  return output;
}

/// Erosion from edges to vertices (\f$\epsilon^\bullet\f$).
template <typename I>
inline
mln_concrete(I)
erosion_e2v(const mln::Image<I>& input)
{
  typedef trait::graph<I> T;

  mln_concrete(I) output;
  mln::initialize(output, mln::exact(input));
  mln::data::fill(output, false);
  mln::data::paste(mln::morpho::erosion(mln::extend(input | T::is_vertex(),
						      input),
					   T::v2e()),
		     output);
  return output;
}

/// Dilation from vertices to edges (\f$\delta^\times\f$).
template <typename I>
inline
mln_concrete(I)
dilation_v2e(const mln::Image<I>& input)
{
  typedef trait::graph<I> T;

  mln_concrete(I) output;
  mln::initialize(output, mln::exact(input));
  mln::data::fill(output, false);
  mln::data::paste(mln::morpho::dilation(mln::extend(input | T::is_edge(),
						       input),
					   T::e2v()),
		     output);
  return output;
}


// ------------------------------ //
// Other dilations and erosions.  //
// ------------------------------ //

/// Vertex dilation (\f$\delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_vertex(const mln::Image<I>& input)
{
  return dilation_e2v(dilation_v2e(input));
}

/// Vertex erosion (\f$\epsilon\f$).
template <typename I>
inline
mln_concrete(I)
erosion_vertex(const mln::Image<I>& input)
{
  return erosion_e2v(erosion_v2e(input));
}


/// Edge dilation (\f$\Delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_edge(const mln::Image<I>& input)
{
  return dilation_v2e(dilation_e2v(input));
}

/// Edge erosion (\f$\mathcal{E}\f$).
template <typename I>
inline
mln_concrete(I)
erosion_edge(const mln::Image<I>& input)
{
  return erosion_v2e(erosion_e2v(input));
}


/// Graph dilation (\f$\delta \oplus \Delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_graph(const mln::Image<I>& input)
{
  return combine(dilation_vertex(input), dilation_edge(input));
}

/// Graph erosion (\f$\epsilon \oplus \mathcal{E}\f$).
template <typename I>
inline
mln_concrete(I)
erosion_graph(const mln::Image<I>& input)
{
  return combine(erosion_vertex(input), erosion_edge(input));
}


/*-------------------------.
| Additional adjunctions.  |
`-------------------------*/

template <typename I>
inline
mln_concrete(I)
alpha1(const mln::Image<I>& input)
{
  mln_concrete(I) vertices;
  mln::initialize(vertices, input);
  mln::data::fill(vertices, true);
  return combine(vertices, input);
}

template <typename I>
inline
mln_concrete(I)
beta1(const mln::Image<I>& input)
{
  return combine(dilation_e2v(input), input);
}

template <typename I>
inline
mln_concrete(I)
alpha2(const mln::Image<I>& input)
{
  return combine(input, erosion_v2e(input));
}

template <typename I>
inline
mln_concrete(I)
beta2(const mln::Image<I>& input)
{
  mln_concrete(I) edges;
  mln::initialize(edges, input);
  mln::data::fill(edges, false);
  return combine(input, edges);
}

template <typename I>
inline
mln_concrete(I)
alpha3(const mln::Image<I>& input)
{
  return combine(erosion_e2v(input), erosion_v2e(erosion_e2v(input)));
}

template <typename I>
inline
mln_concrete(I)
beta3(const mln::Image<I>& input)
{
  return combine(dilation_e2v(dilation_v2e(input)), dilation_v2e(input));
}


/*------------------------.
| Openings and closings.  |
`------------------------*/

/// Vertex opening (\f$\gamma_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_vertex(const mln::Image<I>& input)
{
  return dilation_vertex(erosion_vertex(input));
}

/// Vertex closing (\f$\phi_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_vertex(const mln::Image<I>& input)
{
  return erosion_vertex(dilation_vertex(input));
}


/// Edge opening (\f$\Gamma_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_edge(const mln::Image<I>& input)
{
  return dilation_edge(erosion_edge(input));
}

/// Edge closing (\f$\Phi_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_edge(const mln::Image<I>& input)
{
  return erosion_edge(dilation_edge(input));
}


/// Graph opening (\f${\gamma \oplus \Gamma}_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_graph(const mln::Image<I>& input)
{
  return combine(opening_vertex(input), opening_edge(input));
}

/// Graph closing (\f${\phi \oplus \Phi}_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_graph(const mln::Image<I>& input)
{
  return combine(closing_vertex(input), closing_edge(input));
}


/*----------------------------------.
| Half-openings and half-closings.  |
`----------------------------------*/

/// Vertex half-opening (\f$\gamma_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_vertex(const mln::Image<I>& input)
{
  return dilation_e2v(erosion_v2e(input));
}

/// Vertex half-closing (\f$\phi_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_vertex(const mln::Image<I>& input)
{
  return erosion_e2v(dilation_v2e(input));
}


/// Edge half-opening (\f$\Gamma_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_edge(const mln::Image<I>& input)
{
  return dilation_v2e(erosion_e2v(input));
}

/// Edge half-closing (\f$\Phi_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_edge(const mln::Image<I>& input)
{
  return erosion_v2e(dilation_e2v(input));
}


/// Graph half-opening (\f${\gamma \oplus \Gamma}_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_graph(const mln::Image<I>& input)
{
  return combine(half_opening_vertex(input), half_opening_edge(input));
}

/// Graph half-closing (\f${\phi \oplus \Phi}_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_graph(const mln::Image<I>& input)
{
  return combine(half_closing_vertex(input), half_closing_edge(input));
}


/*-------------------------------------------------------.
| Parameterized openings and closings (granulometries).  |
`-------------------------------------------------------*/

/// Opening (\f${\gamma \oplus \Gamma}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
opening(const mln::Image<I>& input, unsigned lambda)
{
  unsigned i = lambda / 2;
  unsigned j = lambda % 2;
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 0; m < i; ++m)
    output = erosion_graph(output);
  for (unsigned m = 0; m < j; ++m)
    output = half_opening_graph(output);
  for (unsigned m = 0; m < i; ++m)
    output = dilation_graph(output);
  return output;
}

/// Opening (\f${\phi \oplus \Phi}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
closing(const mln::Image<I>& input, unsigned lambda)
{
  unsigned i = lambda / 2;
  unsigned j = lambda % 2;
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 0; m < i; ++m)
    output = dilation_graph(output);
  for (unsigned m = 0; m < j; ++m)
    output = half_closing_graph(output);
  for (unsigned m = 0; m < i; ++m)
    output = erosion_graph(output);
  return output;
}

/*-------------------------------.
| Alternate Sequential Filters.  |
`-------------------------------*/

/// Alternate Sequential Filter (ASF) (\f${ASF}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
asf(const mln::Image<I>& input, unsigned lambda)
{
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 1; m <= lambda; ++m)
    output = opening(closing(output, m), m);
  return output;
}

#endif // ! APPS_GRAPH_MORPHO_MORPHO_HH
