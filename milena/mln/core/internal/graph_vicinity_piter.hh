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

# include <set>

# include <mln/core/internal/site_relative_iterator_base.hh>
# include <mln/core/site_set/p_graph.hh>
# include <mln/core/image/graph_psite.hh>

/* FIXME: Factor those classes:

   - mln::internal::graph_vicinity_piter.hh
   - mln::internal::line_graph_vicinity_piter.hh  */

/* FIXME: Due to the poor interface of mln::p_graph and
   mln::util::graph, we show to much implementation details here.
   Enrich their interfaces to avoid that.  */


namespace mln
{
  // Fwd decls.
  template <typename P> class p_graph;
  template <typename P> class graph_psite;

  // FIXME: Consider renaming graph_vicinity_piter_ as
  // graph_relative_piter_.


  namespace internal
  {

    /// \brief Base for iterator on a graph vicinity.
    template <typename P, typename S, typename E>
    class graph_vicinity_piter_
      : public internal::site_relative_iterator_base< S, E >
    {
    public:
      enum { dim = P::dim };

      // FIXME: Dummy typedef.
      typedef void dpoint;
      // FIXME: Dummy value.
      typedef void mesh;

      // The type of the set of vicinity sites (adjacent vertex ids).
      typedef std::set<util::vertex_id> sites_t;

    public:
      /// Return the set of sites (adjacent vertex ids).
      sites_t& sites();

    protected:
      /// Construction.
      /// \{
      graph_vicinity_piter_();
      template <typename Pref>
      graph_vicinity_piter_(const Pref& p_ref);
      /// \}

    protected:
      /// The set of edge ids adjacent to the reference psite.
      sites_t sites_;
    };


    /// Print an mln::line_graph_vicinity_piter_<P, S, E>.
    template <typename P, typename S, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const graph_vicinity_piter_<P, S, E>& p);



# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename S, typename E>
    inline
    graph_vicinity_piter_<P, S, E>::graph_vicinity_piter_()
    {
    }

    template <typename P, typename S, typename E>
    template <typename Pref>
    inline
    graph_vicinity_piter_<P, S, E>::graph_vicinity_piter_(const Pref& p_ref)
    {
      center_at(p_ref);
    }

    template <typename P, typename S, typename E>
    inline
    std::set<util::vertex_id>&
    graph_vicinity_piter_<P, S, E>::sites()
    {
      return sites_;
    }


    template <typename P, typename S, typename E>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const graph_vicinity_piter_<P, S, E>& p)
    {
      return ostr << p.unproxy_();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_GRAPH_VICINITY_PITER_HH
