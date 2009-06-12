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

#ifndef SCRIBO_TEXT_GROUPING_GROUP_FROM_GRAPH_HH
# define SCRIBO_TEXT_GROUPING_GROUP_FROM_GRAPH_HH

/// \file
///
/// Group lines of text according to a graph of links between
/// the lines of text.

# include <mln/core/concept/graph.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>

# include <mln/canvas/browsing/depth_first_search.hh>

# include <mln/accu/shape/bbox.hh>

# include <mln/pw/all.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/util/array.hh>
# include <mln/util/graph_ids.hh>

# include <scribo/core/object_image.hh>
# include <scribo/core/macros.hh>


namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      using namespace mln;

      /// Group lines of text according to a graph of links between
      /// the lines of text.
      ///
      /// \param[in] text  An object image.
      /// \param[in] g_	  The graph of links between the lines of text.
      ///
      /// \return An image with grouped objects.
      //
      template <typename L, typename G>
      object_image(L)
      group_from_graph(const object_image(L)& text,
		       const Graph<G>& g_);


# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	/// Functor to be passed to depth_first_search.
	/// Map each component vertex with its representative vertex id.
	template <typename V>
	struct map_vertex_to_representative_id_functor
	{

	  /// Initialize the functor.
	  template <typename G>
	  void init(const Graph<G>& g)
	  {
	    vertextorep.resize(exact(g).v_nmax(), mln_max(V));
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
	  { return vertextorep(id) == mln_max(V); }

	  /// Does the neighbor vertex \p id have been already treated?
	  bool to_be_queued(const mln::util::vertex_id_t& id)
	  { return to_be_treated(id); }

	  /// The number of components found.
	  mln::util::vertex_id_t current_;

	  /// Map a verted id to its component id.
	  fun::i2v::array<V> vertextorep;
	};

      } // end of namespace scribo::text::grouping::internal



      template <typename L, typename G>
      inline
      object_image(L)
      group_from_graph(const object_image(L)& text,
		       const Graph<G>& g_)
      {
	trace::entering("scribo::text::grouping::group_from_graph");

	const G& g = exact(g_);

	mln_assertion(g.is_valid());

	internal::map_vertex_to_representative_id_functor<mln_value(L)> f;
	canvas::browsing::depth_first_search(g, f);

	object_image(L) output;
	output.init_from_(text);
	output.relabel(f.vertextorep);

	trace::exiting("scribo::text::grouping::group_from_graph");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_FROM_GRAPH_HH
