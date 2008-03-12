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

#ifndef MLN_CORE_LINE_GRAPH_P_HH
# define MLN_CORE_LINE_GRAPH_P_HH

# include <mln/core/concept/point_site.hh>
# include <mln/core/internal/point_set_base.hh>
# include <mln/accu/bbox.hh>
# include <mln/util/graph.hh>
# include <mln/core/line_graph_psite.hh>
# include <mln/core/p_line_graph_piter.hh>

/* FIXME: This class shares a lot with p_graph.  Factor as much as
   possible.  */


/// \file mln/core/p_line_graph.hh
/// \brief Definition of a point set based on line graph.

namespace mln
{
  /* FIXME: Contray to, e.g., p_array, the sole parameter P of
     p_line_graph is expected to be a point, not a psite!!  We should
     have a uniform scheme for point site sets.  */

  template<typename P>
  struct p_line_graph
    : public internal::point_set_base_< line_graph_psite<P>, p_line_graph<P> >
  {
    typedef util::graph<P> graph;

    /// Construct a line graph psite set from a graph of points.
    p_line_graph (graph& gr);

    /// Point_Site associated type.
    typedef line_graph_psite<P> psite;

    /// Forward Point_Iterator associated type.
    typedef p_line_graph_fwd_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef p_line_graph_bkd_piter_<P> bkd_piter;

    /// Return The number of points (sites) of the set, i.e., the
    /// number of \em edges, since this is a point set based on a line
    /// graph.
    std::size_t npoints() const;

    /// Return The number of nodes (vertices) in the graph.
    std::size_t nnodes() const;
    /// Return The number of edges in the graph.
    std::size_t nedges() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    bool has(const psite& p) const;

    // FIXME: Should be private.
    graph gr_;
    // FIXME: (Roland) Is it really useful/needed?
    /* 2007-12-19: It seems so, since graph_image must implement a
       method named bbox().  Now the question is: should each image
       type have a bounding box?  */
    box_<P> bb_;
  };


  /// \brief Comparison between two mln::p_line_graph's.
  ///
  /// Two mln::p_line_graph's are considered equal if they have the
  /// same address.
  template <typename P>
  bool
  operator==(const p_line_graph<P>& lhs, const p_line_graph<P>& rhs);


# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  inline
  p_line_graph<P>::p_line_graph (util::graph<P>& gr)
    : gr_ (gr)
  {
    // FIXME: Warning: if the underlying graph is updated later, this
    // won't be taken into account by this p_line_graph!
    accu::bbox<P> a;
    for (unsigned i = 0; i < nnodes(); ++i)
      a.take(gr_.node_data(i));
    bb_ = a.to_result();
  }

  template<typename P>
  inline
  std::size_t
  p_line_graph<P>::npoints() const
  {
    return nedges();
  }

  template<typename P>
  inline
  std::size_t
  p_line_graph<P>::nnodes() const
  {
    return this->gr_.nnodes();
  }

  template<typename P>
  inline
  std::size_t
  p_line_graph<P>::nedges() const
  {
    return this->gr_.nedges();
  }

  template<typename P>
  inline
  const box_<P>&
  p_line_graph<P>::bbox() const
  {
    return bb_;
  }

  template<typename P>
  inline
  bool
  p_line_graph<P>::has(const psite& p) const
  {
    return
      // Check whether P is compatible with this psite set.
      (&p.plg() == this) &&
      // Check that the edge id of P belongs to the range of valid edge ids.
      (p.id() < gr_.nedges());
  }


  template <typename P>
  bool
  operator==(const p_line_graph<P>& lhs, const p_line_graph<P>& rhs)
  {
    return &lhs == &rhs;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_CORE_P_GRAPH_HH
