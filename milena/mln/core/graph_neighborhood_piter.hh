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

#ifndef MLN_CORE_GRAPH_NEIGHBORHOOD_PITER_HH
# define MLN_CORE_GRAPH_NEIGHBORHOOD_PITER_HH

/// \file   mln/core/graph_neighborhood_piter.hh
/// \brief  Definition of a point iterator on a graph neighborhood.

/* FIXME: Factor those classes:

   - mln::graph_window_fwd_piter
   - mln::graph_neighborhood_fwd_piter
   - mln::line_graph_window_fwd_piter
   - mln::line_graph_neighborhood_fwd_piter.
   - mln::graph_window_bkd_piter
   - mln::graph_neighborhood_bkd_piter
   - mln::line_graph_window_bkd_piter
   - mln::line_graph_neighborhood_bkd_piter.  */

# include <mln/core/concept/point_iterator.hh>
# include <mln/core/p_graph.hh>
# include <mln/core/graph_psite.hh>

/* FIXME: Due to the poor interface of mln::p_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{
  // Fwd decls.
  template <typename P> class p_graph;
  template <typename P> class graph_psite;


  /*----------------------------------.
  | graph_neighborhood_fwd_piter<P>.  |
  `----------------------------------*/

  /// \brief Forward iterator on graph neighborhood.
  template <typename P>
  class graph_neighborhood_fwd_piter :
    public Point_Iterator< graph_neighborhood_fwd_piter<P> > // or Iterator<...>?
  {
    typedef graph_neighborhood_fwd_piter<P> self_;
    typedef Point_Iterator< self_ > super_;

  public:
    enum { dim = P::dim };

    typedef graph_psite<P> psite;
    typedef P point;
    typedef mln_coord(P) coord;
    // FIXME: Dummy typedef.
    typedef void dpoint;
    // FIXME: Dummy value.
    typedef void mesh;

  public:
    /// Construction.
    /// \{
    template <typename N, typename Pref>
    graph_neighborhood_fwd_piter(const N& nbh, const Point_Site<Pref>& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid() const;
    /// Invalidate the iterator.
    void invalidate();
    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();
    /// Is the piter adjacent or equal to the reference point?
    bool adjacent_or_equal_to_p_ref_() const;
    /// Update the internal data of the iterator.
    void update_();
    /// \}

    /// Conversion and accessors.
    /// Reference to the corresponding point.
    const point& to_point() const;
    /// Reference to the corresponding point site.
    const psite& to_psite() const;
    /// Convert the iterator into a line graph psite.
    operator psite() const;

    /// Read-only access to the \a i-th coordinate.
    coord operator[](unsigned i) const;

  private:
    /// The ``central'' psite of the neighborhood.
    const psite& p_ref_;
    /// An internal iterator on the set of nodes of the underlying graph.
    util::node_id id_;
    /// The psite corresponding to this iterator.
    psite psite_;
    /// The point corresponding to this iterator.
    point p_;
  };


  /*----------------------------------.
  | graph_neighborhood_bkd_piter<P>.  |
  `----------------------------------*/

  /// \brief Backward iterator on graph neighborhood.
  template <typename P>
  class graph_neighborhood_bkd_piter :
    public Point_Iterator< graph_neighborhood_bkd_piter<P> > // or Iterator<...>?
  {
    typedef graph_neighborhood_bkd_piter<P> self_;
    typedef Point_Iterator< self_ > super_;

  public:
    enum { dim = P::dim };

    typedef graph_psite<P> psite;
    typedef P point;
    typedef mln_coord(P) coord;
    // FIXME: Dummy typedef.
    typedef void dpoint;
    // FIXME: Dummy value.
    typedef void mesh;

  public:
    /// Construction.
    /// \{
    template <typename N, typename Pref>
    graph_neighborhood_bkd_piter(const N& nbh, const Point_Site<Pref>& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid() const;
    /// Invalidate the iterator.
    void invalidate();
    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();
    /// Is the piter adjacent or equal to the reference point?
    bool adjacent_or_equal_to_p_ref_() const;
    /// Update the internal data of the iterator.
    void update_();
    /// \}

    /// Conversion and accessors.
    /// Reference to the corresponding point.
    const point& to_point() const;
    /// Reference to the corresponding point site.
    const psite& to_psite() const;
    /// Convert the iterator into a line graph psite.
    operator psite() const;

    /// Read-only access to the \a i-th coordinate.
    coord operator[](unsigned i) const;

  private:
    /// The ``central'' psite of the neighborhood.
    const psite& p_ref_;
    /// An internal iterator on the set of nodes of the underlying graph.
    util::node_id id_;
    /// The psite corresponding to this iterator.
    psite psite_;
    /// The point corresponding to this iterator.
    point p_;
  };



# ifndef MLN_INCLUDE_ONLY

  /*----------------------------------.
  | graph_neighborhood_fwd_piter<P>.  |
  `----------------------------------*/

  // FIXME: Currently, argument nbh is ignored.
  template <typename P>
  template <typename N, typename Pref>
  inline
  graph_neighborhood_fwd_piter<P>::graph_neighborhood_fwd_piter(const N& /* nbh */,
								const Point_Site<Pref>& p_ref)
    : p_ref_(exact(p_ref).to_psite()),
      // Initialize psite_ to a dummy value.
      psite_(),
      p_()
  {
    // Invalidate id_.
    invalidate();
  }

  template <typename P>
  inline
  bool
  graph_neighborhood_fwd_piter<P>::is_valid() const
  {
    // FIXME: We depend too much on the implementation of util::graph
    // here.  The util::graph should provide the service to abstract
    // these manipulations.
    return p_ref_.is_valid() && id_ < p_ref_.pg().gr_->nnodes();
  }

  template <typename P>
  inline
  void
  graph_neighborhood_fwd_piter<P>::invalidate()
  {
    id_ = -1
  }

  template <typename P>
  inline
  void
  graph_neighborhood_fwd_piter<P>::start()
  {
    id_ = 0;
    if (!adjacent_or_equal_to_p_ref_())
      next_();
    /* FIXME: This is redundant with the end of next_(), but we might
       change the implementation of start_() when we'll fix it later,
       and no longer use next_().  */
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  void
  graph_neighborhood_fwd_piter<P>::next_()
  {
    /* FIXME: This is inefficient.  The graph structure should be able
       to produce the set of adjacent nodes fast.  Boost Graphs
       probably provides adequates structures to fetch these
       neighbors in constant time.  */
    /* FIXME: Moreover, the behavior of next shall depend on the
       neighborhood, which is not the case now!  (Currently, next_() behaves
       as nbh was always an elementary neighborhood.) */
    do
      ++id_;
    while (is_valid() && !adjacent_or_equal_to_p_ref_());
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  bool
  graph_neighborhood_fwd_piter<P>::adjacent_or_equal_to_p_ref_() const
  {
    return p_ref_.pg().adjacent_or_equal(p_ref_.id(), id_);
  }

  template <typename P>
  inline
  void
  graph_neighborhood_fwd_piter<P>::update_()
  {
    // Update psite_.
    psite_ = graph_psite<P>(p_ref_.pg(), id_);
    // Update p_.
    p_ = p_ref_.pg().gr_->node_data(id_);
  }

  template <typename P>
  inline
  const P&
  graph_neighborhood_fwd_piter<P>::to_point() const
  {
    return p_;
  }

  template <typename P>
  inline
  const graph_psite<P>&
  graph_neighborhood_fwd_piter<P>::to_psite() const
  {
    return psite_;
  }

  template <typename P>
  inline
  graph_neighborhood_fwd_piter<P>::operator graph_psite<P>() const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <typename P>
  inline
  mln_coord(P)
  graph_neighborhood_fwd_piter<P>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }


  /*----------------------------------.
  | graph_neighborhood_bkd_piter<P>.  |
  `----------------------------------*/

  // FIXME: Currently, argument nbh is ignored.
  template <typename P>
  template <typename N, typename Pref>
  inline
  graph_neighborhood_bkd_piter<P>::graph_neighborhood_bkd_piter(const N& /* nbh */,
								const Point_Site<Pref>& p_ref)
    : p_ref_(exact(p_ref).to_psite()),
      // Initialize psite_ to a dummy value.
      psite_(),
      p_()
  {
    // Invalidate id_.
    invalidate();
  }

  template <typename P>
  inline
  bool
  graph_neighborhood_bkd_piter<P>::is_valid() const
  {
    // FIXME: We depend too much on the implementation of util::graph
    // here.  The util::graph should provide the service to abstract
    // these manipulations.
    return p_ref_.is_valid() && id_ < p_ref_.pg().gr_->nnodes();
  }

  template <typename P>
  inline
  void
  graph_neighborhood_bkd_piter<P>::invalidate()
  {
    id_ = -1;
  }

  template <typename P>
  inline
  void
  graph_neighborhood_bkd_piter<P>::start()
  {
    id_ = p_ref_.pg().gr_->nnodes() - 1;
    if (!adjacent_or_equal_to_p_ref_())
      next_();
    /* FIXME: This is redundant with the end of next_(), but we might
       change the implementation of start_() when we'll fix it later,
       and no longer use next_().  */
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  void
  graph_neighborhood_bkd_piter<P>::next_()
  {
    /* FIXME: This is inefficient.  The graph structure should be able
       to produce the set of adjacent nodes fast.  Boost Graphs
       probably provides adequates structures to fetch these
       neighbors in constant time.  */
    /* FIXME: Moreover, the behavior of next shall depend on the
       neighborhood, which is not the case now!  (Currently, next_() behaves
       as nbh was always an elementary neighborhood.) */
    do
      --id_;
    while (is_valid() && !adjacent_or_equal_to_p_ref_());
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  bool
  graph_neighborhood_bkd_piter<P>::adjacent_or_equal_to_p_ref_() const
  {
    return p_ref_.pg().adjacent_or_equal(p_ref_.id(), id_);
  }

  template <typename P>
  inline
  void
  graph_neighborhood_bkd_piter<P>::update_()
  {
    // Update psite_.
    psite_ = graph_psite<P>(p_ref_.pg(), id_);
    // Update p_.
    p_ = p_ref_.pg().gr_->node_data(id_);
  }

  template <typename P>
  inline
  const P&
  graph_neighborhood_bkd_piter<P>::to_point() const
  {
    return p_;
  }

  template <typename P>
  inline
  const graph_psite<P>&
  graph_neighborhood_bkd_piter<P>::to_psite() const
  {
    return psite_;
  }

  template <typename P>
  inline
  graph_neighborhood_bkd_piter<P>::operator graph_psite<P>() const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <typename P>
  inline
  mln_coord(P)
  graph_neighborhood_bkd_piter<P>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_GRAPH_NEIGHBORHOOD_PITER_HH
