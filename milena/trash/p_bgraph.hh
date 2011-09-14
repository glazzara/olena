// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_BGRAPH_P_HH
# define MLN_CORE_BGRAPH_P_HH

# include <utility>

# include <mln/core/concept/point_site.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/accu/shape/bbox.hh>
# include <mln/util/internal/boost_graph.hh>
# include <mln/core/image/bgraph_psite.hh>
# include <mln/core/site_set/p_bgraph_piter.hh>



/// \file mln/core/site_set/p_bgraph.hh
///
/// Definition of a point set based on a boost graph.

namespace mln
{

  template<typename P> class p_bgraph_piter_;

  template<typename P>
  struct p_bgraph
    : public internal::site_set_base_< bgraph_psite<P>, p_bgraph<P> >
  {
    typedef util::internal::boost_graph<P, util::empty> graph;

    /// Point_Site associated type.
    typedef bgraph_psite<P> psite;

    /// Forward Site_Iterator associated type.
    typedef p_bgraph_piter_<P> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_bgraph_piter_<P> bkd_piter;

    /// Graph vertex/edge identifier
    typedef typename graph::vertex_descriptor node_id;
    typedef typename graph::edge_descriptor edge_id;

    /// Graph vertex/edge iterator
    typedef typename graph::vertex_iterator node_iterator;
    typedef typename graph::edge_iterator edge_iterator;
    typedef std::pair<node_iterator, node_iterator> node_iterators;


    /// Construct a graph psite set from a graph of points.
    /// \p gr is a pointer on a boost graph.
    /// This boost graph must have been allocated dynamically.
    p_bgraph (graph* gr);

    /// Return The number of points (i.e., nodes) in the graph.
    unsigned nsites() const;

    /// Return The number of lines (i.e., edges) in the graph.
    unsigned nlines() const;

    /// Give the exact bounding box.
    const box<P>& bbox() const;

    bool has(const psite& p) const;

    /// Return the graph point (FIXME site?) from an index
    const P& point_from_id(const node_id& id) const;
    P& point_from_id(const node_id& id);

    /// Return the point contained in the first node adjacent
    // to the edge id \a e.
    const P& node1(const edge_id& e) const;
    /// Return the point contained in the second node adjacent
    /// to the edge  id \a e.
    const P& node2(const edge_id& e) const;

    /// Return the graph associated to the p_bgraph domain:
    const graph& to_graph() const;
    graph& to_graph();


  private:
    graph* gr_;
    box<P> bb_;
  };

# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  inline
  p_bgraph<P>::p_bgraph (graph* gr)
    : gr_ (gr)
  {
    mln_precondition(gr != 0);
    // FIXME: Warning: if the underlying graph is updated later, this
    // won't be taken into account by this p_bgraph!
    accu::shape::bbox<P> a;

    for (node_iterators iter = boost::vertices(*gr_);
	 iter.first != iter.second;
	 ++iter.first)
      a.take((*gr_)[*iter.first]);
    bb_ = a.to_result();
  }

  template<typename P>
  inline
  unsigned
  p_bgraph<P>::nsites() const
  {
    return boost::num_vertices(*gr_);
  }

  template<typename P>
  inline
  unsigned
  p_bgraph<P>::nlines() const
  {
    return boost::num_edges(gr_->nedges());
  }

  template<typename P>
  inline
  const box<P>&
  p_bgraph<P>::bbox() const
  {
    return bb_;
  }

  template<typename P>
  inline
  bool
  p_bgraph<P>::has(const psite& p) const
  {
    return
      // Check whether P is compatible with this psite set.
      (&p.pg() == this) &&
      // Check that the node id of P belongs to the range of valid node ids.
      (p.id() < this->nsites());
  }


  template <typename P>
  const P&
  p_bgraph<P>::point_from_id(const node_id& id) const
  {
    return this->gr_->operator[](id);
  }

  template <typename P>
  P&
  p_bgraph<P>::point_from_id(const node_id& id)
  {
    return this->gr_->operator[](id);
  }


  /// FIXME: Compare to p_bgraph, here we are loosing the vertex ordering
  /// information. Is it bad??
  template <typename P>
  const P&
  p_bgraph<P>::node1(const edge_id& e) const
  {
    return this->point_from_id(source(e, *this->gr_));
  }

  /// FIXME: same as node1...
  template <typename P>
  const P&
  p_bgraph<P>::node2(const edge_id& e) const
  {
    return this->point_from_id(target(e, *this->gr_));
  }

  template <typename P>
  const typename p_bgraph<P>::graph&
  p_bgraph<P>::to_graph() const
  {
    return *this->gr_;
  }

  template <typename P>
  typename p_bgraph<P>::graph&
  p_bgraph<P>::to_graph()
  {
    return *this->gr_;
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_CORE_BGRAPH_P_HH
