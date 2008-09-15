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

#ifndef MLN_CORE_IMAGE_LINE_GRAPH_NEIGHBORHOOD_PITER_HH
# define MLN_CORE_IMAGE_LINE_GRAPH_NEIGHBORHOOD_PITER_HH

/// \file   mln/core/image/line_graph_neighborhood_piter.hh
/// \brief  Definition of a point iterator on a line graph neighborhood.

# include <mln/core/internal/line_graph_vicinity_piter.hh>

/* FIXME: Due to the poor interface of mln::p_line_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{

  /*------------------------------------------.
  | line_graph_neighborhood_fwd_piter<P, N>.  |
  `------------------------------------------*/

  /// \brief Forward iterator on line graph neighborhood.
  template <typename P, typename N>
  class line_graph_neighborhood_fwd_piter :
    public internal::line_graph_vicinity_piter_< P, N,
						 line_graph_neighborhood_fwd_piter<P, N> >
  {
    typedef line_graph_neighborhood_fwd_piter<P, N> self_;
    typedef internal::line_graph_vicinity_piter_<P, N, self_> super_;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    line_graph_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
				      const Pref& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();
    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();
    /// Update the internal data of the iterator.
    void update_();
    /// \}

    /// Conversions.
    /// \{
    /// The corresponding site type.
    typedef P site;
    // FIXME: Dummy.
    site site_;
    // FIXME: Dummy.
    const site& to_site() const;
    // FIXME: Dummy.
    operator site() const;
    /// \}

    /// Give the target neighborhood address.
    const N*& target_();
    /// Change the target neighborhood.
    void change_target(const N& nbh);

  private:
    /// The neighborhood.
    const N* nbh_;

    /// An iterator on the set of adjacent edges.
    typename super_::sites_t::const_iterator i_;
  };


  /*------------------------------------------.
  | line_graph_neighborhood_bkd_piter<P, N>.  |
  `------------------------------------------*/

  /// \brief Backward iterator on line graph neighborhood.
  template <typename P, typename N>
  class line_graph_neighborhood_bkd_piter :
    public internal::line_graph_vicinity_piter_< P, N,
						 line_graph_neighborhood_bkd_piter<P, N> >
  {
    typedef line_graph_neighborhood_bkd_piter<P, N> self_;
    typedef internal::line_graph_vicinity_piter_<P, N, self_> super_;

  public:
    /// Construction.
    /// \{
    template <typename Pref>
    line_graph_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
				      const Pref& p_ref);
    /// \}

    /// Manipulation.
    /// \{
    /// Test if the iterator is valid.
    bool is_valid_() const;
    /// Invalidate the iterator.
    void invalidate_();
    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();
    /// Update the internal data of the iterator.
    void update_();
    /// \}

    /// Conversions.
    /// \{
    /// The corresponding site type.
    typedef P site;
    // FIXME: Dummy.
    site site_;
    // FIXME: Dummy.
    const site& to_site() const;
    // FIXME: Dummy.
    operator site() const;
    /// \}

    /// Give the target neighborhood address.
    const N*& target_();
    /// Change the target neighborhood.
    void change_target(const N& nbh);

  private:
    /// The neighborhood.
    const N* nbh_;

    /// An iterator on the set of adjacent edges.
    typename super_::sites_t::const_reverse_iterator i_;
  };



