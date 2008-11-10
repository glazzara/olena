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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_SITE_SET_P_GRAPH_PITER_HH
# define MLN_CORE_SITE_SET_P_GRAPH_PITER_HH

/// \file mln/core/site_set/p_graph_piter.hh
/// \brief Definition of point iterator on graph-based point set.

# include <mln/core/internal/site_set_iterator_base.hh>
//# include <mln/core/site_set/p_graph.hh>
//# include <mln/core/image/graph_psite.hh>

namespace mln
{
  // Fwd decls.
  template <typename S, typename I> class graph_psite;


  /*------------------------.
  | p_graph_piter<S,I>.  |
  `------------------------*/

  /// \brief Generic iterator on point sites of a mln::S.
  template <typename S, typename I>
  class p_graph_piter
    : public internal::site_set_iterator_base< S,
					       p_graph_piter<S, I> >
  {
    typedef p_graph_piter<S,I> self_;
    typedef internal::site_set_iterator_base< S, self_ > super_;
    typedef I iter;

  public:
    /// Construction and assignment.
    /// \{
    p_graph_piter();
    p_graph_piter(const S& pv);
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
    /// \}

  private:
    /// Update the psite corresponding to this iterator.
    void update_();

  private:
    /// The psite corresponding to this iterator.
    using super_::p_;

    /// The underlying vertex iterator.
    iter iter_;
  };


  /// Print an mln::p_graph_piter<S,I>.
  template <typename S, typename I>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_graph_piter<S,I>& p);


# ifndef MLN_INCLUDE_ONLY

  /*---------------------.
  | p_graph_piter<S,I>.  |
  `---------------------*/

  template <typename S, typename I>
  inline
  p_graph_piter<S,I>::p_graph_piter()
  {
    mln_postcondition(!this->is_valid());
  }

  template <typename S, typename I>
  inline
  p_graph_piter<S,I>::p_graph_piter(const S& pv)
    : iter_(pv.graph())
  {
    this->change_target(pv);
    mln_postcondition(!this->is_valid());
  }

  template <typename S, typename I>
  inline
  bool
  p_graph_piter<S,I>::is_valid_() const
  {
    return iter_.is_valid();
  }

  template <typename S, typename I>
  inline
  void
  p_graph_piter<S,I>::invalidate_()
  {
    iter_.invalidate();
  }

  template <typename S, typename I>
  inline
  void
  p_graph_piter<S,I>::start_()
  {
    iter_.start();
    if (this->is_valid())
      update_();
  }

  template <typename S, typename I>
  inline
  void
  p_graph_piter<S,I>::next_()
  {
    iter_.next();
      if (this->is_valid())
	update_();
  }

  template <typename S, typename I>
  inline
  void
  p_graph_piter<S,I>::update_()
  {
    mln_precondition(this->is_valid());
    // Update psite_.
    p_.update_id(iter_.id());
  }



  template <typename S, typename I>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_graph_piter<S,I>& p)
  {
    return ostr << p.unproxy_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_SITE_SET_P_GRAPH_PITER_HH
