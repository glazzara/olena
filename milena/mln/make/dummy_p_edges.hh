// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_DUMMY_P_EDGES_HH
# define MLN_MAKE_DUMMY_P_EDGES_HH

/// \file
///
/// Create a p_edges which associate a graph element to a constant site.
///
/// \sa edge_image, p_edges


# include <mln/core/concept/graph.hh>
# include <mln/core/site_set/p_edges.hh>
# include <mln/pw/cst.hh>


namespace mln
{

  namespace make
  {


    /// Create a p_edges which associate a graph element to a constant site.
    ///
    /// \param[in] g_ A graph.
    /// \param[in] dummy_site The dummy site mapped to graph edges.
    ///
    /// \return A p_edges.
    //
    template <typename G, typename P>
    p_edges< G, pw::cst_<P> >
    dummy_p_edges(const Graph<G>& g_, const P& dummy_site);


    /// Create a p_edges which associate a graph element to a constant site.
    /// \c 0 (int) is used as dummy site.
    ///
    /// \param[in] g A graph.
    ///
    /// \return A p_edges.
    //
    template <typename G>
    p_edges<G>
    dummy_p_edges(const Graph<G>& g);


# ifndef MLN_INCLUDE_ONLY


    template <typename G, typename P>
    p_edges<G,pw::cst_<P> >
    dummy_p_edges(const Graph<G>& g_, const P& dummy_site)
    {
      mln_trace("dummy_p_edges");

      const G& g = exact(g_);
      mln_precondition(g.is_valid());

      p_edges< G, pw::cst_<P> > pe(g, pw::cst(dummy_site));

      return pe;
    }


    template <typename G>
    p_edges<G>
    dummy_p_edges(const Graph<G>& g)
    {
      return p_edges<G>(g);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_DUMMY_P_EDGES_HH
