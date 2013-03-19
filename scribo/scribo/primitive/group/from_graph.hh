// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_GROUP_FROM_GRAPH_HH
# define SCRIBO_PRIMITIVE_GROUP_FROM_GRAPH_HH

/// \file
///
/// Group objects according to a graph of links


# include <mln/core/concept/graph.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>

# include <mln/canvas/browsing/depth_first_search.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/pw/all.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/util/array.hh>
# include <mln/util/graph_ids.hh>

# include <scribo/core/object_links.hh>
# include <scribo/core/object_groups.hh>
# include <scribo/core/macros.hh>


namespace scribo
{

  namespace primitive
  {

    namespace group
    {

      using namespace mln;

      /// Group objects according to a graph of links
      ///
      /// \param[in] comps    A component set.
      /// \param[in] g_	      The graph of object links.
      ///
      /// \return Object groups information.
      //
      template <typename L, typename G>
      object_groups<L>
      from_graph(const component_set<L>& comps,
		 const Graph<G>& g_);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	/// Functor to be passed to depth_first_search.
	/// Map each component vertex with its representative vertex id.
	template <typename L>
	struct map_vertex_to_representative_id_functor
	{

	  map_vertex_to_representative_id_functor(const component_set<L>& comps)
	    : vertextorep(comps)
	  {
	  }

	  /// Initialize the functor.
	  template <typename G>
	  void init(const Graph<G>& g)
	  {
	    vertextorep.resize(exact(g).v_nmax(), mln_max(mln_value(L)));
	  }

	  /// All components have been processed.
	  void final()
	  {}

	  /// Starting to treat a new component.
	  void next_component()
	  {}

	  /// A vertex with id \p id has been found in the current
	  /// component.
	  void new_component_from_vertex(const mln::util::vertex_id_t& id)
	  {
	    current_ = id;
	    vertextorep(id) = id;
	  }

	  /// A new neighbor with id \p id have been found for the
	  /// current vertex.
	  void added_to_queue(const mln::util::vertex_id_t&)
	  {}

	  /// Starting to process a new vertex.
	  void process_vertex(const mln::util::vertex_id_t& id)
	  {
	    vertextorep(id) = current_;
	  }

	  /// Does the vertex with id \p id have been already treated?
	  bool to_be_treated(const mln::util::vertex_id_t& id)
	  { return vertextorep(id) == mln_max(mln_value(L)); }

	  /// Does the neighbor vertex \p id have been already treated?
	  bool to_be_queued(const mln::util::vertex_id_t& id)
	  { return to_be_treated(id); }

	  /// The number of components found.
	  mln::util::vertex_id_t current_;

	  /// Map a verted id to its component id.
	  object_links<L> vertextorep;
	};

      } // end of namespace scribo::primitive::group::internal



      template <typename L, typename G>
      inline
      object_groups<L>
      from_graph(const component_set<L>& comps,
		 const Graph<G>& g_)
      {
	mln_trace("scribo::primitive::group::from_graph");

	const G& g = exact(g_);

	mln_assertion(g.is_valid());

	internal::map_vertex_to_representative_id_functor<L> f(comps);
	canvas::browsing::depth_first_search(g, f);

	object_groups<L> groups(comps);
	groups.init_(f.vertextorep);

	return groups;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::group

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_GROUP_FROM_GRAPH_HH
