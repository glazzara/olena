// Copyright (C) 2008, 2009, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_INTERNAL_GRAPH_WINDOW_BASE_HH
# define MLN_CORE_INTERNAL_GRAPH_WINDOW_BASE_HH

# include <mln/core/concept/window.hh>

/// \file
///
/// FIXME: doc

namespace mln
{

  /*!
    \internal
    \brief Base class for windows on graphes.

    \tparam P Site type.
  */
  template <typename P, typename E>
  class graph_window_base : public Window<E>
  {
    typedef graph_window_base<P, E> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of site corresponding to the window.
    typedef P site;

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

    /// Is the window neighbable?
    bool is_neighbable_() const;
    /// \}

    /// Return true by default.
    bool is_valid() const;

  protected:
    graph_window_base();
  };


# ifndef MLN_INCLUDE_ONLY



  template <typename P, typename E>
  inline
  graph_window_base<P,E>::graph_window_base()
  {
  }

  template <typename P, typename E>
  inline
  bool
  graph_window_base<P,E>::is_empty() const
  {
    return false;
  }

  template <typename P, typename E>
  inline
  bool
  graph_window_base<P,E>::is_centered() const
  {
    return false;
  }

  template <typename P, typename E>
  inline
  bool
  graph_window_base<P,E>::is_symmetric() const
  {
    return true;
  }

  template <typename P, typename E>
  inline
  unsigned
  graph_window_base<P,E>::delta() const
  {
    // Dummy value (see the interface of the method above).
    return 0;
  }

  template <typename P, typename E>
  inline
  graph_window_base<P,E>&
  graph_window_base<P,E>::sym()
  {
    return *this;
  }

  template <typename P, typename E>
  inline
  bool
  graph_window_base<P,E>::is_neighbable_() const
  {
    return true;
  }

  template <typename P, typename E>
  inline
  bool
  graph_window_base<P,E>::is_valid() const
  {
    return true;
  }



# endif // !MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // ! MLN_CORE_INTERNAL_GRAPH_WINDOW_BASE_HH
