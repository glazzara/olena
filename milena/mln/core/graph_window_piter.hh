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

# include <mln/core/internal/graph_vicinity_piter.hh>

/* FIXME: Due to the poor interface of mln::p_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{

  /*-------------------------------.
  | graph_window_fwd_piter<P, W>.  |
  `-------------------------------*/

  /// \brief Forward iterator on graph window.
  template <typename P, typename W>
  class graph_window_fwd_piter :
    public internal::graph_vicinity_piter_< P, graph_window_fwd_piter<P, W> >
  {
    typedef graph_window_fwd_piter<P, W> self_;
    typedef internal::graph_vicinity_piter_<P, self_> super_;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    graph_window_fwd_piter(const Window<W>& win, const Point_Site<Pref>& p_ref);
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
    /// Update the internal data of the iterator.
    void update_();
    /// \}

  private:
    /// The window.
    const W& win_;

    /// An iterator on the set of adjacent vertices.
    typename super_::sites_t::const_iterator i_; 
  };


  /*-------------------------------.
  | graph_window_bkd_piter<P, W>.  |
  `-------------------------------*/

  /// \brief Backward iterator on graph window.
  template <typename P, typename W>
  class graph_window_bkd_piter :
    public internal::graph_vicinity_piter_< P, graph_window_bkd_piter<P, W> >
  {
    typedef graph_window_bkd_piter<P, W> self_;
    typedef internal::graph_vicinity_piter_<P, self_> super_;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    graph_window_bkd_piter(const Window<W>& win, const Point_Site<Pref>& p_ref);
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
    /// Update the internal data of the iterator.
    void update_();
    /// \}

  private:
    /// The window.
    const W& win_;

    /// An iterator on the set of adjacent vertices.
    typename super_::sites_t::const_reverse_iterator i_; 
  };



# ifndef MLN_INCLUDE_ONLY

  /*-------------------------------.
  | graph_window_fwd_piter<P, W>.  |
  `-------------------------------*/

  template <typename P, typename W>
  template <typename Pref>
  inline
  graph_window_fwd_piter<P, W>::graph_window_fwd_piter(const Window<W>& win,
						       const Point_Site<Pref>& p_ref)
    : super_(p_ref),
      win_(exact(win))
  {
    // Invalidate i_.
    invalidate();
  }

  template <typename P, typename W>
  inline
  bool
  graph_window_fwd_piter<P, W>::is_valid() const
  {
    return
      // The reference point must be valid...
      this->p_ref_.is_valid()
      // ...and must not have changed since the window has been computed...
      && this->p_ref_ == this->saved_p_ref_
      // ...and the iterator i_ must point a valid value.
      && i_ != this->sites_.end();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::invalidate()
  {
    i_ = this->sites_.end();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::start()
  {
    mln_precondition(this->p_ref_.is_valid());
    // Update the sites, if needed.
    if (!this->saved_p_ref_.is_valid() || this->p_ref_ != this->saved_p_ref_)
      {
	this->saved_p_ref_ = this->p_ref_;
	win_.compute_sites_(*this);
      }
    i_ = this->sites_.begin();
    // FIXME: We might move the is_valid condition within update_.
    if (is_valid())
      update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::next_()
  {
    // Ensure the p_ref_ has not changed.
    mln_precondition(this->p_ref_ == this->saved_p_ref_);
    ++i_;
    // FIXME: We might move the is_valid condition within update_.
    if (is_valid())
      update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::update_()
  {
    // Update psite_.
    this->psite_ = graph_psite<P>(this->pg(), *i_);
  }


  /*-------------------------------.
  | graph_window_bkd_piter<P, W>.  |
  `-------------------------------*/

  template <typename P, typename W>
  template <typename Pref>
  inline
  graph_window_bkd_piter<P, W>::graph_window_bkd_piter(const Window<W>& win,
						       const Point_Site<Pref>& p_ref)
    : super_(p_ref),
      win_(exact(win))
  {
    // Invalidate i_.
    invalidate();
  }

  template <typename P, typename W>
  inline
  bool
  graph_window_bkd_piter<P, W>::is_valid() const
  {
    return
      // The reference point must be valid...
      this->p_ref_.is_valid()
      // ...and must not have changed since the window has been computed...
      && this->p_ref_ == this->saved_p_ref_
      // ...and the iterator i_ must point a valid value.
      && i_ != this->sites_.rend();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::invalidate()
  {
    i_ = this->sites_.rend();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::start()
  {
    mln_precondition(this->p_ref_.is_valid());
    // Update the sites, if needed.
    if (!this->saved_p_ref_.is_valid() || this->p_ref_ != this->saved_p_ref_)
      {
	this->saved_p_ref_ = this->p_ref_;
	win_.compute_sites_(*this);
      }
    i_ = this->sites_.rbegin();
    // FIXME: We might move the is_valid condition within update_.
    if (is_valid())
      update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::next_()
  {
    // Ensure the p_ref_ has not changed.
    mln_precondition(this->p_ref_ == this->saved_p_ref_);
    ++i_;
    // FIXME: We might move the is_valid condition within update_.
    if (is_valid())
      update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::update_()
  {
    // Update psite_.
    this->psite_ = graph_psite<P>(this->pg(), *i_);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_GRAPH_WINDOW_PITER_HH
