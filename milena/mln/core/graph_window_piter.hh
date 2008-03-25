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

#ifndef MLN_CORE_GRAPH_WINDOW_PITER_HH
# define MLN_CORE_GRAPH_WINDOW_PITER_HH

/// \file   mln/core/graph_window_piter.hh
/// \brief  Definition of a point iterator on a graph window.

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


  /*-------------------------------.
  | graph_window_fwd_piter<P, W>.  |
  `-------------------------------*/

  /// \brief Forward iterator on graph window.
  template <typename P, typename W>
  class graph_window_fwd_piter :
    public Point_Iterator< graph_window_fwd_piter<P, W> >
  {
    typedef graph_window_fwd_piter<P, W> self_;
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
    template <typename Pref>
    graph_window_fwd_piter(const W& win, const Point_Site<Pref>& p_ref);
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
    /// \{
    /// Reference to the corresponding point.
    const point& to_point() const;
    /// Reference to the corresponding point site.
    const psite& to_psite() const;
    /// Convert the iterator into a line graph psite.
    operator psite() const;

    /// Read-only access to the \a i-th coordinate.
    coord operator[](unsigned i) const;
    /// \}

    /// Internals, used by the window.
    /// \{
  public:
    /// Set the iterator to the first site of the graph.
    void first_();
    /// Advance the position of the iterator by one step.
    void step_();

    /// An internal iterator on the set of nodes of the underlying graph.
    util::node_id id_;
    /// \}

  private:
    /// The window.
    const W& win_;
    /// The ``central'' psite of the window.
    const psite& p_ref_;
    /// The psite corresponding to this iterator.
    psite psite_;
    /// The point corresponding to this iterator.
    point p_;
  };


  /*-------------------------------.
  | graph_window_bkd_piter<P, W>.  |
  `-------------------------------*/

  /// \brief Backward iterator on graph window.
  template <typename P, typename W>
  class graph_window_bkd_piter :
    public Point_Iterator< graph_window_bkd_piter<P, W> >
  {
    typedef graph_window_bkd_piter<P, W> self_;
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
    template <typename Pref>
    graph_window_bkd_piter(const W& win, const Point_Site<Pref>& p_ref);
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
    /// \{
    /// Reference to the corresponding point.
    const point& to_point() const;
    /// Reference to the corresponding point site.
    const psite& to_psite() const;
    /// Convert the iterator into a line graph psite.
    operator psite() const;

    /// Read-only access to the \a i-th coordinate.
    coord operator[](unsigned i) const;
    /// \}

    /// Internals, used by the window.
    /// \{
  public:
    /// Set the iterator to the first site of the graph.
    void first_();
    /// Advance the position of the iterator by one step.
    void step_();

    /// An internal iterator on the set of nodes of the underlying graph.
    util::node_id id_;
    /// \}

  private:
    /// The window.
    const W& win_;
    /// The ``central'' psite of the window.
    const psite& p_ref_;
    /// The psite corresponding to this iterator.
    psite psite_;
    /// The point corresponding to this iterator.
    point p_;
  };



# ifndef MLN_INCLUDE_ONLY

  /*-------------------------------.
  | graph_window_fwd_piter<P, W>.  |
  `-------------------------------*/

  template <typename P, typename W>
  template <typename Pref>
  inline
  graph_window_fwd_piter<P, W>::graph_window_fwd_piter(const W& win,
						       const Point_Site<Pref>& p_ref)
    : win_(exact(win)),
      p_ref_(exact(p_ref).to_psite()),
      // Initialize psite_ to a dummy value.
      psite_(),
      p_()
  {
    // Invalidate id_.
    invalidate();
  }

  template <typename P, typename W>
  inline
  bool
  graph_window_fwd_piter<P, W>::is_valid() const
  {
    return p_ref_.is_valid() && id_ < p_ref_.pg().npoints();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::invalidate()
  {
    id_ = -1;
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::start()
  {
    win_.start(*this);
    if (is_valid())
      update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::next_()
  {
    win_.next_(*this);
    if (is_valid())
      update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::first_()
  {
    id_ = 0;
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::step_()
  {
    ++id_;
  }


  template <typename P, typename W>
  inline
  bool
  graph_window_fwd_piter<P, W>::adjacent_or_equal_to_p_ref_() const
  {
    return p_ref_.pg().adjacent_or_equal(p_ref_.id(), id_);
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::update_()
  {
    // Update psite_.
    psite_ = graph_psite<P>(p_ref_.pg(), id_);
    // Update p_.
    p_ = p_ref_.pg().point_from_id(id_);
  }

  template <typename P, typename W>
  inline
  const P&
  graph_window_fwd_piter<P, W>::to_point() const
  {
    return p_;
  }

  template <typename P, typename W>
  inline
  const graph_psite<P>&
  graph_window_fwd_piter<P, W>::to_psite() const
  {
    return psite_;
  }

  template <typename P, typename W>
  inline
  graph_window_fwd_piter<P, W>::operator graph_psite<P>() const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <typename P, typename W>
  inline
  mln_coord(P)
  graph_window_fwd_piter<P, W>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }


  /*-------------------------------.
  | graph_window_bkd_piter<P, W>.  |
  `-------------------------------*/

  template <typename P, typename W>
  template <typename Pref>
  inline
  graph_window_bkd_piter<P, W>::graph_window_bkd_piter(const W& win,
						       const Point_Site<Pref>& p_ref)
    : win_(exact(win)),
      p_ref_(exact(p_ref).to_psite()),
      // Initialize psite_ to a dummy value.
      psite_(),
      p_()
  {
    // Invalidate id_.
    invalidate();
  }

  template <typename P, typename W>
  inline
  bool
  graph_window_bkd_piter<P, W>::is_valid() const
  {
    return p_ref_.is_valid() && id_ < p_ref_.pg().npoints();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::invalidate()
  {
    id_ = -1;
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::start()
  {
    win_.start(*this);
    if (is_valid())
      update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::next_()
  {
    win_.next_(*this);
    if (is_valid())
      update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::first_()
  {
    id_ = p_ref_.pg().gr_->nnodes() - 1;
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::step_()
  {
    --id_;
  }


  template <typename P, typename W>
  inline
  bool
  graph_window_bkd_piter<P, W>::adjacent_or_equal_to_p_ref_() const
  {
    return p_ref_.pg().adjacent_or_equal(p_ref_.id(), id_);
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::update_()
  {
    // Update psite_.
    psite_ = graph_psite<P>(p_ref_.pg(), id_);
    // Update p_.
    p_ = p_ref_.pg().point_from_id(id_);
  }

  template <typename P, typename W>
  inline
  const P&
  graph_window_bkd_piter<P, W>::to_point() const
  {
    return p_;
  }

  template <typename P, typename W>
  inline
  const graph_psite<P>&
  graph_window_bkd_piter<P, W>::to_psite() const
  {
    return psite_;
  }

  template <typename P, typename W>
  inline
  graph_window_bkd_piter<P, W>::operator graph_psite<P>() const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <typename P, typename W>
  inline
  mln_coord(P)
  graph_window_bkd_piter<P, W>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_GRAPH_WINDOW_PITER_HH
