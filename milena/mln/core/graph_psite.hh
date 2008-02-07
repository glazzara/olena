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
    graph_psite(const p_graph<P>& pg_, unsigned id);
    graph_psite(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Access to point/psite.
    /// \{
    operator P() const;
    const point& to_point() const;
    coord operator[](unsigned id) const;
    /// \}

    /// Return the mln::p_graph this point site belongs to.
    const p_graph<P>& pg() const;
    /// Return the node id of this point site.
    util::node_id id() const;

  private:
    const p_graph<P>& pg_;
    util::node_id id_;
  };



# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  inline
  graph_psite<P>::graph_psite(const p_graph<P>& g, util::node_id id)
    : pg_(g),
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
    // FIXME: Could we get rid of this cast?
    const_cast< p_graph<P>& >(pg_) = rhs.pg_;
    id_ = rhs.id_;
    return *this;
  }

  template<typename P>
  inline
  graph_psite<P>::operator P() const
  {
    return pg_.gr_.node_data(id_);
  }

  template<typename P>
  inline
  const P&
  graph_psite<P>::to_point() const
  {
    return pg_.gr_.node_data(id_);
  }

  template<typename P>
  inline
  mln_coord(P)
  graph_psite<P>::operator[](util::node_id id) const
  {
    return to_point()[id];
  }

  template<typename P>
  inline
  const p_graph<P>&
  graph_psite<P>::pg() const
  {
    return pg_;
  }

  template<typename P>
  inline
  util::node_id
  graph_psite<P>::id() const
  {
    return id_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of mln

#endif // MLN_CORE_GRAPH_PSITE_HH
