// Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
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

#ifndef MLN_CANVAS_BROWSING_DEPTH_FIRST_SEARCH_HH
# define MLN_CANVAS_BROWSING_DEPTH_FIRST_SEARCH_HH

/// \file
///
/// Depth-limited search algorithm for graph.
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

# include <stack>
# include <mln/canvas/browsing/internal/graph_first_search.hh>

namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      /// Breadth-first search algorithm for graph, on vertices.
      struct depth_first_search_t :
        public internal::graph_first_search_t<depth_first_search_t, std::stack>
      {
	// This default constructor is needed for compilation with gcc
	// 4.6.0, gcc 4.6.1 and Clang.
	depth_first_search_t();
      };

      extern const depth_first_search_t depth_first_search;



# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

      const depth_first_search_t depth_first_search;

#  endif // ! MLN_WO_GLOBAL_VARS

      depth_first_search_t::depth_first_search_t()
      {
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_BROWSING_DEPTH_FIRST_SEARCH_HH
