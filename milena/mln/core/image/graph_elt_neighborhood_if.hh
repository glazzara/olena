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

#ifndef MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_IF_HH
# define MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_IF_HH

/// \file
///
/// Definition of the elementary ``neighborhood_if'' on a graph.

# include <mln/core/neighb.hh>
# include <mln/core/internal/is_masked_impl_selector.hh>
# include <mln/core/image/graph_elt_window_if.hh>


namespace mln
{

  /// Elementary neighborhood_if on graph class.
  template <typename G, typename S, typename I>
  struct graph_elt_neighborhood_if
    : public neighb< graph_elt_window_if<G,S,I> >
  {
    typedef neighb< graph_elt_window_if<G,S,I> > super_;

    /// \name Constructors.
    /// \{

    /// Construct an invalid neighborhood.
    graph_elt_neighborhood_if();
    /// \param[in] mask A graph image of Boolean.
    graph_elt_neighborhood_if(const Image<I>& mask);

    /// \}

    /// Return the graph image used as mask.
    const I& mask() const;

  };


# ifndef MLN_INCLUDE_ONLY


  template <typename G, typename S, typename I>
  inline
  graph_elt_neighborhood_if<G,S,I>::graph_elt_neighborhood_if()
  {
  }


  template <typename G, typename S, typename I>
  inline
  graph_elt_neighborhood_if<G,S,I>::graph_elt_neighborhood_if(const Image<I>& mask)
  {
    this->hook_win_().change_mask(mask);
  }


  template <typename G, typename S, typename I>
  inline
  const I&
  graph_elt_neighborhood_if<G,S,I>::mask() const
  {
    return this->hook_win_().mask();
  }


# endif // ! MLN_INCLUDE_ONLY

}

#endif // ! MLN_CORE_IMAGE_GRAPH_ELT_NEIGHBORHOOD_IF_HH
