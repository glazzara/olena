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

#ifndef MLN_CORE_IMAGE_LINE_GRAPH_PSITE_HH
# define MLN_CORE_IMAGE_LINE_GRAPH_PSITE_HH

/// \file mln/core/image/line_graph_psite.hh
/// \brief Definition of a line graph-based point site.

# include <mln/core/internal/pseudo_site_base.hh>

# include <mln/util/site_pair.hh>
# include <mln/core/site_set/p_line_graph.hh>

/* FIXME: This class shares a lot with graph_psite.  Factor as much as
   possible.  */


namespace mln
{

  // Fwd decl.
  template <typename P> class p_line_graph;
  template <typename P> class line_graph_psite;

  // FIXME: Rename as line_graph_psite as p_line_graph_psite.

  /// \brief Point site associated to a mln::line_graph_image.
  ///
  /// \arg \p P The type of the site.
  template <typename P>
  class line_graph_psite
    : public internal::pseudo_site_base_< const site_pair<P>&,
					  line_graph_psite<P> >
  {
    typedef line_graph_psite<P> self_;
    typedef internal::pseudo_site_base_< P, self_ > super_;

  public:
    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef p_line_graph<P> target;

    /// Construction and assignment.
    /// \{
    line_graph_psite();
    line_graph_psite(const p_line_graph<P>& plg, util::edge_id id);
    /// \}

    /// Psite manipulators.
    /// \{
    /// Is this psite valid?
    bool is_valid() const;
    /// Invalidate this psite.
    void invalidate();
    /// \}

    /// Site set manipulators.
    /// \{
    /// \brief Get the site set (shortcut for *target()).
    /// \pre Member plg_ is non null.
    const target& site_set() const;

    /// Get a pointer to the target site_set.
    const target* target_() const;
    /// Set the target site_set.
    void change_target(const target& new_target);
    /// \}

    /// Proxy manipulators.
    /// \{
    /// Return the site corresponding to this psite.
    const site_pair<P>& subj_();
    /// \}

    /// Edge id manipulators.
    //// \{
    /// Return the edge id of this point site.
    util::edge_id edge_id() const;
    /// Set the edge id of this point site.
    void change_edge_id(const util::edge_id& id);
    /// Increment the edge id of this point site.
    void inc_edge_id();
    /// Increment the edge id of this point site.
    void dec_edge_id();
    /// \}

    /// Accessors.
    /// \{
    /// Return the first associated vertex.
    P first() const;
    /// Return the second associated vertex.
    P second() const;

    /// Return the id of the first associated vertex.
    // FIXME: Rename as first_vertex_id.
    util::vertex_id first_id() const;
    /// Return the id of the second associated vertex.
    // FIXME: Rename as second_vertex_id.
    util::vertex_id second_id() const;
    /// \}

  private:
    /// Site-related members.
    /// \{
    /// Update the site corresponding to this psite.
    void update_();
    // The site corresponding to this psite.
    site_pair<P> p_;
    /// \}

  private:
    /// Graph-related members.
    /// \{
    /// The p_line_graph this point site belongs to.
    const target* plg_;
    /// The id of the edge this psite is pointing towards.
    util::edge_id id_;
    /// \}
  };


  /// Comparison of two mln::line_graph_psite<P> instances.
  /// \{
  /* FIXME: Shouldn't those comparisons be part of a much general
     mechanism?  */

  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_line_graph.
  template <typename P>
  bool
  operator==(const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs);

  /// \brief Is \a lhs not equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_line_graph.
  template <typename P>
  bool
  operator!=(const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs);

  /// \brief Is \a lhs ``less'' than \a rhs?
  ///
  /// This comparison is required by algorithms sorting psites.
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_line_graph.
  template <typename P>
  bool
  operator< (const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs);
  /// \}


  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const line_graph_psite<P>& p);



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  line_graph_psite<P>::line_graph_psite()
    : plg_(0)
  {
  }

  template <typename P>
  inline
  line_graph_psite<P>::line_graph_psite(const p_line_graph<P>& plg,
					util::edge_id id)
    : plg_(&plg),
      id_(id)
  {
    update_();
  }

  template <typename P>
  inline
  const p_line_graph<P>&
  line_graph_psite<P>::site_set() const
  {
    mln_precondition(plg_);
    return *plg_;
  }

  template <typename P>
  inline
  const p_line_graph<P>*
  line_graph_psite<P>::target_() const
  {
    return plg_;
  }

  template <typename P>
  inline
  void
  line_graph_psite<P>::change_target(const p_line_graph<P>& new_target)
  {
    plg_ = & new_target;
    invalidate();
  }

  template <typename P>
  inline
  const site_pair<P>&
  line_graph_psite<P>::subj_()
  {
    // FIXME: p_ is not properly updated yet; we shouldn't call this
    // method yet.
    abort();
    return p_;
  }


  template <typename P>
  inline
  bool
  line_graph_psite<P>::is_valid() const
  {
    /* FIXME: Instead of `plg_->gr_->nedges()', we should have
       something like `run_->has_edge_id(id_)' (see the implementation of
       p_run_psite.  */
    return plg_ && id_ < plg_->gr_->nedges();
  }

  template <typename P>
  inline
  void
  line_graph_psite<P>::invalidate()
  {
    /* FIXME: Instead of `plg_->gr_->nedges()', we should have
       something like `run_->has_edge_id(id_)' (see the implementation of
       p_run_psite.  */
    id_ = plg_->gr_->nedges();
  }


  template <typename P>
  inline
  util::edge_id
  line_graph_psite<P>::edge_id() const
  {
    return id_;
  }

  template <typename P>
  inline
  void
  line_graph_psite<P>::change_edge_id(const util::edge_id& id)
  {
    id_ = id;
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  void
  line_graph_psite<P>::inc_edge_id()
  {
    ++id_.to_equiv();
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  void
  line_graph_psite<P>::dec_edge_id()
  {
    --id_.to_equiv();
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  P
  line_graph_psite<P>::first() const
  {
    mln_assertion(is_valid());
    // FIXME: Too low-level.
    return plg_->gr_->vertex_data(first_id());
  }

  template <typename P>
  inline
  P
  line_graph_psite<P>::second() const
  {
    mln_assertion(is_valid());
    // FIXME: Too low-level.
    return plg_->gr_->vertex_data(second_id());
  }

  template <typename P>
  inline
  util::vertex_id
  line_graph_psite<P>::first_id() const
  {
    mln_assertion(is_valid());
    // FIXME: Too low-level.
    return plg_->gr_->edge(id_).v1();
  }

  template <typename P>
  inline
  util::vertex_id
  line_graph_psite<P>::second_id() const
  {
    mln_assertion(is_valid());
    // FIXME: Too low-level.
    return plg_->gr_->edge(id_).v2();
  }


  template <typename P>
  inline
  void
  line_graph_psite<P>::update_()
  {
    mln_assertion(is_valid());
    p_.pair_.change_both(first(), second());
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <typename P>
  bool
  operator==(const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.edge_id() == rhs.edge_id();
  }

  template <typename P>
  bool
  operator!=(const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.edge_id() != rhs.edge_id();
  }

  template <typename P>
  bool
  operator< (const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.edge_id() < rhs.edge_id();
  }


  /*------------------.
  | Pretty-printing.  |
  `------------------*/

  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const line_graph_psite<P>& p)
  {
    return ostr << '(' << p.first() << " -- " << p.second() << ')';
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln

#endif // MLN_CORE_IMAGE_LINE_GRAPH_PSITE_HH
