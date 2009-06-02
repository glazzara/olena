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

#ifndef MLN_CANVAS_BROWSING_INTERNAL_GRAPH_FIRST_SEARCH_HH
# define MLN_CANVAS_BROWSING_INTERNAL_GRAPH_FIRST_SEARCH_HH

/// \file mln/canvas/browsing/internal/graph_first_search.hh
///
/// \brief Depth-limited search algorithm for graph.  Browse over all
/// vertices for each component.
///
/// \todo Code "next(const S& stack)".

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

# include <deque>
# include <queue>
# include <stack>
# include <mln/core/concept/iterator.hh>
# include <mln/core/concept/browsing.hh>
# include <mln/core/concept/graph.hh>
# include <mln/util/vertex.hh>

namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      namespace internal
      {

        /// Search algorithm for graph.
        /// Browse over all vertices for each component.
        template <typename E,
                  template <typename T, typename Sequence = std::deque<T> >
                  class C>
        class graph_first_search_t : public Browsing< E >
        {
          typedef C<util::vertex_id_t> container_t;
        public:
          template <typename G, typename F>
          void operator()(const Graph<G>&, F& f) const;
        };



# ifndef MLN_INCLUDE_ONLY


	/// Call the correct member on the container.

	template <typename T>
	inline
	util::vertex_id_t
	next(const std::queue<T>& queue)
	{
	  return queue.front();
	}

	template <typename T>
	inline
	util::vertex_id_t
	next(const std::stack<T>& stack)
	{
	  return stack.top();
	}

	template <typename S>
	inline
	util::vertex_id_t
	next(const S& stack)
	{
	  mln_assertion(0);
	  /// Not defined yet!
	  // mlc_abort(S)::check();
	  return 0;
	}




        template <typename E,
                  template <typename T, typename Sequence = std::deque<T> >
                  class C>
        template <typename G, typename F>
        inline
        void
        graph_first_search_t<E, C>::operator()(const Graph<G>& g_, F& f) const
        {
          trace::entering("canvas::browsing::internal::graph_first_search");

          const G& g = exact(g_);
          mln_precondition(g.is_valid());

          f.init(g); // <--- init

          mln_vertex_iter(G) v(g);
          for_all(v)
            if (f.to_be_treated(v.id())) // <--- to_be_treated
            {
              container_t queue;
              queue.push(v.id());
              f.new_component_from_vertex(v.id()); // <--- new_component_from_vertex
              while (!queue.empty())
              {
                util::vertex<G> current_v = g.vertex(next(queue));
                f.process_vertex(current_v.id()); // <--- process_vertex
                queue.pop();
                for (unsigned nv = 0; nv < current_v.nmax_nbh_vertices(); ++nv)
                  if (f.to_be_queued(current_v.ith_nbh_vertex(nv))) // <--- to_be_queued
                  {
                    f.added_to_queue(current_v.ith_nbh_vertex(nv)); // <--- added_to_queue
                    queue.push(current_v.ith_nbh_vertex(nv));
                  }
              }
              f.next_component(); // <-- next_component
            }
          f.final(); // <-- final

          trace::exiting("canvas::browsing::internal::graph_first_search");
        }

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::canvas::browsing::internal

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_BROWSING_INTERNAL_GRAPH_FIRST_SEARCH_HH
