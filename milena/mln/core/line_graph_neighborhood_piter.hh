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

#ifndef MLN_CORE_LINE_GRAPH_NEIGHBORHOOD_PITER_HH
# define MLN_CORE_LINE_GRAPH_NEIGHBORHOOD_PITER_HH

/// \file   mln/core/line_graph_neighborhood_piter.hh
/// \brief  Definition of a point iterator on a line graph neighborhood.

/* FIXME: Factor those classes:

   - mln::graph_window_fwd_piter
   - mln::graph_neighborhood_fwd_piter
   - mln::line_graph_window_fwd_piter
   - mln::line_graph_neighborhood_fwd_piter.
   - mln::graph_window_bkd_piter
   - mln::graph_neighborhood_bkd_piter
   - mln::line_graph_window_bkd_piter
   - mln::line_graph_neighborhood_bkd_piter.  */

# include <set>

# include <mln/core/concept/point_iterator.hh>
# include <mln/core/p_line_graph.hh>
# include <mln/core/line_graph_psite.hh>

/* FIXME: Due to the poor interface of mln::p_line_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{
  // Fwd decls.
  template <typename P> class p_line_graph;
  template <typename P> class line_graph_psite;


  /*------------------------------------------.
  | line_graph_neighborhood_fwd_piter<P, N>.  |
  `------------------------------------------*/

  /// \brief Forward iterator on line graph neighborhood.
  template <typename P, typename N>
  class line_graph_neighborhood_fwd_piter :
    public Point_Iterator< line_graph_neighborhood_fwd_piter<P, N> >
  {
    typedef line_graph_neighborhood_fwd_piter<P, N> self_;
    typedef Point_Iterator< self_ > super_;

  public:
    enum { dim = P::dim };

    typedef line_graph_psite<P> psite;
    typedef P point;
    typedef mln_coord(point) coord;
    // FIXME: Dummy typedef.
    typedef void dpoint;
    // FIXME: Dummy typedef.
    typedef void mesh;

    // The type of the set of neighbors (adjacent edge ids).
    typedef std::set<util::edge_id> neighbors_t;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    line_graph_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
				      const Point_Site<Pref>& p_ref);
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

    /// Return the reference psite.
    const psite& p_ref() const;
    /// Return the mln::p_line_graph corresponding to this piter.
    const p_line_graph<P>& plg() const; 
    /// Return the set of neighbors (adjacent edge ids).
    neighbors_t& neighbors();

    /// Read-only access to the \a i-th coordinate.
    // FIXME: Dummy.
    coord operator[](unsigned i) const;
    /// \}

  private:
    /// The neighborhood.
    const N& nbh_;
    /// The ``central'' psite of the neighborhood.
    const psite& p_ref_;

    /// The last reference psite whose ajacent psites have been computed.
    psite saved_p_ref_;
    /// The set of edge ids adjacent to the reference psite.
    neighbors_t neighbors_;
    /// An iterator on the set of adjacent edges.
    neighbors_t::const_iterator i_; 

    /// The psite corresponding to this iterator.
    psite psite_;
    /// The point corresponding to this iterator.
    /* FIXME: Dummy value.  To be removed as soon as the conversion
       from psite to point is no longer mandatory.  */
    point p_;
  };

  /* FIXME: This hand-made delegation is painful.  We should rely on
     the general mechanism provided by Point_Site.  But then again, we
     need to refine/adjust the interface of Point_Site w.r.t. the
     mandatory conversions to points.  */
  template <typename P, typename N>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const line_graph_neighborhood_fwd_piter<P, N>& p);


  /*------------------------------------------.
  | line_graph_neighborhood_bkd_piter<P, N>.  |
  `------------------------------------------*/

  /// \brief Backward iterator on line graph neighborhood.
  template <typename P, typename N>
  class line_graph_neighborhood_bkd_piter :
    public Point_Iterator< line_graph_neighborhood_bkd_piter<P, N> >
  {
    typedef line_graph_neighborhood_bkd_piter<P, N> self_;
    typedef Point_Iterator< self_ > super_;

  public:
    enum { dim = P::dim };

    typedef line_graph_psite<P> psite;
    typedef P point;
    typedef mln_coord(point) coord;
    // FIXME: Dummy typedef.
    typedef void dpoint;
    // FIXME: Dummy typedef.
    typedef void mesh;

    // The type of the set of neighbors (adjacent edge ids).
    typedef std::set<util::edge_id> neighbors_t;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    line_graph_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
				      const Point_Site<Pref>& p_ref);
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

    /// Return the reference psite.
    const psite& p_ref() const;
    /// Return the mln::p_line_graph corresponding to this piter.
    const p_line_graph<P>& plg() const;
    /// Return the set of neighbors (adjacent edge ids).
    neighbors_t& neighbors();

    /// Read-only access to the \a i-th coordinate.
    // FIXME: Dummy.
    coord operator[](unsigned i) const;
    /// \}

  private:
    /// The neighborhood.
    const N& nbh_;
    /// The ``central'' psite of the neighborhood.
    const psite& p_ref_;

    /// The last reference psite whose ajacent psites have been computed.
    psite saved_p_ref_;
    /// The set of edge ids adjacent to the reference psite.
    neighbors_t neighbors_;
    /// An iterator on the set of adjacent edges.
    neighbors_t::const_reverse_iterator i_; 

    /// The psite corresponding to this iterator.
    psite psite_;
    /// The point corresponding to this iterator.
    /* FIXME: Dummy value.  To be removed as soon as the conversion
       from psite to point is no longer mandatory.  */
    point p_;
  };

  /* FIXME: This hand-made delegation is painful.  We should rely on
     the general mechanism provided by Point_Site.  But then again, we
     need to refine/adjust the interface of Point_Site w.r.t. the
     mandatory conversions to points.  */
  template <typename P, typename N>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const line_graph_neighborhood_bkd_piter<P, N>& p);



