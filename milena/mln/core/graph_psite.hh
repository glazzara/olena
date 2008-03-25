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

#ifndef MLN_CORE_GRAPH_PSITE_HH
# define MLN_CORE_GRAPH_PSITE_HH

/// \file mln/core/graph_psite.hh
/// \brief Definition of a graph-based point site.

# include <mln/core/p_graph.hh>


namespace mln
{

  // Fwd decl.
  template<typename P> class p_graph;


  /// \brief Point site associated to a mln::graph_image.
  template<typename P>
  class graph_psite : public Point_Site< graph_psite<P> >
  {
    typedef graph_psite<P> self_;

  public:
    typedef mln_mesh(P) mesh;
    enum { dim = P::dim };
    typedef P point;
    typedef mln_dpoint(P) dpoint;
    typedef mln_coord(P) coord;

    /// Construction and assignment.
    /// \{
    graph_psite();
    graph_psite(const p_graph<P>& pg_, unsigned id);
    graph_psite(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Access to psite.
    const self_& to_psite() const;

    /// Access to point.
    /// \{
    const point& to_point() const;
    coord operator[](unsigned id) const;
    /// \}

    /// Return the p_graph this point site belongs to.
    const p_graph<P>& pg() const;
    /// Return the node id of this point site.
    util::node_id id() const;

    /// Is this psite valid?
    bool is_valid() const;

  private:
    /// The p_graph this point site belongs to.
   const p_graph<P>* pg_;
    /// The id of the node this psite is pointing towards.
    util::node_id id_;
  };

  /// Compare two mln::graph_psite<P> instances.
  /* FIXME: Shouldn't this comparison be part of a much general
     mechanism?  */
  template <typename P>
  bool
  operator==(const graph_psite<P>& lhs, const graph_psite<P>& rhs);


# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  inline
  graph_psite<P>::graph_psite()
    // Dummy initializations.
    : pg_(0),
      id_(-1)
  {
  }

  template<typename P>
  inline
  graph_psite<P>::graph_psite(const p_graph<P>& g, util::node_id id)
    : pg_(&g),
      id_(id)
  {
  }

  template<typename P>
  inline
  graph_psite<P>::graph_psite(const graph_psite<P>& rhs)
    : pg_(rhs.pg_),
      id_(rhs.id_)
  {
  }

  template<typename P>
  inline
  graph_psite<P>&
  graph_psite<P>::operator= (const graph_psite<P>& rhs)
  {
    if (&rhs == this)
      return *this;
    pg_ = rhs.pg_;
    id_ = rhs.id_;
    return *this;
  }

  template<typename P>
  inline
  bool
  graph_psite<P>::is_valid() const
  {
    return pg_ && id_ < pg_->gr_->nnodes();
  }

  template<typename P>
  inline
  const graph_psite<P>&
  graph_psite<P>::to_psite() const
  {
    return *this;
  }

  template<typename P>
  inline
  const P&
  graph_psite<P>::to_point() const
  {
    return pg().point_from_id(id_);
  }

  template<typename P>
  inline
  mln_coord(P)
  graph_psite<P>::operator[](unsigned i) const
  {
    mln_assertion(is_valid());
    return to_point()[i];
  }

  template<typename P>
  inline
  const p_graph<P>&
  graph_psite<P>::pg() const
  {
    mln_assertion(pg_);
    return *pg_;
  }

  template<typename P>
  inline
  util::node_id
  graph_psite<P>::id() const
  {
    return id_;
  }

  template <typename P>
  bool
  operator==(const graph_psite<P>& lhs, const graph_psite<P>& rhs)
  {
    return &lhs.pg() == &rhs.pg() && lhs.id() == rhs.id();
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of mln

#endif // MLN_CORE_GRAPH_PSITE_HH
