// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_DUMMY_P_VERTICES_HH
# define MLN_MAKE_DUMMY_P_VERTICES_HH

/// \file mln/make/dummy_p_vertices.hh
///
/// Create a p_vertices which associate a graph element to a constant site.
///
/// \sa edge_image, p_vertices


# include <mln/core/concept/graph.hh>
# include <mln/core/site_set/p_vertices.hh>
# include <mln/pw/cst.hh>


namespace mln
{

  namespace make
  {


    /// Create a p_vertices which associate a graph element to a constant site.
    ///
    /// \param[in] g_ A graph.
    /// \param[in] dummy_site The dummy site mapped to graph vertices.
    ///
    /// \return A p_vertices.
    //
    template <typename G, typename P>
    p_vertices< G, pw::cst_<P> >
    dummy_p_vertices(const Graph<G>& g_, const P& dummy_site);


    /// Create a p_vertices which associate a graph element to a constant site.
    /// \c 0 (int) is used as dummy site.
    ///
    /// \param[in] g_ A graph.
    ///
    /// \return A p_vertices.
    //
    template <typename G>
    p_vertices< G, pw::cst_<int> >
    dummy_p_vertices(const Graph<G>& g);


# ifndef MLN_INCLUDE_ONLY


    template <typename G, typename P>
    p_vertices<G,pw::cst_<P> >
    dummy_p_vertices(const Graph<G>& g_, const P& dummy_site)
    {
      trace::entering("dummy_p_vertices");

      const G& g = exact(g_);
      mln_precondition(g.is_valid());

      p_vertices< G, pw::cst_<P> > pe(g, pw::cst(dummy_site));

      trace::exiting("dummy_p_vertices");
      return pe;
    }


    template <typename G>
    p_vertices< G, pw::cst_<int> >
    dummy_p_vertices(const Graph<G>& g)
    {
      return dummy_p_vertices(g, 0);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln

#endif // ! MLN_MAKE_DUMMY_P_VERTICES_HH
