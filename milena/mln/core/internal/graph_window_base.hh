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

#ifndef MLN_CORE_INTERNAL_GRAPH_WINDOW_BASE_HH
# define MLN_CORE_INTERNAL_GRAPH_WINDOW_BASE_HH

/// \file mln/internal/graph_window_base.hh
///
/// FIXME: doc

namespace mln
{

  template <typename G, typename F, typename P, typename E>
  class graph_window_base : public Window< E >
  {
    typedef graph_window_base<G, F, P, E> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of site corresponding to the window.
    typedef mln_site(P) site;

    // The type of the set of window sites (ids adjacent to the
    // reference psite).
    typedef std::set<unsigned> sites_t;

    // FIXME: This is a dummy value.
    typedef void dpsite;
    /// \}

    /// Interface of the concept Window.
    /// \{
    /// Is the window is empty?
    bool is_empty() const;
    /// Is the window centered?
    bool is_centered() const;
    /// Is the window symmetric?
    // FIXME: We should define this more precisely.
    bool is_symmetric() const;
    /// Return the maximum coordinate gap between the window center
    /// and a window point.
    /* FIXME: This method returns a dummy value (0), since the delta
       of a window on a graph/line_graph

       1. is not constant (line graph edges/graph vertices are not necessarily
          aligned on a regular grid);

       2. depends on the underlying line_graph/graph, too.

       It raises another question: should delta() be part of the
       concept ``Window''?  */
    unsigned delta() const;
    /// Apply a central symmetry to the target window.
    self_& sym();
    /// \}

  protected:
    graph_window_base();
  };

} // end of namespace mln

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  template <typename G, typename F, typename P, typename E>
  inline
  graph_window_base<G, F, P, E>::graph_window_base()
  {
  }

  template <typename G, typename F, typename P, typename E>
  inline
  bool
  graph_window_base<G, F, P, E>::is_empty() const
  {
    return false;
  }

  template <typename G, typename F, typename P, typename E>
  inline
  bool
  graph_window_base<G, F, P, E>::is_centered() const
  {
    return false;
  }

  template <typename G, typename F, typename P, typename E>
  inline
  bool
  graph_window_base<G, F, P, E>::is_symmetric() const
  {
    return true;
  }

  template <typename G, typename F, typename P, typename E>
  inline
  unsigned
  graph_window_base<G, F, P, E>::delta() const
  {
    // Dummy value (see the interface of the method above).
    return 0;
  }

  template <typename G, typename F, typename P, typename E>
  inline
  graph_window_base<G, F, P, E>&
  graph_window_base<G, F, P, E>::sym()
  {
    return *this;
  }

} // end of namespace mln

# endif // !MLN_INCLUDE_ONLY

#endif // !MLN_CORE_INTERNAL_GRAPH_WINDOW_BASE_HH

