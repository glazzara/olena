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

#ifndef MLN_CORE_IMAGE_GRAPH_PSITE_HH
# define MLN_CORE_IMAGE_GRAPH_PSITE_HH

/// \file mln/core/image/graph_psite.hh
/// \brief Definition of a graph-based psite.

# include <mln/core/internal/pseudo_site_base.hh>

# include <mln/core/site_set/p_graph.hh>

/* FIXME: This class shares a lot with line_graph_psite.  Factor as
   much as possible.  */

// FIXME: Rename graph_psite as p_graph_psite, and move this
// to core/site_set.


namespace mln
{

  // Forward declaration.
  template <typename P> class p_graph;
  template <typename P> class graph_psite;


  /// \brief Point site associated to a mln::graph_image.
  ///
  /// \arg \p P The type of the site.
  template <typename P>
  class graph_psite
    : public internal::pseudo_site_base_< const P&, graph_psite<P> >
  {
    typedef graph_psite<P> self_;

  public:
    // This associated type is important to know that this particular
    // pseudo site knows the site set it refers to.
    typedef p_graph<P> target;

    /// Construction and assignment.
    /// \{
    graph_psite();
    graph_psite(const p_graph<P>& pg_, util::vertex_id id);
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
    const P& subj_();
    /// \}

    /// Vertex id manipulators.
    //// \{
    /// Return the vertex id of this point site.
    util::vertex_id vertex_id() const;
    /// Set the vertex id of this point site.
    void change_vertex_id(const util::vertex_id& id);
    /// Increment the vertex id of this point site.
    void inc_vertex_id();
    /// Increment the vertex id of this point site.
    void dec_vertex_id();
    /// \}

  private:
    /// Site-related members.
    /// \{
    /// Update the site corresponding to this psite.
    void update_();
    // The site corresponding to this psite.
    P p_;
    /// \}

  private:
    /// Graph-related members.
    /// \{
    /// The p_graph this point site belongs to.
   const target* pg_;
    /// The id of the vertex this psite is pointing towards.
    util::vertex_id id_;
    /// \}
  };


  /// Comparison of two mln::graph_psite<P> instances.
  /// \{
  /* FIXME: Shouldn't those comparisons be part of a much general
     mechanism?  */

  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_graph.
  template <typename P>
  bool
  operator==(const graph_psite<P>& lhs, const graph_psite<P>& rhs);

  /// \brief Is \a lhs not equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_graph.
  template <typename P>
  bool
  operator!=(const graph_psite<P>& lhs, const graph_psite<P>& rhs);

  /// \brief Is \a lhs ``less'' than \a rhs?
  ///
  /// This comparison is required by algorithms sorting psites.
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_graph.
  template <typename P>
  bool
  operator< (const graph_psite<P>& lhs, const graph_psite<P>& rhs);
  /// \}


  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const graph_psite<P>& p);



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  graph_psite<P>::graph_psite()
    : pg_(0)
  {
  }

  template <typename P>
  inline
  graph_psite<P>::graph_psite(const p_graph<P>& g, util::vertex_id id)
    // FIXME: Use change_target instead.
    : pg_(&g),
      id_(id)
  {
    update_();
  }

  template <typename P>
  inline
  bool
  graph_psite<P>::is_valid() const
  {
    /* FIXME: Instead of `plg_->gr_->nvertices()', we should have
       something like `run_->has_edge_id(id_)' (see the implementation of
       p_run_psite.  */
    return pg_ && id_ < pg_->gr_->nvertices();
  }

  template <typename P>
  inline
  void
  graph_psite<P>::invalidate()
  {
    /* FIXME: Instead of `plg_->gr_->nvertices()', we should have
       something like `run_->has_edge_id(id_)' (see the implementation of
       p_run_psite.  */
    id_ = pg_->gr_->nvertices();
  }

  template <typename P>
  inline
  const p_graph<P>&
  graph_psite<P>::site_set() const
  {
    mln_precondition(target_());
    return *target_();
  }

  template <typename P>
  inline
  const p_graph<P>*
  graph_psite<P>::target_() const
  {
    return pg_;
  }

  template <typename P>
  inline
  void
  graph_psite<P>::change_target(const p_graph<P>& new_target)
  {
    pg_ = &new_target;
    invalidate();
  }

  // FIXME: Write or extend a test to exercise this method.
  template <typename P>
  inline
  const P&
  graph_psite<P>::subj_()
  {
    return p_;
  }

  template <typename P>
  inline
  util::vertex_id
  graph_psite<P>::vertex_id() const
  {
    return id_;
  }

  template <typename P>
  inline
  void
  graph_psite<P>::change_vertex_id(const util::vertex_id& id)
  {
    id_ = id;
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  void
  graph_psite<P>::inc_vertex_id()
  {
    ++id_.to_equiv();
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  void
  graph_psite<P>::dec_vertex_id()
  {
    --id_.to_equiv();
    if (is_valid())
      update_();
  }

  template <typename P>
  inline
  void
  graph_psite<P>::update_()
  {
    mln_precondition(is_valid());
    p_ = site_set().point_from_id(id_);
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <typename P>
  bool
  operator==(const graph_psite<P>& lhs, const graph_psite<P>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.vertex_id() == rhs.vertex_id();
  }

  template <typename P>
  bool
  operator!=(const graph_psite<P>& lhs, const graph_psite<P>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.vertex_id() != rhs.vertex_id();
  }

  template <typename P>
  bool
  operator< (const graph_psite<P>& lhs, const graph_psite<P>& rhs)
  {
    mln_assertion(lhs.target_() == rhs.target_());
    return lhs.vertex_id() < rhs.vertex_id();
  }


  /*------------------.
  | Pretty-printing.  |
  `------------------*/

  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const graph_psite<P>& p)
  {
    return ostr << p.unproxy_();
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of mln

#endif // MLN_CORE_IMAGE_GRAPH_PSITE_HH
