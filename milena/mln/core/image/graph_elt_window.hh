// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_HH
# define MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_HH

/// \file mln/core/image/graph_elt_window.hh
/// \brief Definition of the elementary ``window'' on a graph.

/* FIXME: Factor those classes:
   - mln::graph_elt_window
   - mln::graph_elt_neighborhood
   - mln::line_graph_elt_window
   - mln::line_graph_elt_neighborhood.

   See https://trac.lrde.org/olena/ticket/139.  */

# include <mln/core/concept/window.hh>
# include <mln/util/internal/graph_vertex_psite.hh>
# include <mln/core/image/graph_window_piter.hh>


namespace mln
{

  /// Forward declaration
  template <typename G, typename F> class graph_elt_window;

  namespace trait
  {

    ///FIXME: check that!
    template <typename G, typename F>
    struct window_< mln::graph_elt_window<G, F> >
    {
      typedef trait::window::size::unknown       size;
      typedef trait::window::support::irregular  support;
      typedef trait::window::definition::varying definition;
    };

  } // end of namespace mln::trait


  /// \brief Elementary window on graph class.
  template <typename G, typename F>
  class graph_elt_window : public Window< graph_elt_window<G, F> >
  {
    typedef graph_elt_window<G, F> self_;
    typedef mln_result(F) P;

  public:
    /// Associated types.
    /// \{
    /// The type of psite corresponding to the window.
    typedef internal::vertex_psite<G, F> psite;
    /// The type of site corresponding to the window.
    typedef mln_site(psite) site;
    // The type of the set of window sites (vertex ids adjacent to the
    // reference psite).
    typedef std::set<unsigned> sites_t;

    // FIXME: This is a dummy value.
    typedef void dpsite;

    /// \brief Site_Iterator type to browse the psites of the window
    /// w.r.t. the ordering of vertices.
    typedef graph_window_fwd_piter<G, F, self_> fwd_qiter;

    /// \brief Site_Iterator type to browse the psites of the window
    /// w.r.t. the reverse ordering of vertices.
    typedef graph_window_bkd_piter<G, F, self_> bkd_qiter;

    /// The default qiter type.
    typedef fwd_qiter qiter;
    /// \}

    /// Services for iterators.
    /// \{
    /// Compute the set of sites for this window around \a piter.
    template <typename Piter>
    void compute_sites_(Site_Iterator<Piter>& piter) const;
    /// \}

    /// Interface of the concept Window.
    /// \{
    /// Is the window is empty?
    bool is_empty() const;
    /// Is the window centered?
    bool is_centered() const;
    /// Is the window symmetric?
    // FIXME: We should define this more precisely.
    bool is_symmetric() const;
    /// Return the maximum coordinate gap between the window center
    /// and a window point.
    /* FIXME: This method returns a dummy value (0), since the delta
       of a window on a graph

       1. is not constant (graph vertices are not necessarily aligned on
          a regular grid);

       2. depends on the underlying graph, too.

       It raises another question: should delta() be part of the
       concept ``Window''?  */
    unsigned delta() const;
    /// Apply a central symmetry to the target window.
    self_& sym();
    /// \}
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename G, typename F>
  template <typename Piter>
  inline
  void
  graph_elt_window<G, F>::compute_sites_(Site_Iterator<Piter>& piter_) const
  {
    Piter& piter = exact(piter_);
    const G& g = piter.center().graph();

    unsigned central_vertex = piter.center().v().id();
    sites_t& sites = piter.sites();
    sites.clear();

    sites.insert(central_vertex);
    for (unsigned i = 0; i < g.v_nmax_nbh_vertices(central_vertex); ++i)
      sites.insert(g.v_ith_nbh_vertex(central_vertex, i));
  }

  template <typename G, typename F>
  inline
  bool
  graph_elt_window<G, F>::is_empty() const
  {
    return false;
  }

  template <typename G, typename F>
  inline
  bool
  graph_elt_window<G, F>::is_centered() const
  {
    return false;
  }

  template <typename G, typename F>
  inline
  bool
  graph_elt_window<G, F>::is_symmetric() const
  {
    return true;
  }

  template <typename G, typename F>
  inline
  unsigned
  graph_elt_window<G, F>::delta() const
  {
    // Dummy value (see the interface of the method above).
    return 0;
  }

  template <typename G, typename F>
  inline
  graph_elt_window<G, F>&
  graph_elt_window<G, F>::sym()
  {
    return *this;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_HH
