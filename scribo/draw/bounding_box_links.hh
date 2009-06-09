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
# include <scribo/text/grouping/internal/is_link_valid.hh>

namespace scribo
{

  namespace draw
  {

    using namespace mln;

    /// Draw a list of bounding box links. Draw links from the bounding
    /// box centers.
    ///
    /// \param[in,out] input_ An image where to draw.
    /// \param[in] bboxes Bounding boxes.
    /// \param[in] link_array Bounding box links.
    /// \param[in] value Value used to draw links.
    template <typename I>
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const mln::util::array<unsigned>& link_array,
		       const mln_value(I)& value);


    /// Draw a list of bounding box links from their mass centers.
    ///
    /// \param[in,out] input_ An image where to draw.
    /// \param[in] mass_centers Bounding boxes mass centers.
    /// \param[in] link_array Bounding box links.
    /// \param[in] value Value used to draw links.
    template <typename I>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array<mln_site(I)::vec>& mass_centers,
		       const mln::util::array<unsigned>& link_array,
		       const mln_value(I)& value);


    /// Draw left, right and validated lists of bounding box links.
    /// Draw from the bounding box centers.
    ///
    /// \param[in,out] input_ An image where to draw.
    /// \param[in] bboxes Bounding boxes.
    /// \param[in] left_link Bounding box left links.
    /// \param[in] right_link Bounding box right links.
    /// \param[in] left_link_value Value used to draw left links.
    /// \param[in] right_link_value Value used to draw right links.
    /// \param[in] validated_link_value Value used to draw validated links.
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


    /// Draw left, right and validated lists of bounding box links.
    /// Draw from the bounding box mass centers.
    ///
    /// \param[in,out] input_ An image where to draw.
    /// \param[in] mass_centers Bounding box mass centers.
    /// \param[in] left_link Bounding box left links.
    /// \param[in] right_link Bounding box right links.
    /// \param[in] left_link_value Value used to draw left links.
    /// \param[in] right_link_value Value used to draw right links.
    /// \param[in] validated_link_value Value used to draw validated links.
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


    /// Draw a graph of bounding box links.
    /// Draw from bounding box centers.
    ///
    /// \param[in,out] input_ An image where to draw.
    /// \param[in] bboxes Bounding boxes.
    /// \param[in] g_ The link graph.
    /// \param[in] link_value The value used to draw the links.
    template <typename I, typename G>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const mln::util::array< box<mln_site(I)> >& bboxes,
		       const Graph<G>& g_,
		       const mln_value(I)& link_value);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      /// Draw a link graph in an image.
      /// Functor to be passed to depth_first_search.
      template <typename I>
      struct draw_graph_edges_functor
      {

	/// Constructor
	///
	/// \param[in,out] ima The image where to draw.
	/// \param[in] textbboxes The line of text bounding boxes.
	/// \param[in] value The value used to draw the links.
	draw_graph_edges_functor(I& ima,
				 const mln::util::array<box<mln_site(I)> >& textbboxes,
				 const mln_value(I)& value)
	  : ima_(ima), textbboxes_(textbboxes), value_(value)
	{}

	/// Initialize the functor.
	///
	/// \param[in] g The graph this functor will work on.
	template <typename G>
	void
	init(const Graph<G>& g)
	{ deja_vu.resize(exact(g).v_nmax(), false); }

	/// All components/vertices have been treated.
	void final()
	{}

	/// Starting to process a new component.
	void next_component()
	{}

	/// A new component have been found through the vertex with
	/// the given id.
	void new_component_from_vertex(unsigned)
	{}


	/// A new vertex neighbor have been found and will be treated.
	void added_to_queue(unsigned id)
	{
	  deja_vu[id] = true;
	  mln::draw::line(ima_, textbboxes_[current_vertex].center(),
	      textbboxes_[id].center(), value_);
	}

	/// Treating a new vertex.
	void process_vertex(unsigned id)
	{ current_vertex = id; }

	/// Does this vertex with id \p id have to be treated?
	bool to_be_treated(unsigned id)
	{ return ! deja_vu[id]; }

	/// Does this vertex neighbor have to be treated?
	bool to_be_queued(unsigned id)
	{ return to_be_treated(id); }

	/// The image where to draw.
	I& ima_;

	/// Line of text bounding boxes.
	const mln::util::array<box<mln_site(I)> >& textbboxes_;

	/// Value to be used to draw links.
	mln_value(I) value_;

	/// Current vertex being processed.
	unsigned current_vertex;

	/// Store whether a vertex has been already seen or not.
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
		       const mln::util::array<mln_site(I)::vec>& mass_centers,
		       const mln::util::array<unsigned>& left_link,
		       const mln::util::array<unsigned>& right_link,
		       const mln_value(I)& left_link_value,
		       const mln_value(I)& right_link_value,
		       const mln_value(I)& validated_link_value)
    {
      trace::entering("scribo::draw::bounding_box_links");

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

	mln::util::couple<bool, unsigned>
	  nbh = text::grouping::internal::is_link_valid(left_link, right_link, i);

	if (nbh.first())
	  mln::draw::line(input,
			  mass_centers[i],
			  mass_centers[nbh.second()],
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

	mln::util::couple<bool, unsigned>
	  nbh = text::grouping::internal::is_link_valid(left_link, right_link, i);

	if (nbh.first())
	  mln::draw::line(input,
			  bboxes[i].center(),
			  bboxes[nbh.second()].center(),
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
