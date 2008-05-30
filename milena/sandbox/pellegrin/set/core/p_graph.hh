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

#ifndef MLN_CORE_P_GRAPH_HH
# define MLN_CORE_P_GRAPH_HH

# include <mln/core/concept/point_site.hh>
# include <mln/core/internal/point_set_base.hh>
# include <mln/accu/bbox.hh>
# include <mln/util/graph.hh>
# include <mln/core/graph_psite.hh>
# include <mln/core/p_graph_piter.hh>
# include <trait/point_set.hh>


/// \file mln/core/p_graph.hh
/// \brief Definition of a point set based on graph.

namespace mln
{

  // Fwd decls.
  template <typename P> struct p_graph;
  template <typename P> class  p_graph_piter_;

  namespace trait
  {

    template <typename P>
    struct point_set_< p_graph<P> > : public default_point_set_< p_graph<P> >
    {
      typedef trait::point_set::arity::unique   arity;
      typedef trait::point_set::has_speed::fast has_speed;
    };

  }

  template<typename P>
  struct p_graph
    : public internal::point_set_base_< graph_psite<P>, p_graph<P> >
  {
    typedef util::graph<P> graph;

    /// Construct a graph psite set from a graph of points.
    p_graph (graph& gr);

    /// Point_Site associated type.
    typedef graph_psite<P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_graph_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_graph_piter_<P> bkd_piter;

    /// Return The number of points (i.e., nodes) in the graph.
    std::size_t npoints() const;

    /// Return The number of lines (i.e., edges) in the graph.
    std::size_t nlines() const;

    /// Give the exact bounding box.
    const box_<P>& bbox() const;

    bool has(const psite& p) const;

    /// Return the graph point (FIXME site?) from an index
    const P& point_from_id(const util::node_id& id) const;
    P& point_from_id(const util::node_id& id);


    /// Return the point contained in the first node adjacent
    // to the edge id \a e.
    const P& node1(const util::edge_id& e) const;
    /// Return the point contained in the second node adjacent
    /// to the edge  id \a e.
    const P& node2(const util::edge_id& e) const;

    /// Return true if the psites lhs and rhs are adjacent, or equal.
    bool adjacent_or_equal(const psite& lhs, const psite& rhs) const;

    /// Return true if the nodes lhs and rhs are adjacent, or equal
    bool adjacent_or_equal(const util::node_id& lhs,
			   const util::node_id& rhs) const;

    /// Return the graph associated to the p_graph domain:
    const graph& to_graph() const;
    graph& to_graph();


  private:
    graph gr_;
    // FIXME: (Roland) Is it really useful/needed?
    /* 2007-12-19: It seems so, since graph_image must implement a method
       named bbox().  Now the question is: should each image type have a
       bounding box?  */
    box_<P> bb_;
  };

# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  inline
  p_graph<P>::p_graph (util::graph<P>& gr)
    : gr_ (gr)
  {
    // FIXME: Warning: if the underlying graph is updated later, this
    // won't be taken into account by this p_graph!
    accu::bbox<P> a;
    for (unsigned i = 0; i < npoints(); ++i)
      a.take(gr_.node_data(i));
    bb_ = a.to_result();
  }

  // FIXME: Rename to npsites?  In fact, this depends on the
  // interface expected from models of Point_Sets.
  template<typename P>
  inline
  std::size_t
  p_graph<P>::npoints() const
  {
    return this->gr_.nnodes();
  }

  template<typename P>
  inline
  std::size_t
  p_graph<P>::nlines() const
  {
    return this->gr_.nedges();
  }

  template<typename P>
  inline
  const box_<P>&
  p_graph<P>::bbox() const
  {
    return bb_;
  }

  template<typename P>
  inline
  bool
  p_graph<P>::has(const psite& p) const
  {
    return
      // Check whether P is compatible with this psite set.
      (&p.pg() == this) &&
      // Check that the node id of P belongs to the range of valid node ids.
      (p.id() < gr_.nnodes());
  }

  template <typename P>
  const P&
  p_graph<P>::point_from_id(const util::node_id& id) const
  {
    return this->gr_.node_data(id);
  }

  template <typename P>
  P&
  p_graph<P>::point_from_id(const util::node_id& id)
  {
    return this->gr_.node_data(id);
  }

  template <typename P>
  const P&
  p_graph<P>::node1(const util::edge_id& e) const
  {
    util::node_id n1 = this->gr_.edge(e).n1();
    return this->point_from_id(n1);
  }

  template <typename P>
  const P&
  p_graph<P>::node2(const util::edge_id& e) const
  {
    util::node_id n2 = this->gr_.edge(e).n2();
    return this->point_from_id(n2);
  }

  template <typename P>
  inline
  bool
  p_graph<P>::adjacent_or_equal(const psite& lhs, const psite& rhs) const
  {
    assert (&lhs.pg() == this && rhs.pg() == this);
    return adjacent_or_equal(lhs.id(), rhs.id());
  }

  template <typename P>
  inline
  bool
  p_graph<P>::adjacent_or_equal(const util::node_id& lhs,
				const util::node_id& rhs) const
  {
    // FIXME: Likewise, this is inefficient.

    assert (lhs < this->npoints());
    assert (rhs < this->npoints());

    if (rhs == lhs)
      return true;

    // Check whether the iterator is among the neighbors of P_REF_.
    typedef std::vector<util::node_id> adjacency_type;
    const adjacency_type& lhs_neighbs = gr_.nodes()[lhs]->edges;

    adjacency_type::const_iterator j =
      std::find (lhs_neighbs.begin(), lhs_neighbs.end(), rhs);
    if (j != lhs_neighbs.end())
      return true;

    return false;
  }

  template <typename P>
  const typename p_graph<P>::graph&
  p_graph<P>::to_graph() const
  {
    return this->gr_;
  }

  template <typename P>
  typename p_graph<P>::graph&
  p_graph<P>::to_graph()
  {
    return this->gr_;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_CORE_P_GRAPH_HH