# ifndef MLN_INCLUDE_ONLY

  /*------------------------------------------.
  | line_graph_neighborhood_fwd_piter<P, N>.  |
  `------------------------------------------*/

  template <typename P, typename N>
  template <typename Pref>
  inline
  line_graph_neighborhood_fwd_piter<P, N>::line_graph_neighborhood_fwd_piter(const Neighborhood<N>& nbh,
									     const Pref& p_ref)
    : super_(p_ref),
      nbh_(&exact(nbh))
  {
    // Invalidate i_.
    this->invalidate();
  }

  template <typename P, typename N>
  inline
  bool
  line_graph_neighborhood_fwd_piter<P, N>::is_valid_() const
  {
    return
      // The reference point must be valid...
      this->p_ref_.is_valid()
      // ...and must not have changed since the neighborhood has been
      // computed...
      && this->p_ref_ == this->saved_p_ref_
      // ...and the iterator i_ must point a valid value.
      && i_ != this->sites_.end();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_fwd_piter<P, N>::invalidate_()
  {
    i_ = this->sites_.end();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_fwd_piter<P, N>::start_()
  {
    mln_precondition(this->p_ref_.is_valid());
    // Update the neighbors, if needed.
    if (!this->saved_p_ref_.is_valid() || this->p_ref_ != this->saved_p_ref_)
      {
	this->saved_p_ref_ = this->p_ref_;
	nbh_->compute_sites_(*this);
      }
    i_ = this->sites_.begin();
    // FIXME: We might move the is_valid_ condition within update_.
    if (this->is_valid())
      update_();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_fwd_piter<P, N>::next_()
  {
    // Ensure the p_ref_ has not changed.
    mln_precondition(this->p_ref_ == this->saved_p_ref_);
    ++i_;
    // FIXME: We might move the is_valid_ condition within update_.
    if (this->is_valid())
      update_();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_fwd_piter<P, N>::update_()
  {
    // Update psite_.
    this->psite_ = line_graph_psite<P>(this->plg(), *i_);
  }


  template <typename P, typename N>
  inline
  const P&
  line_graph_neighborhood_fwd_piter<P, N>::to_site() const
  {
    // FIXME: Dummy.
    abort();
    return site_;
  }

  template <typename P, typename N>
  inline
  line_graph_neighborhood_fwd_piter<P, N>::operator P() const
  {
    // FIXME: Dummy.
    abort();
    return site_;
  }


  template <typename P, typename N>
  inline
  const N*&
  line_graph_neighborhood_fwd_piter<P, N>::target_()
  {
    return nbh_;
  }

  template <typename P, typename N>
  inline
  void line_graph_neighborhood_fwd_piter<P, N>::change_target(const N& nbh)
  {
    nbh_ = &nbh;
    this->invalidate();
  }



  /*------------------------------------------.
  | line_graph_neighborhood_bkd_piter<P, N>.  |
  `------------------------------------------*/

  template <typename P, typename N>
  template <typename Pref>
  inline
  line_graph_neighborhood_bkd_piter<P, N>::line_graph_neighborhood_bkd_piter(const Neighborhood<N>& nbh,
									     const Pref& p_ref)
    : super_(p_ref),
      nbh_(&exact(nbh))
  {
    // Invalidate i_.
    this->invalidate();
  }

  template <typename P, typename N>
  inline
  bool
  line_graph_neighborhood_bkd_piter<P, N>::is_valid_() const
  {
    return
      // The reference point must be valid...
      this->p_ref_.is_valid()
      // ...and must not have changed since the neighborhood has been
      // computed...
      && this->p_ref_ == this->saved_p_ref_
      // ...and the iterator i_ must point a valid value.
      && i_ != this->sites_.rend();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_bkd_piter<P, N>::invalidate_()
  {
    i_ = this->sites_.rend();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_bkd_piter<P, N>::start_()
  {
    mln_precondition(this->p_ref_.is_valid());
    // Update the neighbors, if needed.
    if (!this->saved_p_ref_.is_valid() || this->p_ref_ != this->saved_p_ref_)
      {
	this->saved_p_ref_ = this->p_ref_;
	nbh_->compute_sites_(*this);
      }
    i_ = this->sites_.rbegin();
    // FIXME: We might move the is_valid_ condition within update_.
    if (this->is_valid())
      update_();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_bkd_piter<P, N>::next_()
  {
    // Ensure the p_ref_ has not changed.
    mln_precondition(this->p_ref_ == this->saved_p_ref_);
    ++i_;
    // FIXME: We might move the is_valid_ condition within update_.
    if (this->is_valid())
      update_();
  }

  template <typename P, typename N>
  inline
  void
  line_graph_neighborhood_bkd_piter<P, N>::update_()
  {
    // Update psite_.
    this->psite_ = line_graph_psite<P>(this->plg(), *i_);
  }


  template <typename P, typename N>
  inline
  const P&
  line_graph_neighborhood_bkd_piter<P, N>::to_site() const
  {
    // FIXME: Dummy.
    abort();
    return site_;
  }

  template <typename P, typename N>
  inline
  line_graph_neighborhood_bkd_piter<P, N>::operator P() const
  {
    // FIXME: Dummy.
    abort();
    return site_;
  }


  template <typename P, typename N>
  inline
  const N*&
  line_graph_neighborhood_bkd_piter<P, N>::target_()
  {
    return nbh_;
  }

  template <typename P, typename N>
  inline
  void line_graph_neighborhood_bkd_piter<P, N>::change_target(const N& nbh)
  {
    nbh_ = &nbh;
    this->invalidate();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_LINE_GRAPH_NEIGHBORHOOD_PITER_HH
