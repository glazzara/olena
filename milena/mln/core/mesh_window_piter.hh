// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_MESH_WINDOW_PITER_HH
# define MLN_CORE_MESH_WINDOW_PITER_HH

/*!
 * \file   mln/core/mesh_window_piter.hh
 *
 * \brief  Definition of point iterator of mesh window.
 *
 */

# include <mln/core/concept/point_iterator.hh>
# include <mln/core/mesh_p.hh>
# include <mln/core/mesh_psite.hh>


namespace mln
{
  // Fwd decls.
  template <typename P> class mesh_p;
  template <typename P> class mesh_psite;


  template <typename P>
  class mesh_window_fwd_piter :
    public Point_Iterator< mesh_window_fwd_piter<P> > // or Iterator<...>?
  {
    typedef mesh_window_fwd_piter<P> self_;
    typedef Point_Iterator< self_ > super_;

  public:
    typedef mesh_psite<P> psite;

    enum { dim = P::dim };
    typedef mesh_p<P> mesh;

    typedef P point;
    // FIXME: Dummy typedef.
    typedef void dpoint;
    typedef mln_coord(P) coord;

  public:
    template <typename W, typename Pref>
    mesh_window_fwd_piter(const W& win,
			  const Point_Site<Pref>& p_ref);

    bool is_valid() const;
    void invalidate();
    void start();

    void next_();
    bool adjacent_or_equal_to_p_ref_() const;

    // FIXME: In fact, this method should be named `to_psite', since
    // it return as mln::mesh_psite<P> object, not a P object.
    const point& to_point() const;
    operator psite () const;
    coord operator[](unsigned i) const;

  private:
    /// The ``central'' point of the window.
    const psite& p_ref_;

    /// An internal iterator on the set of nodes of the underlying graph.
    unsigned i_;
  };

  // FIXME: mesh_window_bkd_piter.


# ifndef MLN_INCLUDE_ONLY

  // FIXME: Currently, argument win is ignored.
  template <typename P>
  template <typename W, typename Pref>
  mesh_window_fwd_piter<P>::mesh_window_fwd_piter(const W& /* win */,
						  const Point_Site<Pref>& p_ref)
    : p_ref_(exact(p_ref).to_psite())
  {
    // Invalidate i_.
    invalidate();
  }

  template <typename P>
  bool
  mesh_window_fwd_piter<P>::is_valid() const
  {
    // FIXME: We depend too much on the implementation of util::graph
    // here.  The util::graph should provide the service to abstract
    // these manipulations.
    return i_ >= 0 && i_ < p_ref_.mesh_.gr_.nb_node_;
  }

  template <typename P>
  void
  mesh_window_fwd_piter<P>::invalidate()
  {
    i_ = p_ref_.mesh_.gr_.nb_node_;
  }

  template <typename P>
  void
  mesh_window_fwd_piter<P>::start()
  {
    i_ = 0;
    if (!adjacent_or_equal_to_p_ref_())
      next_();
  }

  template <typename P>
  void
  mesh_window_fwd_piter<P>::next_()
  {
    // FIXME: This is inefficient.  The graph structure should be able
    // to produce the set of adjacent nodes fast.  Boost Graphs
    // probably provides adequates structures to fetch these
    // neighbors in constant time.
    do
      ++i_;
    while (is_valid() && !adjacent_or_equal_to_p_ref_());
  }

  template <typename P>
  bool
  mesh_window_fwd_piter<P>::adjacent_or_equal_to_p_ref_() const
  {
    // FIXME: Likewise, this is inefficient.

    // Check wether the iterator points to P_REF_.
    if (i_ == p_ref_.i_)
      return true;

    typedef std::list<unsigned> adjacency_type;

    // Check whether the iterator is among the neighbors of P_REF_.
    {
      // Paranoid assertion.
      assert (p_ref_.i_ >= 0 && 
	      p_ref_.i_ < p_ref_.mesh_.gr_.nodes_.size ());
      const adjacency_type& p_ref_neighbs =
	p_ref_.mesh_.gr_.nodes_[p_ref_.i_]->links;
      adjacency_type::const_iterator j =
	std::find (p_ref_neighbs.begin(), p_ref_neighbs.end(), i_);
      if (j != p_ref_neighbs.end())
	return true;
    }

    // Check whether P_REF_ is among the neighbors of the iterator.
    {
      assert (is_valid ());
      const adjacency_type& i_neighbs = p_ref_.mesh_.gr_.nodes_[i_]->links;
      adjacency_type::const_iterator k =
	std::find (i_neighbs.begin(), i_neighbs.end(), p_ref_.i_);
      if (k != i_neighbs.end())
	return true;
    }

    // Otherwise, the iterator is not adjacent to P_REF_.
    return false;
  }

  template <typename P>
  const P&
  mesh_window_fwd_piter<P>::to_point() const
  {
    return p_ref_.mesh_.loc_[i_];
  }

  template <typename P>
  mesh_window_fwd_piter<P>::operator mesh_psite<P> () const
  {
    return mesh_psite<P>(p_ref_.mesh_, i_);
  }

  template <typename P>
  inline
  mln_coord(P)
  mesh_window_fwd_piter<P>::operator[](unsigned i) const
  {
    assert(i < dim);
    return to_point()[i];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_MESH_WINDOW_PITER_HH
