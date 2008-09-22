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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_SITE_SET_P_LINE_GRAPH_PITER_HH
# define MLN_CORE_SITE_SET_P_LINE_GRAPH_PITER_HH

/// \file mln/core/site_set/p_line_graph_piter.hh
/// \brief Definition of point iterators on line graph-based site set.

# include <mln/core/internal/site_set_iterator_base.hh>
# include <mln/core/site_set/p_line_graph.hh>
# include <mln/core/image/line_graph_psite.hh>

/* FIXME: Iterators on p_graph and p_line_graph share common code.
   Factor as much as possible.  */


namespace mln
{

  // Forward declarations.
  template <typename P> class p_line_graph;
  template <typename P> class line_graph_psite;


  /*-----------------------------.
  | p_line_graph_fwd_piter_<P>.  |
  `-----------------------------*/

  /// \brief Forward iterator on point sites of a mln::p_line_graph<P>.
  template <typename P>
  class p_line_graph_fwd_piter_
    : public internal::site_set_iterator_base< p_line_graph<P>,
					       p_line_graph_fwd_piter_<P> >
  {
    typedef p_line_graph_fwd_piter_<P> self_;
    typedef internal::site_set_iterator_base< p_line_graph<P>, self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_line_graph_fwd_piter_();
    p_line_graph_fwd_piter_(const p_line_graph<P>& plg);
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

  protected:
    /// The psite corresponding to this iterator.
    using super_::p_;
  };


  /// Print an mln::p_line_graph_fwd_piter_<P>.
  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_line_graph_fwd_piter_<P>& p);


  /*-----------------------------.
  | p_line_graph_bkd_piter_<P>.  |
  `-----------------------------*/

  /// \brief Backward iterator on point sites of a mln::p_line_graph<P>.
  template <typename P>
  class p_line_graph_bkd_piter_
    : public internal::site_set_iterator_base< p_line_graph<P>,
					       p_line_graph_bkd_piter_<P> >
  {
    typedef p_line_graph_bkd_piter_<P> self_;
    typedef internal::site_set_iterator_base< p_line_graph<P>, self_ > super_;

  public:
    /// Construction and assignment.
    /// \{
    p_line_graph_bkd_piter_();
    p_line_graph_bkd_piter_(const p_line_graph<P>& plg);
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

  protected:
    /// The psite corresponding to this iterator.
    using super_::p_;
  };


  /// Print an mln::p_line_graph_bkd_piter_<P>.
  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_line_graph_bkd_piter_<P>& p);



# ifndef MLN_INCLUDE_ONLY

  /*-----------------------------.
  | p_line_graph_fwd_piter_<P>.  |
  `-----------------------------*/

  template <typename P>
  inline
  p_line_graph_fwd_piter_<P>::p_line_graph_fwd_piter_()
  {
    mln_postcondition(!this->is_valid());
  }

  template <typename P>
  inline
  p_line_graph_fwd_piter_<P>::p_line_graph_fwd_piter_(const p_line_graph<P>& plg)
  {
    this->change_target(plg);
    mln_postcondition(!this->is_valid());
  }

  template <typename P>
  inline
  bool
  p_line_graph_fwd_piter_<P>::is_valid_() const
  {
    return p_.is_valid();
  }

  template <typename P>
  inline
  void
  p_line_graph_fwd_piter_<P>::invalidate_()
  {
    p_.invalidate();
  }

  template <typename P>
  inline
  void
  p_line_graph_fwd_piter_<P>::start_()
  {
    p_.change_edge_id(0);
  }

  template <typename P>
  inline
  void
  p_line_graph_fwd_piter_<P>::next_()
  {
    p_.inc_edge_id();
  }


  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_line_graph_fwd_piter_<P>& p)
  {
    return ostr << p.unproxy_();
  }


  /*-----------------------------.
  | p_line_graph_bkd_piter_<P>.  |
  `-----------------------------*/

  template <typename P>
  inline
  p_line_graph_bkd_piter_<P>::p_line_graph_bkd_piter_()
  {
    mln_postcondition(!this->is_valid());
  }

  template <typename P>
  inline
  p_line_graph_bkd_piter_<P>::p_line_graph_bkd_piter_(const p_line_graph<P>& plg)
  {
    this->change_target(plg);
    mln_postcondition(!this->is_valid());
  }

  template <typename P>
  inline
  bool
  p_line_graph_bkd_piter_<P>::is_valid_() const
  {
    return p_.is_valid();
  }

  template <typename P>
  inline
  void
  p_line_graph_bkd_piter_<P>::invalidate_()
  {
    p_.change_edge_id(this->site_set().nedges());
  }

  template <typename P>
  inline
  void
  p_line_graph_bkd_piter_<P>::start_()
  {
    p_.change_edge_id(this->site_set().nedges() - 1);
  }

  template <typename P>
  inline
  void
  p_line_graph_bkd_piter_<P>::next_()
  {
    p_.dec_edge_id();
  }


  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const p_line_graph_bkd_piter_<P>& p)
  {
    return ostr << p.unproxy_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_CORE_SITE_SET_P_LINE_GRAPH_PITER_HH
