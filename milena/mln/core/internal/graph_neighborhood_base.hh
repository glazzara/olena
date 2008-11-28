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

#ifndef MLN_CORE_INTERNAL_GRAPH_NEIGHBORHOOD_BASE_HH
# define MLN_CORE_INTERNAL_GRAPH_NEIGHBORHOOD_BASE_HH

/// \file mln/internal/graph_neighborhood_base.hh
///
/// FIXME: doc

# include <set>
# include <mln/core/concept/neighborhood.hh>

namespace mln
{

  template <typename G, typename F, typename P, typename E>
  class graph_neighborhood_base : public Neighborhood< E >
  {
    typedef graph_neighborhood_base<G, F, P, E> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of site corresponding to the neighborhood.
    typedef mln_site(P) site;

    // The type of the set of neighborhood sites (ids adjacent to the
    // reference psite).
    typedef std::set<unsigned> sites_t;
    /// \}

    /// Conversions.
    /// \{
    /// The window type corresponding to this neighborhood.
    typedef E window;
    /// Create a window corresponding to this neighborhood.
    E win() const;
    /// \}

  protected:
    graph_neighborhood_base();
  };

} // end of namespace mln

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  template <typename G, typename F, typename P, typename E>
  inline
  graph_neighborhood_base<G, F, P, E>::graph_neighborhood_base()
  {
  }

  template <typename G, typename F, typename P, typename E>
  inline
  E
  graph_neighborhood_base<G, F, P, E>::win() const
  {
    return E();
  }

} // end of namespace mln

# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_CORE_INTERNAL_GRAPH_NEIGHBORHOOD_BASE_HH

