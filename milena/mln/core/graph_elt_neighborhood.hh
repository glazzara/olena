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

#ifndef MLN_CORE_GRAPH_ELT_NEIGHBORHOOD_HH
# define MLN_CORE_GRAPH_ELT_NEIGHBORHOOD_HH

/// \file mln/core/graph_elt_neighborhood.hh
/// \brief Definition of the elementary ``neighborhood'' on a graph.

/* FIXME: Have a consistent naming: we have neighborhood (without '_')
   but point_, neighb_, etc.  */

/* FIXME: Factor those classes:
   - mln::graph_elt_window
   - mln::graph_elt_neighborhood
   - mln::line_graph_elt_window
   - mln::line_graph_elt_neighborhood.  */

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/graph_psite.hh>
# include <mln/core/graph_neighborhood_piter.hh>


namespace mln
{
  // Fwd decls.
  template <typename P, typename N> class graph_neighborhood_fwd_piter;
  template <typename P, typename N> class graph_neighborhood_bkd_piter;


  /// Elementary neighborhood on graph class.
  template <typename P>
  class graph_elt_neighborhood
    : public Neighborhood< graph_elt_neighborhood<P> >
  {
    typedef graph_elt_neighborhood<P> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of point corresponding to the neighborhood.
    typedef P point;
    /// The type of psite corresponding to the neighborhood.
    typedef graph_psite<P> psite;

    // FIXME: This is a dummy value.
    typedef void dpoint;

    /// \brief Point_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the ordering of vertices.
    typedef graph_neighborhood_fwd_piter<P, self_> fwd_niter;

    /// \brief Point_Iterator type to browse the psites of the
    /// neighborhood w.r.t. the reverse ordering of vertices.
    typedef graph_neighborhood_bkd_piter<P, self_> bkd_niter;

    /// The default niter type.
    typedef fwd_niter niter;
    /// \}

    /// Services for iterators.
    /// \{
    /// Move \a piter to the beginning of the iteration on this neighborhood.
    template <typename Piter>
    void start(Point_Iterator<Piter>& piter) const;
    /// Move \a piter to the next site on this neighborhood.
    template <typename Piter>
    void next_(Point_Iterator<Piter>& piter) const;
    /// \}
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  template <typename Piter>
  inline
  void
  graph_elt_neighborhood<P>::start(Point_Iterator<Piter>& piter_) const
  {
    Piter& piter = exact(piter_);
    piter.first_();
    if (!piter.adjacent_or_equal_to_p_ref_())
      next_(piter);
  }

  template <typename P>
  template <typename Piter>
  inline
  void
  graph_elt_neighborhood<P>::next_(Point_Iterator<Piter>& piter_) const
  {
    Piter& piter = exact(piter_);
    /* FIXME: This is inefficient.  The graph structure should be able
       to produce the set of adjacent nodes fast.  Boost Graphs
       probably provides adequates structures to fetch these
       neighbors in constant time.  */
    do
      piter.step_();
    while (piter.is_valid() && !piter.adjacent_or_equal_to_p_ref_());
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_GRAPH_ELT_NEIGHBORHOOD_HH
