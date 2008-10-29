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

#ifndef MLN_CORE_IMAGE_GRAPH_NEIGHBORHOOD_PITER_HH
# define MLN_CORE_IMAGE_GRAPH_NEIGHBORHOOD_PITER_HH

/// \file   mln/core/image/graph_neighborhood_piter.hh
/// \brief  Definition of a point iterator on a graph neighborhood.

# include <mln/core/internal/graph_relative_piter.hh>

namespace mln
{

  /*----------------------------------------.
  | graph_neighborhood_fwd_piter<G, F, N>.  |
  `----------------------------------------*/

  /// \p G Graph type.
  /// \p F function i2p used in the p_vertices<G, F>.
  /// \p N Type of the neighborhood.
  template <typename G, typename F, typename N>
  class graph_neighborhood_fwd_piter :
    public internal::graph_relative_piter< mln_result(F), N,
					    graph_neighborhood_fwd_piter<G, F, N> >
  {
    typedef graph_neighborhood_fwd_piter<G, F, N> self_;
    typedef internal::graph_relative_piter<mln_result(F), N, self_> super_;

  public:
    /// The Point_Site type.
    typedef mln_psite(N) psite;

  public:
    /// Construction.
    /// \{
    graph_neighborhood_fwd_piter();
    template <typename Pref>
    graph_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
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


  /*----------------------------------------.
  | graph_neighborhood_bkd_piter<G, F, N>.  |
  `----------------------------------------*/

  template <typename G, typename F, typename N>
  class graph_neighborhood_bkd_piter :
    public internal::graph_relative_piter< mln_result(F), N,
					    graph_neighborhood_bkd_piter<G, F, N> >
  {
    typedef graph_neighborhood_bkd_piter<G, F, N> self_;
    typedef internal::graph_relative_piter<mln_result(F), N, self_> super_;

    /// The Point_Site type.
    typedef mln_psite(N) psite;

  public:
    /// Construction.
    /// \{
    graph_neighborhood_bkd_piter();
    template <typename Pref>
    graph_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
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

  /*----------------------------------------.
  | graph_neighborhood_fwd_piter<G, F, N>.  |
  `----------------------------------------*/

  template <typename G, typename F, typename N>
  inline
  graph_neighborhood_fwd_piter<G, F, N>::graph_neighborhood_fwd_piter()
  {
  }

  template <typename G, typename F, typename N>
  template <typename Pref>
  inline
  graph_neighborhood_fwd_piter<G, F, N>::graph_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
								   const Pref& p_ref)
    : super_(p_ref)
  {
    this->change_target(exact(nbh));
  }

  template <typename G, typename F, typename N>
  inline
  bool
  graph_neighborhood_fwd_piter<G, F, N>::is_valid_() const
  {
    return i_ != this->sites_.end();
  }

  template <typename G, typename F, typename N>
  inline
  void
  graph_neighborhood_fwd_piter<G, F, N>::invalidate_()
  {
    i_ = this->sites_.end();
  }

  template <typename G, typename F, typename N>
  inline
  void
  graph_neighborhood_fwd_piter<G, F, N>::do_start_()
  {
    this->site_set().compute_sites_(*this);
    i_ = this->sites_.begin();
  }

  template <typename G, typename F, typename N>
  inline
  void
  graph_neighborhood_fwd_piter<G, F, N>::do_next_()
  {
    ++i_;
  }

  template <typename G, typename F, typename N>
  inline
  mln_psite(N)
  graph_neighborhood_fwd_piter<G, F, N>::compute_p_() const
  {
    return internal::vertex_psite<G, F>(this->center().site_set(), *i_);
  }


  /*----------------------------------------.
  | graph_neighborhood_bkd_piter<G, F, N>.  |
  `----------------------------------------*/

  template <typename G, typename F, typename N>
  inline
  graph_neighborhood_bkd_piter<G, F, N>::graph_neighborhood_bkd_piter()
  {
  }

  template <typename G, typename F, typename N>
  template <typename Pref>
  inline
  graph_neighborhood_bkd_piter<G, F, N>::graph_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
								   const Pref& p_ref)
    : super_(p_ref)
  {
    this->change_target(exact(nbh));
  }

  template <typename G, typename F, typename N>
  inline
  bool
  graph_neighborhood_bkd_piter<G, F, N>::is_valid_() const
  {
    return i_ != this->sites_.rend();
  }

  template <typename G, typename F, typename N>
  inline
  void
  graph_neighborhood_bkd_piter<G, F, N>::invalidate_()
  {
    i_ = this->sites_.rend();
  }

  template <typename G, typename F, typename N>
  inline
  void
  graph_neighborhood_bkd_piter<G, F, N>::do_start_()
  {
    this->site_set().compute_sites_(*this);
    i_ = this->sites_.rbegin();
  }

  template <typename G, typename F, typename N>
  inline
  void
  graph_neighborhood_bkd_piter<G, F, N>::do_next_()
  {
    ++i_;
  }

  template <typename G, typename F, typename N>
  inline
  mln_psite(N)
  graph_neighborhood_bkd_piter<G, F, N>::compute_p_() const
  {
    return internal::vertex_psite<G, F>(this->center().site_set(), *i_);
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_GRAPH_NEIGHBORHOOD_PITER_HH
