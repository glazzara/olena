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

# include <mln/core/internal/graph_vicinity_piter.hh>

/* FIXME: Due to the poor interface of mln::p_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{

  /*-------------------------------------.
  | graph_neighborhood_fwd_piter<P, N>.  |
  `-------------------------------------*/

  template <typename P, typename N>
  class graph_neighborhood_fwd_piter :
    public internal::graph_vicinity_piter_< P,
					    graph_neighborhood_fwd_piter<P, N> >
  {
    typedef graph_neighborhood_fwd_piter<P, N> self_;
    typedef internal::graph_vicinity_piter_<P, self_> super_;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    graph_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
				 const Point_Site<Pref>& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next point.
    void next_();
    /// \}

    /// Internals, used by the neighborhood.
    /// \{
    /// Set the iterator to the first site of the graph.
    void first_();
    /// Advance the position of the iterator by one step.
    void step_();
    /// \}

    private:
      /// The neighborhood.
      const N& nbh_;
  };


  /*-------------------------------------.
  | graph_neighborhood_bkd_piter<P, N>.  |
  `-------------------------------------*/

  template <typename P, typename N>
  class graph_neighborhood_bkd_piter :
    public internal::graph_vicinity_piter_< P,
					    graph_neighborhood_bkd_piter<P, N> >
  {
    typedef graph_neighborhood_bkd_piter<P, N> self_;
    typedef internal::graph_vicinity_piter_<P, self_> super_;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    graph_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
				 const Point_Site<Pref>& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next point.
    void next_();
    /// \}

    /// Internals, used by the neighborhood.
    /// \{
    /// Set the iterator to the first site of the graph.
    void first_();
    /// Advance the position of the iterator by one step.
    void step_();
    /// \}

    private:
      /// The neighborhood.
      const N& nbh_;
  };



# ifndef MLN_INCLUDE_ONLY

  /*-------------------------------------.
  | graph_neighborhood_fwd_piter<P, N>.  |
  `-------------------------------------*/

  template <typename P, typename N>
  template <typename Pref>
  inline
  graph_neighborhood_fwd_piter<P, N>::graph_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
								   const Point_Site<Pref>& p_ref)
    : super_(p_ref),
      nbh_(exact(nbh))
  {
  }

  template <typename P, typename N>
  inline
  void
  graph_neighborhood_fwd_piter<P, N>::start()
  {
    this->nbh_.start(*this);
    if (this->is_valid())
      this->update_();
  }

  template <typename P, typename N>
  inline
  void
  graph_neighborhood_fwd_piter<P, N>::next_()
  {
    this->nbh_.next_(*this);
    if (this->is_valid())
      this->update_();
  }

  template <typename P, typename N>
  inline
  void
  graph_neighborhood_fwd_piter<P, N>::first_()
  {
    this->id_ = 0;
  }

  template <typename P, typename N>
  inline
  void
  graph_neighborhood_fwd_piter<P, N>::step_()
  {
    ++this->id_;
  }


  /*-------------------------------------.
  | graph_neighborhood_bkd_piter<P, N>.  |
  `-------------------------------------*/

  template <typename P, typename N>
  template <typename Pref>
  inline
  graph_neighborhood_bkd_piter<P, N>::graph_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
								   const Point_Site<Pref>& p_ref)
    : super_(p_ref),
      nbh_(exact(nbh))
  {
  }

  template <typename P, typename N>
  inline
  void
  graph_neighborhood_bkd_piter<P, N>::start()
  {
    this->nbh_.start(*this);
    if (this->is_valid())
      this->update_();
  }

  template <typename P, typename N>
  inline
  void
  graph_neighborhood_bkd_piter<P, N>::next_()
  {
    this->nbh_.next_(*this);
    if (this->is_valid())
      this->update_();
  }

  template <typename P, typename N>
  inline
  void
  graph_neighborhood_bkd_piter<P, N>::first_()
  {
    this->id_ = this->p_ref_.pg().gr_->nnodes() - 1;
  }

  template <typename P, typename N>
  inline
  void
  graph_neighborhood_bkd_piter<P, N>::step_()
  {
    --this->id_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_GRAPH_NEIGHBORHOOD_PITER_HH
