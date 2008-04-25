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

# include <mln/core/internal/graph_vicinity_piter.hh>

/* FIXME: Due to the poor interface of mln::p_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{

  /*-----------------------------------------.
  | internal::graph_window_piter_<P, W, E>.  |
  `-----------------------------------------*/

  namespace internal
  {

    /// \brief Base for iterator on a graph window.
    template <typename P, typename W, typename E>
    class graph_window_piter_ : public graph_vicinity_piter_<P, E>
    {
      typedef graph_window_piter_<P, W, E> self_;
      typedef graph_vicinity_piter_<P, E> super_;

    public:
      /// Construction.
      /// \{
      template <typename Pref>
      graph_window_piter_(const Window<W>& win, const Point_Site<Pref>& p_ref);
      /// \}

    protected:
      /// The window.
      const W& win_;
    };

  } // end of namespace mln::internal


  /*-------------------------------.
  | graph_window_fwd_piter<P, W>.  |
  `-------------------------------*/

  /// \brief Forward iterator on graph window.
  template <typename P, typename W>
  class graph_window_fwd_piter :
    public internal::graph_window_piter_< P, W, graph_window_fwd_piter<P, W> >
  {
    typedef graph_window_fwd_piter<P, W> self_;
    typedef internal::graph_window_piter_<P, W, self_> super_;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    graph_window_fwd_piter(const Window<W>& win, const Point_Site<Pref>& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next point.
    void next_();
    /// \}

    /// Internals, used by the window.
    /// \{
    /// Set the iterator to the first site of the graph.
    void first_();
    /// Advance the position of the iterator by one step.
    void step_();
    /// \}
  };


  /*-------------------------------.
  | graph_window_bkd_piter<P, W>.  |
  `-------------------------------*/

  /// \brief Backward iterator on graph window.
  template <typename P, typename W>
  class graph_window_bkd_piter :
    public internal::graph_window_piter_< P, W, graph_window_bkd_piter<P, W> >
  {
    typedef graph_window_bkd_piter<P, W> self_;
    typedef internal::graph_window_piter_<P, W, self_> super_;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    graph_window_bkd_piter(const Window<W>& win, const Point_Site<Pref>& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next point.
    void next_();
    /// \}

    /// Internals, used by the window.
    /// \{
    /// Set the iterator to the first site of the graph.
    void first_();
    /// Advance the position of the iterator by one step.
    void step_();
    /// \}
  };



# ifndef MLN_INCLUDE_ONLY

  /*-----------------------------------------.
  | internal::graph_window_piter_<P, W, E>.  |
  `-----------------------------------------*/

  namespace internal
  {

    template <typename P, typename W, typename E>
    template <typename Pref>
    inline
    graph_window_piter_<P, W, E>::graph_window_piter_(const Window<W>& win,
						      const Point_Site<Pref>& p_ref)
      : super_(p_ref),
	win_(exact(win))
    {
    }

  } // end of namespace mln::internal


  /*-------------------------------.
  | graph_window_fwd_piter<P, W>.  |
  `-------------------------------*/

  template <typename P, typename W>
  template <typename Pref>
  inline
  graph_window_fwd_piter<P, W>::graph_window_fwd_piter(const Window<W>& win,
						       const Point_Site<Pref>& p_ref)
    : super_(win, p_ref)
  {
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::start()
  {
    this->win_.start(*this);
    if (this->is_valid())
      this->update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::next_()
  {
    this->win_.next_(*this);
    if (this->is_valid())
      this->update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::first_()
  {
    this->id_ = 0;
  }

  template <typename P, typename W>
  inline
  void
  graph_window_fwd_piter<P, W>::step_()
  {
    ++this->id_;
  }


  /*-------------------------------.
  | graph_window_bkd_piter<P, W>.  |
  `-------------------------------*/

  template <typename P, typename W>
  template <typename Pref>
  inline
  graph_window_bkd_piter<P, W>::graph_window_bkd_piter(const Window<W>& win,
						       const Point_Site<Pref>& p_ref)
    : super_(win, p_ref)
  {
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::start()
  {
    this->win_.start(*this);
    if (this->is_valid())
      this->update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::next_()
  {
    this->win_.next_(*this);
    if (this->is_valid())
      this->update_();
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::first_()
  {
    this->id_ = this->p_ref_.pg().gr_->nnodes() - 1;
  }

  template <typename P, typename W>
  inline
  void
  graph_window_bkd_piter<P, W>::step_()
  {
    --this->id_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_GRAPH_WINDOW_PITER_HH
