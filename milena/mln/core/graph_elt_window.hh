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

#ifndef MLN_CORE_GRAPH_ELT_WINDOW_HH
# define MLN_CORE_GRAPH_ELT_WINDOW_HH

/// \file mln/core/graph_elt_window.hh
/// \brief Definition of the elementary ``window'' on a graph.

/* FIXME: Have a consistent naming: we have window (without '_') but
   point_, neighb_, etc.  */

/* FIXME: Factor those classes:
   - mln::graph_elt_window
   - mln::graph_elt_neighborhood
   - mln::line_graph_elt_window
   - mln::line_graph_elt_neighborhood.  */

/* FIXME: Due to the poor interface of mln::p_line_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

# include <mln/core/concept/window.hh>
# include <mln/core/graph_psite.hh>
# include <mln/core/graph_window_piter.hh>


namespace mln
{
  // Fwd decls.
  template <typename P, typename W> class graph_window_fwd_piter;
  template <typename P, typename W> class graph_window_bkd_piter;


  /// \brief Elementary window on graph class.
  template <typename P>
  class graph_elt_window : public Window< graph_elt_window<P> >
  {
    typedef graph_elt_window<P> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of point corresponding to the window.
    typedef P point;
    /// The type of psite corresponding to the window.
    typedef graph_psite<P> psite;
    // The type of the set of window sites (vertex ids adjacent to the
    // reference psite).
    typedef std::set<util::vertex_id> sites_t;

    // FIXME: This is a dummy value.
    typedef void dpoint;

    /// \brief Point_Iterator type to browse the psites of the window
    /// w.r.t. the ordering of vertices.
    typedef graph_window_fwd_piter<P, self_> fwd_qiter;

    /// \brief Point_Iterator type to browse the psites of the window
    /// w.r.t. the reverse ordering of vertices.
    typedef graph_window_bkd_piter<P, self_> bkd_qiter;

    /// The default qiter type.
    typedef fwd_qiter qiter;
    /// \}

    /// Services for iterators.
    /// \{
    /// Compute the set of sites for this window around \a piter.
    template <typename Piter>
    void compute_sites_(Point_Iterator<Piter>& piter) const;
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

  template <typename P>
  template <typename Piter>
  inline
  void
  graph_elt_window<P>::compute_sites_(Point_Iterator<Piter>& piter_) const
  {
    Piter& piter = exact(piter_);
    util::vertex_id ref_vertex_id = piter.p_ref().id();
    const util::vertex<P>& ref_vertex = piter.pg().gr_->vertex(ref_vertex_id);
    sites_t& sites = piter.sites();
    sites.clear();
    /* FIXME: Move this computation out of the window. In fact,
       this should be a service of the graph, also proposed by the
       p_line_graph.  */
    /* Adjacent vertices.

       We don't need to explicitely insert the reference piter (vertex
       id) itself into SITES, since it is part of the set of vertices
       adjacent to V1 and V2, and will therefore be
       automatically added.  */
    for (std::vector<util::edge_id>::const_iterator e =
	   ref_vertex.edges.begin();
	 e != ref_vertex.edges.end(); ++e)
      {
	util::vertex_id v1 = piter.pg().gr_->edges()[*e]->v1();
	sites.insert(v1);
	util::vertex_id v2 = piter.pg().gr_->edges()[*e]->v2();
	sites.insert(v2);
      }
  }

  template <typename P>
  inline
  bool
  graph_elt_window<P>::is_empty() const
  {
    return false;
  }

  template <typename P>
  inline
  bool
  graph_elt_window<P>::is_centered() const
  {
    return false;
  }

  template <typename P>
  inline
  bool
  graph_elt_window<P>::is_symmetric() const
  {
    return true;
  }

  template <typename P>
  inline
  unsigned
  graph_elt_window<P>::delta() const
  {
    // Dummy value (see the interface of the method above).
    return 0;
  }

  template <typename P>
  inline
  graph_elt_window<P>&
  graph_elt_window<P>::sym()
  {
    return *this;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_GRAPH_ELT_WINDOW_HH
