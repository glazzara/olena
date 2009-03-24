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


#ifndef SCRIBO_DRAW_BOUNDING_BOX_LINKS_HH
# define SCRIBO_DRAW_BOUNDING_BOX_LINKS_HH

/// \file scribo/draw/bounding_boxes.hh
///
/// Draw a list of bounding box links.

# include <mln/core/concept/image.hh>
# include <mln/draw/line.hh>
# include <mln/util/array.hh>
# include <mln/canvas/browsing/depth_first_search.hh>

# include <scribo/core/macros.hh>

namespace scribo
{

  namespace draw
  {

    using namespace mln;

    /// Draw a list of bounding box links.
    template <typename I>
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const mln::util::array<unsigned>& link_array,
		       const mln_value(I)& value);

    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array<mln_site(I)::vec>& mass_centers,
		       const mln::util::array<unsigned>& link_array,
		       const mln_value(I)& value);

    template <typename I>
    void
    bounding_box_links(Image<I>& input,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& value);

    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array<mln_site(I)::vec>& mass_centers,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& value);

    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& left_link_value,
		       const mln_value(I)& right_link_value,
		       const mln_value(I)& validated_link_value);

    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array<mln_site(I)::vec>& mass_centers,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& left_link_value,
		       const mln_value(I)& right_link_value,
		       const mln_value(I)& validated_link_value);

    template <typename I, typename G>
    inline
    void
    bounding_box_links(Image<I>& input,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const Graph<G>& g,
		       const mln_value(I)& link_value);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {
      /// Functor to be passed to depth_first_search.
      template <typename I>
      struct draw_graph_edges_functor
      {

	draw_graph_edges_functor(I& ima,
				 const mln::util::array<box<mln_site(I)> >& textbboxes,
				 const mln_value(I)& value)
	  : ima_(ima), textbboxes_(textbboxes), value_(value)
	{}

	template <typename G>
	void
	init(const Graph<G>& g)
	{ deja_vu.resize(exact(g).v_nmax(), false); }

	void final()
	{}

	void next_component()
	{}

	void new_component_from_vertex(unsigned)
	{}

	void added_to_queue(unsigned id)
	{
	  deja_vu[id] = true;
	  mln::draw::line(ima_, textbboxes_[current_vertex].center(),
	      textbboxes_[id].center(), value_);
	}

	void process_vertex(unsigned id)
	{ current_vertex = id; }

	bool to_be_treated(unsigned id)
	{ return ! deja_vu[id]; }

	bool to_be_queued(unsigned id)
	{ return to_be_treated(id); }

	I& ima_;
	const mln::util::array<box<mln_site(I)> >& textbboxes_;
	mln_value(I) value_;
	unsigned current_vertex;
	std::vector<bool> deja_vu;
      };

    } // end of namespace scribo::text::grouping::internal


    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const mln::util::array<unsigned>& link_array,
		       const mln_value(I)& value)
    {
      trace::entering("scribo::draw::bounding_box_links");

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      for_all_components(i, link_array)
	if (link_array[i] != i)
	  mln::draw::line(input,
			  bboxes[i].center(),
			  bboxes[link_array[i]].center(),
			  value);

      trace::exiting("scribo::draw::bounding_box_links");
    }

    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array<mln_site(I)::vec>& mass_centers,
		       const mln::util::array<unsigned>& link_array,
		       const mln_value(I)& value)
    {
      trace::entering("scribo::draw::bounding_box_links");

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      for_all_components(i, link_array)
      {
	if (link_array[i] != i)
	  mln::draw::line(input,
			  mass_centers[i],
			  mass_centers[link_array[i]],
			  value);
	input(mass_centers[i]) = value;
      }

      trace::exiting("scribo::draw::bounding_box_links");
    }


    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& value)
    {
      trace::entering("scribo::draw::bounding_box_links");
      mln_precondition(exact(input).is_valid());

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      for_all_components(i, left_link)
      {
	  unsigned nbh = right_link[left_link[i]];
	  if (nbh == i && left_link[i] != i)
	    mln::draw::line(input,
			    bboxes[i].center(),
			    bboxes[left_link[i]].center(),
			    value);
      }

      trace::exiting("scribo::draw::bounding_box_links");
    }

    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array<mln_site(I)::vec>& mass_centers,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& value)
    {
      trace::entering("scribo::draw::bounding_box_links");
      mln_precondition(exact(input).is_valid());

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      for_all_components(i, left_link)
      {
	unsigned nbh = right_link[left_link[i]];
	if (nbh == i && left_link[i] != i)
	  mln::draw::line(input,
			  mass_centers[i],
			  mass_centers[left_link[i]],
			  value);
	input(mass_centers[i]) = value;
      }

      trace::exiting("scribo::draw::bounding_box_links");
    }


    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array<mln_site(I)::vec>& mass_centers,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& left_link_value,
		       const mln_value(I)& right_link_value,
		       const mln_value(I)& validated_link_value)
    {
      trace::entering("scribo::draw::bounding_box_links");
      mln_precondition(exact(input).is_valid());

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      mln_site(I)::vec dleft = literal::origin;
      dleft[0] = 2;
      mln_site(I)::vec dright = literal::origin;
      dright[0] = -2;
      for_all_components(i, left_link)
      {
	mln::draw::line(input,
			mass_centers[i] + dleft,
			mass_centers[left_link[i]] + dleft,
			left_link_value);

	mln::draw::line(input,
			mass_centers[i] + dright,
			mass_centers[right_link[i]] + dright,
			right_link_value);

	unsigned nbh = right_link[left_link[i]];
	if (nbh == i && left_link[i] != i)
	  mln::draw::line(input,
			  mass_centers[i],
			  mass_centers[left_link[i]],
			  validated_link_value);

	input(mass_centers[i]) = validated_link_value;
      }

      trace::exiting("scribo::draw::bounding_box_links");
    }

    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& left_link_value,
		       const mln_value(I)& right_link_value,
		       const mln_value(I)& validated_link_value)
    {
      trace::entering("scribo::draw::bounding_box_links");
      mln_precondition(exact(input).is_valid());

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      mln_site(I)::vec dleft = literal::origin;
      dleft[0] = 2;
      mln_site(I)::vec dright = literal::origin;
      dright[0] = -2;
      for_all_components(i, left_link)
      {
	mln::draw::line(input,
			bboxes[i].center() + dleft,
			bboxes[left_link[i]].center() + dleft,
			left_link_value);

	mln::draw::line(input,
			bboxes[i].center() + dright,
			bboxes[right_link[i]].center() + dright,
			right_link_value);

	unsigned nbh = right_link[left_link[i]];
	if (nbh == i && left_link[i] != i)
	  mln::draw::line(input,
			  bboxes[i].center(),
			  bboxes[left_link[i]].center(),
			  validated_link_value);
      }

      trace::exiting("scribo::draw::bounding_box_links");
    }


    template <typename I, typename G>
    inline
    void
    bounding_box_links(Image<I>& input,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const Graph<G>& g,
		       const mln_value(I)& link_value)
    {
      trace::entering("scribo::draw::bounding_box_links");
      mln_precondition(exact(input).is_valid());
      mln_precondition(exact(g).v_nmax() == bboxes.nelements());

      internal::draw_graph_edges_functor<I> f(exact(input), bboxes, link_value);
      canvas::browsing::depth_first_search(g, f);

      trace::exiting("scribo::draw::bounding_box_links");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::draw

} // end of namespace scribo


#endif // ! SCRIBO_DRAW_BOUNDING_BOX_LINKS_HH