# ifndef MLN_INCLUDE_ONLY

  /*------------------------------------------.
  | line_graph_neighborhood_fwd_piter<P, N>.  |
  `------------------------------------------*/

  template <typename P, typename N>
  template <typename Pref>
  inline
  line_graph_neighborhood_fwd_piter<P, N>::line_graph_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
									     const Point_Site<Pref>& p_ref)
    : nbh_(exact(nbh)),
      p_ref_(exact(p_ref).to_psite()),
      // Initialize psite_ to a dummy value.
      psite_()
  {
    // Invalidate i_.
    invalidate();
  }

  template <typename P, typename N>
  inline
  bool
  line_graph_neighborhood_fwd_piter<P, N>::is_valid() const
  {
    return
      // The reference point must be valid...
      p_ref_.is_valid()
      // ...and must not have changed since the neighborhood has been
      // computed...
      && p_ref_ == saved_p_ref_
      // ...and the iterator i_ must point a valid value.
      && i_ != neighbors_.end();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_fwd_piter<P, N>::invalidate()
  {
    i_ = neighbors_.end();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_fwd_piter<P, N>::start()
  {
    mln_precondition(p_ref_.is_valid());
    // Update the neighbors, if needed.
    if (!saved_p_ref_.is_valid() || p_ref_ != saved_p_ref_)
      {
	saved_p_ref_ = p_ref_;
	nbh_.compute_neighbors_(*this);
      }
    i_ = neighbors_.begin();
    // FIXME: We might move the is_valid condition within update_.
    if (is_valid())
      update_();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_fwd_piter<P, N>::next_()
  {
    // Ensure the p_ref_ has not changed.
    mln_precondition(p_ref_ == saved_p_ref_);
    ++i_;
    // FIXME: We might move the is_valid condition within update_.
    if (is_valid())
      update_();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_fwd_piter<P, N>::update_()
  {
    // Update psite_.
    psite_ = line_graph_psite<P>(plg(), *i_);
  }

  template <typename P, typename N>
  inline
  const P&
  line_graph_neighborhood_fwd_piter<P, N>::to_point() const
  {
    return p_;
  }

  template <typename P, typename N>
  inline
  const line_graph_psite<P>&
  line_graph_neighborhood_fwd_piter<P, N>::to_psite() const
  {
    return psite_;
  }

  template <typename P, typename N>
  inline
  line_graph_neighborhood_fwd_piter<P, N>::operator line_graph_psite<P> () const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <typename P, typename N>
  inline
  const line_graph_psite<P>&
  line_graph_neighborhood_fwd_piter<P, N>::p_ref() const
  {
    return p_ref_;
  }

  template <typename P, typename N>
  inline
  const p_line_graph<P>&
  line_graph_neighborhood_fwd_piter<P, N>::plg() const
  {
    return p_ref_.plg();
  }

  template <typename P, typename N>
  inline
  std::set<util::edge_id>&
  line_graph_neighborhood_fwd_piter<P, N>::neighbors()
  {
    return neighbors_;
  }

  template <typename P, typename N>
  inline
  mln_coord(P)
  line_graph_neighborhood_fwd_piter<P, N>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }

  template <typename P, typename N>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const line_graph_neighborhood_fwd_piter<P, N>& p)
  {
    return ostr << p.to_psite();
  }


  /*------------------------------------------.
  | line_graph_neighborhood_bkd_piter<P, N>.  |
  `------------------------------------------*/

  template <typename P, typename N>
  template <typename Pref>
  inline
  line_graph_neighborhood_bkd_piter<P, N>::line_graph_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
									     const Point_Site<Pref>& p_ref)
    : nbh_(exact(nbh)),
      p_ref_(exact(p_ref).to_psite()),
      // Initialize psite_ to a dummy value.
      psite_()
  {
    // Invalidate i_.
    invalidate();
  }

  template <typename P, typename N>
  inline
  bool
  line_graph_neighborhood_bkd_piter<P, N>::is_valid() const
  {
    return
      // The reference point must be valid...
      p_ref_.is_valid()
      // ...and must not have changed since the neighborhood has been
      // computed...
      && p_ref_ == saved_p_ref_
      // ...and the iterator i_ must point a valid value.
      && i_ != neighbors_.rend();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_bkd_piter<P, N>::invalidate()
  {
    i_ = neighbors_.rend();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_bkd_piter<P, N>::start()
  {
    mln_precondition(p_ref_.is_valid());
    // Update the neighbors, if needed.
    if (!saved_p_ref_.is_valid() || p_ref_ != saved_p_ref_)
      {
	saved_p_ref_ = p_ref_;
	nbh_.compute_neighbors_(*this);
      }
    i_ = neighbors_.rbegin();
    // FIXME: We might move the is_valid condition within update_.
    if (is_valid())
      update_();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_bkd_piter<P, N>::next_()
  {
    // Ensure the p_ref_ has not changed.
    mln_precondition(p_ref_ == saved_p_ref_);
    ++i_;
    // FIXME: We might move the is_valid condition within update_.
    if (is_valid())
      update_();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_bkd_piter<P, N>::update_()
  {
    // Update psite_.
    psite_ = line_graph_psite<P>(plg(), *i_);
  }

  template <typename P, typename N>
  inline
  const P&
  line_graph_neighborhood_bkd_piter<P, N>::to_point() const
  {
    return p_;
  }

  template <typename P, typename N>
  inline
  const line_graph_psite<P>&
  line_graph_neighborhood_bkd_piter<P, N>::to_psite() const
  {
    return psite_;
  }

  template <typename P, typename N>
  inline
  line_graph_neighborhood_bkd_piter<P, N>::operator line_graph_psite<P> () const
  {
    mln_precondition(is_valid());
    return psite_;
  }

  template <typename P, typename N>
  inline
  const line_graph_psite<P>&
  line_graph_neighborhood_bkd_piter<P, N>::p_ref() const
  {
    return p_ref_;
  }

  template <typename P, typename N>
  inline
  const p_line_graph<P>&
  line_graph_neighborhood_bkd_piter<P, N>::plg() const
  {
    return p_ref_.plg();
  }

  template <typename P, typename N>
  inline
  std::set<util::edge_id>&
  line_graph_neighborhood_bkd_piter<P, N>::neighbors()
  {
    return neighbors_;
  }

  template <typename P, typename N>
  inline
  mln_coord(P)
  line_graph_neighborhood_bkd_piter<P, N>::operator[](unsigned i) const
  {
    assert(i < dim);
    return p_[i];
  }

  template <typename P, typename N>
  inline
  std::ostream&
  operator<<(std::ostream& ostr,
	     const line_graph_neighborhood_bkd_piter<P, N>& p)
  {
    return ostr << p.to_psite();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_LINE_GRAPH_NEIGHBORHOOD_PITER_HH
