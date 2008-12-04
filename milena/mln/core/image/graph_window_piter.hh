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

#ifndef MLN_CORE_IMAGE_GRAPH_WINDOW_PITER_HH
# define MLN_CORE_IMAGE_GRAPH_WINDOW_PITER_HH

/// \file   mln/core/image/graph_window_piter.hh
///
/// Definition of a point iterator on a line_graph window.


# include <mln/core/internal/site_relative_iterator_base.hh>


namespace mln
{

  /// Forward iterator on line graph window.
  template <typename S, typename W, typename I>
  class graph_window_piter
    : public internal::site_relative_iterator_base< W,
					  graph_window_piter<S,W,I> >
  {
    typedef graph_window_piter<S,W,I> self_;
    typedef internal::site_relative_iterator_base<W,self_> super_;

  public:
    /// Associated types
    /// \{
    typedef mln_result(S::fun_t) P;
    enum { dim = P::dim };

    // FIXME: Dummy typedef.
    typedef void dpoint;
    // FIXME: Dummy value.
    typedef void mesh;
    /// \}

    /// Construction.
    /// \{
    graph_window_piter();
    template <typename Pref>
    graph_window_piter(const Window<W>& win,
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
    mln_psite(W) compute_p_() const;
    /// \}

  private:
    I iter_;
  };


# ifndef MLN_INCLUDE_ONLY


  template <typename S, typename W, typename I>
  inline
  graph_window_piter<S,W,I>::graph_window_piter()
  {
  }

  template <typename S, typename W, typename I>
  template <typename Pref>
  inline
  graph_window_piter<S,W,I>::graph_window_piter(const Window<W>& win,
						const Pref& p_ref)
     : iter_(p_ref.hook_iter_())
  {
    this->change_target(exact(win));
    center_at(p_ref);
  }

  template <typename S, typename W, typename I>
  inline
  bool
  graph_window_piter<S,W,I>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_piter<S,W,I>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_piter<S,W,I>::do_start_()
  {
    iter_.start();
  }

  template <typename S, typename W, typename I>
  inline
  void
  graph_window_piter<S,W,I>::do_next_()
  {
    iter_.next();
  }

  template <typename S, typename W, typename I>
  inline
  mln_psite(W)
  graph_window_piter<S,W,I>::compute_p_() const
  {
    return mln_psite(S)(this->center().site_set(), iter_.id());
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_GRAPH_WINDOW_PITER_HH
