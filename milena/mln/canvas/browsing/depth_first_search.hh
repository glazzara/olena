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

#ifndef MLN_CANVAS_BROWSING_DEPTH_FIRST_SEARCH_HH
# define MLN_CANVAS_BROWSING_DEPTH_FIRST_SEARCH_HH

/// \file mln/canvas/browsing/depth_first_search.hh
///
/// Depth-limited search algorithm for graph.
/// Browse over all vertices for each component.

/*!
** The functor should provide the following methods:
**
**    - template <typename G> void init(const Graph<G>& g)
**	  Will be called at the beginning.
**
**    - void next()
**	  Will be called after all vertices from a component have been treated.
**
**    - void update_treated(unsigned id)
**	  will be called for the first vertex encountered for each component.
**
**    - void update_queued(unsigned id)
**	  Will be called for every vertex encountered in each component, except
**	  the first one.
**
**    - bool to_be_treated(unsigned id)
**	  Return whether this vertex has already been marked or if it may be a
**	  a component representative.
**
**    - bool to_be_queued(unsigned id)
**	  Return whether this vertex has already been marked or if it can be added
**	  to the current component.
**
**    - void final()
**	  Will be called at the end;
**
*/

# include <mln/core/concept/graph.hh>

namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      struct depth_first_search_t : public Browsing< depth_first_search_t >
      {
	template <typename G, typename F>
	void operator()(const Graph<G>&, F& f) const;
      };

      extern const depth_first_search_t depth_first_search;

# ifndef MLN_INCLUDE_ONLY

      const depth_first_search_t depth_first_search;

      template <typename G, typename F>
      inline
      void
      depth_first_search_t::operator()(const Graph<G>& g_, F& f) const
      {
	trace::entering("canvas::browsing::depth_first_search");

	const G& g = exact(g_);

	f.init(g);

	mln_vertex_iter(util::graph) v(g);
	for_all(v)
	  if (f.to_be_treated(v.id()))
	  {
	    std::queue<unsigned> queue;
	    queue.push(v.id());
	    f.update_treated(v.id());
	    while (!queue.empty())
	    {
	      util::vertex<util::graph> current_v = g.vertex(queue.front());
	      queue.pop();
	      for (unsigned nv = 0; nv < current_v.nmax_nbh_vertices(); ++nv)
		if (f.to_be_queued(current_v.ith_nbh_vertex(nv)))
		{
		  f.update_queued(current_v.ith_nbh_vertex(nv));
		  queue.push(current_v.ith_nbh_vertex(nv));
		}
	    }
	    f.next();
	  }

	f.final();

	trace::exiting("canvas::browsing::depth_first_search");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! MLN_CANVAS_BROWSING_DEPTH_FIRST_SEARCH_HH
