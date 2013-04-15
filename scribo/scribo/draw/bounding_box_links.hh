// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
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

#ifndef SCRIBO_DRAW_BOUNDING_BOX_LINKS_HH
# define SCRIBO_DRAW_BOUNDING_BOX_LINKS_HH

/// \file
///
/// Draw a list of bounding box links.

# include <mln/core/concept/image.hh>
# include <mln/draw/line.hh>
# include <mln/util/array.hh>
# include <mln/canvas/browsing/depth_first_search.hh>

# include <scribo/core/tag/anchor.hh>
# include <scribo/core/macros.hh>
# include <scribo/core/object_links.hh>
# include <scribo/primitive/internal/is_link_valid.hh>
# include <scribo/primitive/link/internal/compute_anchor.hh>

namespace scribo
{

  namespace draw
  {

    using namespace mln;

    /// \brief Draw a list of bounding box links.
    ///
    /// \param[in,out] input An image where to draw.
    /// \param[in] link component links.
    /// \param[in] value Value used to draw links.
    /// \param[in] anchor Anchor from where the links are drawn.
    //
    template <typename I, typename L>
    void
    bounding_box_links(Image<I>& input,
		       const object_links<L>& link,
		       const mln_value(I)& value,
		       anchor::Type anchor);


    /// \overload
    /// The default anchor type is set to anchor::Center.
    //
    template <typename I, typename L>
    void
    bounding_box_links(Image<I>& input,
		       const object_links<L>& link,
		       const mln_value(I)& value);


    /// \brief Draw left, right and validated lists of bounding box
    /// links.
    ///
    /// \param[in,out] input An image where to draw.
    /// \param[in] left_link Component's left links.
    /// \param[in] right_link Component's right links.
    /// \param[in] left_link_value Value used to draw left links.
    /// \param[in] right_link_value Value used to draw right links.
    /// \param[in] validated_link_value Value used to draw validated links.
    /// \param[in] anchor Anchor from where the links are drawn.
    //
    template <typename I, typename L>
    inline
    void
    bounding_box_links(Image<I>& input,
		       const object_links<L>& left_link,
		       const object_links<L>& right_link,
		       const mln_value(I)& left_link_value,
		       const mln_value(I)& right_link_value,
		       const mln_value(I)& validated_link_value,
		       anchor::Type anchor);


    /// \brief Draw a graph of bounding box links.
    /// Draw from bounding box centers.
    ///
    /// \param[in,out] input An image where to draw.
    /// \param[in] g The link graph.
    /// \param[in] link_value The value used to draw the links.
    //
    template <typename I, typename G>
    inline
    void
    bounding_box_links(Image<I>& input,
		       const Graph<G>& g,
		       const mln_value(I)& link_value);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {


      template <typename I>
      mln_site(I)
      shift_site(const I& ima,
		 const mln_site(I)& p, const mln_dpsite(mln_site_(I))& dp)
      {
	if (ima.has(p + dp))
	  return p + dp;
	return p;
      }

      /// Draw a link graph in an image.
      /// Functor to be passed to depth_first_search.
      template <typename I, typename G>
      struct draw_graph_edges_functor
      {

	/// Constructor
	///
	/// \param[in,out] ima The image where to draw.
	/// \param[in] value The value used to draw the links.
	draw_graph_edges_functor(I& ima,
				 const mln_value(I)& value)
	  : ima_(ima), value_(value)
	{}

	/// Initialize the functor.
	///
	/// \param[in] g The graph this functor will work on.
	void
	init(const Graph<G>& g)
	{
	  g_ = g;
	  deja_vu.resize(exact(g).v_nmax(), false);
	}

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
	  mln::draw::line(ima_, g_.components()(current_vertex).bbox().pcenter(),
			  g_.components()(id).bbox().pcenter(), value_);
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

	/// Value to be used to draw links.
	mln_value(I) value_;

	/// Current vertex being processed.
	unsigned current_vertex;

	/// Store whether a vertex has been already seen or not.
	std::vector<bool> deja_vu;

	G g_;
      };

    } // end of namespace scribo::draw::internal


    template <typename I, typename L>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const object_links<L>& links,
		       const mln_value(I)& value,
		       anchor::Type anchor)
    {
      mln_trace("scribo::draw::bounding_box_links");

      I& input = exact(input_);

      mln_precondition(input.is_valid());

      const component_set<L>& comps = links.components();
      for_all_links(i, links)
	if (links(i) != i && links(i) != 0)
	{
	  mln_site(L)
	    p1 = primitive::link::internal::compute_anchor(comps, i, anchor),
	    p2 = primitive::link::internal::compute_anchor(comps, links(i), anchor);

	  mln::draw::line(input, p1, p2, value);
	}

    }


    template <typename I, typename L>
    inline
    void
    bounding_box_links(Image<I>& input,
		       const object_links<L>& links,
		       const mln_value(I)& value)
    {
      return bounding_box_links(input, links, value, anchor::Center);
    }


    template <typename I, typename L>
    inline
    void
    bounding_box_links(Image<I>& input_,
		       const object_links<L>& left_link,
		       const object_links<L>& right_link,
		       const mln_value(I)& left_link_value,
		       const mln_value(I)& right_link_value,
		       const mln_value(I)& validated_link_value,
		       anchor::Type anchor)
    {
      mln_trace("scribo::draw::bounding_box_links");

      I& input = exact(input_);
      mln_precondition(input.is_valid());

      typedef mln_site(I) P;

      const component_set<L>& comps = left_link.components();

      mln_dpsite(P) dleft = literal::zero;
      dleft[0] = 2;
      mln_dpsite(P) dright = literal::zero;
      dright[0] = -2;
      for_all_links(i, left_link)
      {
	mln::draw::line(input,
			internal::shift_site(input,
					     primitive::link::internal::compute_anchor(comps, i, anchor),
					     dleft),
			internal::shift_site(input,
					     primitive::link::internal::compute_anchor(comps, left_link(i), anchor),
					     dleft),
			left_link_value);



	mln::draw::line(input,
			internal::shift_site(input,
					     primitive::link::internal::compute_anchor(comps, i, anchor),
					     dleft),
			internal::shift_site(input,
					     primitive::link::internal::compute_anchor(comps, right_link(i), anchor),
					     dleft),
			right_link_value);


	if (primitive::internal::is_link_valid(left_link,
					       right_link, i))
	  mln::draw::line(input,
			  primitive::link::internal::compute_anchor(comps, i, anchor),
			  primitive::link::internal::compute_anchor(comps, left_link(i), anchor),
			  validated_link_value);

      }

    }



    template <typename I, typename G>
    inline
    void
    bounding_box_links(Image<I>& input,
		       const Graph<G>& g,
		       const mln_value(I)& link_value)
    {
      mln_trace("scribo::draw::bounding_box_links");
      mln_precondition(exact(input).is_valid());


      internal::draw_graph_edges_functor<I,G> f(exact(input), link_value);
      mln::canvas::browsing::depth_first_search(g, f);

    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::draw

} // end of namespace scribo


#endif // ! SCRIBO_DRAW_BOUNDING_BOX_LINKS_HH
