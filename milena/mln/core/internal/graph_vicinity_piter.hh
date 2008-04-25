// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_GRAPH_VICINITY_PITER_HH
# define MLN_CORE_INTERNAL_GRAPH_VICINITY_PITER_HH

/// \file   mln/core/internal/graph_vicinity_piter.hh
/// \brief  Factored implementation for point iterators on a graph windows
/// and graph neighborhoods, called "vicinities".

# include <mln/core/concept/point_iterator.hh>
# include <mln/core/p_graph.hh>
# include <mln/core/graph_psite.hh>

/* FIXME: Due to the poor interface of mln::p_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */

namespace mln
{
  // Fwd decls.
  template <typename P> class p_graph;
  template <typename P> class graph_psite;


  /*----------------------------------------.
  | internal::graph_vicinity_piter_<P, E>.  |
  `----------------------------------------*/

  namespace internal
  {

    /// \brief Base for iterator on a graph vicinity.
    template <typename P, typename E>
    class graph_vicinity_piter_ : public Point_Iterator< E >
    {
      typedef graph_vicinity_piter_<P, E> self_;
      typedef Point_Iterator< self_ > super_;

    public:
      enum { dim = P::dim };

      typedef graph_psite<P> psite;
      typedef P point;
      typedef mln_coord(P) coord;
      // FIXME: Dummy typedef.
      typedef void dpoint;
      // FIXME: Dummy value.
      typedef void mesh;

    public:
      /// Construction.
      /// \{
      template <typename Pref>
      graph_vicinity_piter_(const Point_Site<Pref>& p_ref);
      /// \}

      /// Manipulation.
      /// \{
      /// Test if the iterator is valid.
      bool is_valid() const;
      /// Invalidate the iterator.
      void invalidate();

      /// Is the piter adjacent to the reference point?
      bool adjacent_to_p_ref_() const;
      /// Is the piter adjacent or equal to the reference point?
      bool adjacent_or_equal_to_p_ref_() const;
      /// Update the internal data of the iterator.
      void update_();
      /// \}

      /// Conversion and accessors.
      /// \{
      /// Reference to the corresponding point.
      const point& to_point() const;
      /// Reference to the corresponding point site.
      const psite& to_psite() const;
      /// Convert the iterator into a line graph psite.
      operator psite() const;

      /// Read-only access to the \a i-th coordinate.
      coord operator[](unsigned i) const;
      /// \}

      /// Internals, used by the vicinity.
      /// \{
    public:
      /// An internal iterator on the set of nodes of the underlying graph.
      util::node_id id_;
      /// \}

    protected:
      /// The ``central'' psite of the vicinity (for instance, the
      /// center of the neighborhood, in the case of a neighborhood
      /// piter).
      const psite& p_ref_;

    private:
      /// The psite corresponding to this iterator.
      psite psite_;
      /// The point corresponding to this iterator.
      point p_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename E>
    template <typename Pref>
    inline
    graph_vicinity_piter_<P, E>::graph_vicinity_piter_(const Point_Site<Pref>& p_ref)
      : p_ref_(exact(p_ref).to_psite()),
	// Initialize psite_ to a dummy value.
	psite_(),
	p_()
    {
      // Invalidate id_.
      invalidate();
    }

    template <typename P, typename E>
    inline
    bool
    graph_vicinity_piter_<P, E>::is_valid() const
    {
      /* FIXME: We depend too much on the implementation of
	 util::graph here.  The util::graph should provide the service
	 to abstract these manipulations.  */
      return p_ref_.is_valid() && id_ < p_ref_.pg().npoints();
    }

    template <typename P, typename E>
    inline
    void
    graph_vicinity_piter_<P, E>::invalidate()
    {
      id_ = -1;
    }

    template <typename P, typename E>
    inline
    bool
    graph_vicinity_piter_<P, E>::adjacent_to_p_ref_() const
    {
      return p_ref_.pg().adjacent(p_ref_.id(), id_);
    }

    template <typename P, typename E>
    inline
    bool
    graph_vicinity_piter_<P, E>::adjacent_or_equal_to_p_ref_() const
    {
      return p_ref_.pg().adjacent_or_equal(p_ref_.id(), id_);
    }

    template <typename P, typename E>
    inline
    void
    graph_vicinity_piter_<P, E>::update_()
    {
      // Update psite_.
      psite_ = graph_psite<P>(p_ref_.pg(), id_);
      // Update p_.
      p_ = p_ref_.pg().point_from_id(id_);
    }

    template <typename P, typename E>
    inline
    const P&
    graph_vicinity_piter_<P, E>::to_point() const
    {
      return p_;
    }

    template <typename P, typename E>
    inline
    const graph_psite<P>&
    graph_vicinity_piter_<P, E>::to_psite() const
    {
      return psite_;
    }

    template <typename P, typename E>
    inline
    graph_vicinity_piter_<P, E>::operator graph_psite<P>() const
    {
      mln_precondition(is_valid());
      return psite_;
    }

    template <typename P, typename E>
    inline
    mln_coord(P)
    graph_vicinity_piter_<P, E>::operator[](unsigned i) const
    {
      assert(i < dim);
      return p_[i];
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_GRAPH_VICINITY_PITER_HH
