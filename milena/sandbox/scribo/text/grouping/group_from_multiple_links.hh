// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_TEXT_GROUPING_GROUP_FROM_MULTIPLE_LINKS_HH
# define SCRIBO_TEXT_GROUPING_GROUP_FROM_MULTIPLE_LINKS_HH

/// \file scribo/text/grouping/group_from_multiple_links.hh
///
/// Link text bounding boxes with their neighbors.

# include <mln/core/concept/graph.hh>
# include <mln/core/concept/image.hh>
# include <mln/core/site_set/box.hh>

# include <mln/canvas/browsing/depth_first_search.hh>

# include <mln/accu/bbox.hh>

# include <mln/pw/all.hh>

# include <mln/fun/i2v/array.hh>

# include <mln/util/array.hh>

# include <scribo/core/macros.hh>

namespace scribo
{

  namespace text
  {

    namespace grouping
    {

      /// FIXME: Add much more doc!
      template <typename G, typename P>
      util::array<box<P> >
      group_from_multiple_links(util::array<box<P> >& textbboxes,
				const Graph<G>& g_);

# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	/// Functor to be passed to depth_first_search.
	/// Map each component vertex with its representative vertex id.
	struct map_vertex_to_component_id_functor
	{
	    template <typename G>
	    void init(const Graph<G>& g)
	    {
	      vertextocomp.resize(exact(g).v_nmax(), mln_max(unsigned));
	      ncomp = 0;
	    }

	  void final()
	  {}

	  void next_component()
	  { ++ncomp; }

	  void new_component_from_vertex(unsigned id)
	  { vertextocomp(id) = ncomp; }

	  void added_to_queue(unsigned id)
	  { new_component_from_vertex(id); }

	  void process_vertex(unsigned)
	  {}

	  bool to_be_treated(unsigned id)
	  { return vertextocomp(id) == mln_max(unsigned); }

	  bool to_be_queued(unsigned id)
	  { return to_be_treated(id); }

	  unsigned ncomp;
	  fun::i2v::array<unsigned> vertextocomp;
	};

      } // end of namespace scribo::text::grouping::internal



      template <typename G, typename P>
      inline
      util::array<box<P> >
      group_from_multiple_links(util::array<box<P> >& textbboxes,
				const Graph<G>& g_)
      {
	trace::entering("scribo::text::grouping::group_from_multiple_links");

	const G& g = exact(g_);

	mln_assertion(g.is_valid());

	internal::map_vertex_to_component_id_functor f;
	canvas::browsing::depth_first_search(g, f);

	util::array< accu::bbox<P> > tboxes;
	tboxes.resize(textbboxes.nelements());
	for_all_components(i, textbboxes)
	  tboxes[f.vertextocomp(i)].take(textbboxes[i]);

	util::array< box<P> > result;
	// Component 0 - the background has not valid bboxes.
	result.append(box<P>());
	for_all_components(i, textbboxes)
	  if (tboxes[i].is_valid())
	    result.append(tboxes[i].to_result());

	mln_assertion(result.nelements() == f.ncomp);

	trace::exiting("scribo::text::grouping::group_from_multiple_links");
	return result;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::text::grouping

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_GROUPING_GROUP_FROM_MULTIPLE_LINKS_HH
