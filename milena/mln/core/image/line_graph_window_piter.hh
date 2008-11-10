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

#ifndef MLN_CORE_IMAGE_LINE_GRAPH_WINDOW_PITER_HH
# define MLN_CORE_IMAGE_LINE_GRAPH_WINDOW_PITER_HH

/// \file   mln/core/image/line_graph_window_piter.hh
/// \brief  Definition of a point iterator on a line_graph window.

# include <mln/core/internal/line_graph_vicinity_piter.hh>

/* FIXME: Due to the poor interface of mln::p_line_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{

  /*------------------------------------.
  | line_graph_window_fwd_piter<P, W>.  |
  `------------------------------------*/

  /// \brief Forward iterator on line graph window.
  template <typename P, typename W>
  class line_graph_window_fwd_piter
    : public internal::line_graph_vicinity_piter_<P, W,
						  line_graph_window_fwd_piter<P, W> >
  {
    typedef line_graph_window_fwd_piter<P, W> self_;
    typedef internal::line_graph_vicinity_piter_<P, W, self_> super_;

  public:
    /// The Point_Site type.
    typedef mln_psite(W) psite;

  public:
    /// Construction.
    /// \{
    line_graph_window_fwd_piter();
    template <typename Pref>
    line_graph_window_fwd_piter(const Window<W>& win,
				const Pref& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void do_start_();
    /// Go to the next point.
    void do_next_();

    /// Compute the current psite.
    psite compute_p_() const;
    /// \}

  private:
    /// An iterator on the set of adjacent edges.
    typename super_::sites_t::const_iterator i_; 
  };


  /*------------------------------------.
  | line_graph_window_bkd_piter<P, W>.  |
  `------------------------------------*/

  /// \brief Backward iterator on line graph window.
  template <typename P, typename W>
  class line_graph_window_bkd_piter
    : public internal::line_graph_vicinity_piter_<P, W, line_graph_window_bkd_piter<P, W> >
  {
    typedef line_graph_window_bkd_piter<P, W> self_;
    typedef internal::line_graph_vicinity_piter_<P, W, self_> super_;

  public:
    /// The Point_Site type.
    typedef mln_psite(W) psite;

  public:
    /// Construction.
    /// \{
    line_graph_window_bkd_piter();
    template <typename Pref>
    line_graph_window_bkd_piter(const Window<W>& win,
				const Pref& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();

    /// Start an iteration.
    void do_start_();
    /// Go to the next point.
    void do_next_();

    /// Compute the current psite.
    psite compute_p_() const;
    /// \}

  private:
    /// An iterator on the set of adjacent edges.
    typename super_::sites_t::const_reverse_iterator i_; 
  };



# ifndef MLN_INCLUDE_ONLY

  /*------------------------------------.
  | line_graph_window_fwd_piter<P, W>.  |
  `------------------------------------*/

  template <typename P, typename W>
  inline
  line_graph_window_fwd_piter<P, W>::line_graph_window_fwd_piter()
  {
  }

  template <typename P, typename W>
  template <typename Pref>
  inline
  line_graph_window_fwd_piter<P, W>::line_graph_window_fwd_piter(const Window<W>& win,
								 const Pref& p_ref)
     : super_(p_ref)
  {
    this->change_target(exact(win));
  }

  template <typename P, typename W>
  inline
  bool
  line_graph_window_fwd_piter<P, W>::is_valid_() const
  {
    return i_ != this->sites_.end();
  }

  template <typename P, typename W>
  inline
  void
  line_graph_window_fwd_piter<P, W>::invalidate_()
  {
    i_ = this->sites_.end();
  }

  template <typename P, typename W>
  inline
  void
  line_graph_window_fwd_piter<P, W>::do_start_()
  {
    this->site_set().compute_sites_(*this);
    i_ = this->sites_.begin();
  }

  template <typename P, typename W>
  inline
  void
  line_graph_window_fwd_piter<P, W>::do_next_()
  {
    ++i_;
  }

  template <typename P, typename W>
  inline
  mln_psite(W)
  line_graph_window_fwd_piter<P, W>::compute_p_() const
  {
    return line_graph_psite<P>(this->center().site_set(), *i_);
  }


  /*------------------------------------.
  | line_graph_window_bkd_piter<P, W>.  |
  `------------------------------------*/

  template <typename P, typename W>
  inline
  line_graph_window_bkd_piter<P, W>::line_graph_window_bkd_piter()
  {
  }

  template <typename P, typename W>
  template <typename Pref>
  inline
  line_graph_window_bkd_piter<P, W>::line_graph_window_bkd_piter(const Window<W>& win,
								 const Pref& p_ref)
    : super_(p_ref)
  {
    this->change_target(exact(win));
  }

  template <typename P, typename W>
  inline
  bool
  line_graph_window_bkd_piter<P, W>::is_valid_() const
  {
    return i_ != this->sites_.rend();
  }

  template <typename P, typename W>
  inline
  void
  line_graph_window_bkd_piter<P, W>::invalidate_()
  {
    i_ = this->sites_.rend();
  }

  template <typename P, typename W>
  inline
  void
  line_graph_window_bkd_piter<P, W>::do_start_()
  {
    this->site_set().compute_sites_(*this);
    i_ = this->sites_.rbegin();
  }

  template <typename P, typename W>
  inline
  void
  line_graph_window_bkd_piter<P, W>::do_next_()
  {
    ++i_;
  }

  template <typename P, typename W>
  inline
  mln_psite(W)
  line_graph_window_bkd_piter<P, W>::compute_p_() const
  {
    return line_graph_psite<P>(this->center().site_set(), *i_);
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_LINE_GRAPH_WINDOW_PITER_HH
