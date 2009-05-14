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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CANVAS_BROWSING_BREADTH_FIRST_SEARCH_HH
# define MLN_CANVAS_BROWSING_BREADTH_FIRST_SEARCH_HH

/// \file mln/canvas/browsing/breadth_first_search.hh
///
/// Breadth first search algorithm for graph.
/// Browse over all vertices for each component.

/*!
** The functor should provide the following methods:
**
**    - template <typename G> void init(const Graph<G>& g)
**	  Will be called at the beginning.
**
**    - bool to_be_treated(unsigned id)
**	  Return whether this vertex has already been marked or if it may be a
**	  a component representative.
**
**    - void new_component_from_vertex(unsigned id)
**	  will be called for the first vertex encountered for each component.
**
**    - void process_vertex(unsigned id)
**	  Will be called for each vertex queued.
**
**    - bool to_be_queued(unsigned id)
**	  Return whether this vertex has already been marked or if it can be added
**	  to the current component.
**
**    - void added_to_queue(unsigned id)
**	  Will be called for every vertex encountered in each component, except
**	  the first one.
**
**    - void next_component()
**	  Will be called after all vertices from a component have been treated.
**
**    - void final()
**	  Will be called at the end;
**
*/

# include <queue>
# include <mln/canvas/browsing/internal/graph_first_search.hh>

namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      /// Breadth-first search algorithm for graph, on vertices.
      struct breadth_first_search_t :
        public internal::graph_first_search_t<breadth_first_search_t, std::queue>
      {};

      extern const breadth_first_search_t breadth_first_search;



# ifndef MLN_INCLUDE_ONLY

      const breadth_first_search_t breadth_first_search;

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_BROWSING_BREADTH_FIRST_SEARCH_HH
