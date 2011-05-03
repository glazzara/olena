// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_ALIGNMENTS_HH
# define SCRIBO_PRIMITIVE_EXTRACT_ALIGNMENTS_HH

# include <iostream>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/data/convert.hh>

# include <mln/value/rgb8.hh>
# include <mln/value/label_16.hh>
# include <mln/value/int_u.hh>
# include <mln/literal/colors.hh>

# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <mln/io/pbm/load.hh>

# include <mln/labeling/colorize.hh>

# include <mln/util/couple.hh>

# include <scribo/core/def/lbl_type.hh>
# include <scribo/primitive/extract/components.hh>
# include <scribo/filter/object_links_aligned.hh>
# include <scribo/filter/object_groups_small.hh>
# include <scribo/preprocessing/denoise_fg.hh>
# include <scribo/primitive/link/internal/link_single_dmax_ratio_aligned_delta_base.hh>
# include <scribo/primitive/link/internal/dmax_default.hh>
# include <scribo/primitive/link/internal/dmax_hrules.hh>
# include <scribo/primitive/link/merge_double_link_closest_aligned.hh>

# include <scribo/debug/usage.hh>
# include <scribo/debug/links_image.hh>


# include <scribo/core/document.hh>
# include <scribo/core/component_set.hh>
# include <scribo/core/line_set.hh>
# include <scribo/primitive/extract/components.hh>
# include <scribo/primitive/group/from_single_link.hh>
# include <scribo/primitive/group/apply.hh>
# include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
# include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>
# include <scribo/primitive/link/merge_double_link.hh>
# include <scribo/primitive/link/internal/dmax_width_and_height.hh>

# include <scribo/preprocessing/rotate_90.hh>
# include <scribo/filter/object_links_bbox_h_ratio.hh>

# include <scribo/primitive/extract/separators.hh>
# include <scribo/primitive/extract/vertical_separators.hh>
# include <scribo/primitive/extract/horizontal_separators.hh>
# include <scribo/primitive/extract/separators_nonvisible.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      /*! \brief Find page delimitors from tabstops and whitespaces.

	\pre Separators should be removed from input document image .

	\pre Text in \p doc must be constructed from Components
	grouped by lines with a very strict criterion in order to keep
	spaces between words and paragraphs. This first grouping is
	necessary to avoid false positive (e.g. inside the text
	blocks).

	\pre \p doc must have text (has_text() returns true).


	Internal description :
	----------------------

	1) Build an image of line bboxes
	2) For TOP and BOTTOM
	2.a) Link bboxes. Links are validated only if :
	- Alignment difference is less than \p delta_pixel.
	- Bboxes are not too far
	- No component is located at 5 pixels along the aligned side.
	2.b) Invalidates groups if there are less than 3 links
	2.c) Invalidates groups if a component is located at a specific
	     distance from the aligned side.

	This method handles skewed alignments and draw skew lines if
	possible. Examples :

	 |x              |x
	 | x              \x
	 |  x       ->     \x
	 |   x              \x


	 \fixme Because the first link step at step 2.a check aligment
	between only 2 components and not the whole group, we may
	retrieve alignment patterns such this one :

	|x
	| x
	|  x
	| x
	|x

	Here, the 'x' are aligned pair by pair but globally they are
	not. Here we cannot draw skewed lines without processing every
	links and look for that pattern.  More over, in step 2.c
	alignment is validated by looking for other components in the
	supposed "whitespace area" from the aligned side. With such an
	alignment there are more chances that a component intersect
	with that line.  Here, we would like to split links/alignement
	in two groups in order to get something like that :

	 \x
	  \x
	   \x
	  /x
	 /x



	\param[in] doc A document information with text lines.

	\param[in] dmax_ratio The ratio used to compute the maximum
	                      lookup distance while linking up components.

	\param[in] delta_pixel The maximum number of pixels allowed
	                       for alignment delta precision.

      */
      template <typename L>
      mln::util::couple<component_set<L>, mln_ch_value(L,bool)>
      alignments(const document<L>& doc,
		 float dmax_ratio, unsigned delta_pixel);



# ifndef MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive




  namespace internal
  {

    template <typename V>
    point2d
    point_row_adjust(const point2d& pi, const image2d<V>& ima)
    {
      if (ima.domain().has(pi))
	return pi;

      point2d po = pi;

      if (po.row() < ima.domain().pmin().row())
	while (!ima.domain().has(po))
	  ++po.row();
      else
	while (!ima.domain().has(po))
	  --po.row();

      return po;
    }

    template <typename V>
    point2d
    point_col_adjust(const point2d& pi, const image2d<V>& ima)
    {
      if (ima.domain().has(pi))
	return pi;

      point2d po = pi;

      if (po.col() < ima.domain().pmin().col())
	while (!ima.domain().has(po))
	  ++po.col();
      else
	while (!ima.domain().has(po))
	  --po.col();

      return po;
    }

  } // end of namespace scribo::internal




  namespace primitive
  {

    namespace link
    {

      namespace internal
      {

	template <typename L, typename F>
	class single_right_dmax_ratio_aligned_delta_functor
	  : public link_single_dmax_ratio_aligned_delta_base<L, F, single_right_dmax_ratio_aligned_delta_functor<L,F> >
	{
	  typedef single_right_dmax_ratio_aligned_delta_functor<L,F> self_t;
	  typedef link_single_dmax_ratio_aligned_delta_base<L, F, self_t> super_;

	public:
	  typedef mln_site(L) P;

	  single_right_dmax_ratio_aligned_delta_functor(
	    const component_set<L>& components,
	    const DMax_Functor<F>& dmax_f,
	    unsigned delta,
	    anchor::Direction delta_direction,
	    const L& bbox_ima,
	    unsigned delta_ws_lookup)
	    : super_(components, dmax_f, delta, delta_direction),
	      bbox_ima_(bbox_ima), delta_ws_lookup_(delta_ws_lookup)
	  {
	    debug_ = data::convert(value::rgb8(), data::convert(bool(), bbox_ima));
	  }

	  void compute_next_site_(P& p)
	  {
	    ++p.col();
	  }


	  inline
	  bool
	  valid_link_(unsigned current_object,
		      const P& start_point,
		      const P& p,
		      anchor::Type anchor)
	  {
	    if (!super_::valid_link_(current_object, start_point, p, anchor))
	      return false;

	    mln_value(L) nbh = this->labeled_image_(p);

	    switch(anchor)
	    {
	      // Top
	      case anchor::StrictTopCenter:
	      {
		point2d p(std::min(this->components_(current_object).bbox().pmin().row(),
				   this->components_(nbh).bbox().pmin().row()) - delta_ws_lookup_,
			  this->components_(current_object).bbox().pmin().col());
		p = scribo::internal::point_row_adjust(p, bbox_ima_);

		for (; p.col() <= this->components_(nbh).bbox().pmax().col()
		       && (bbox_ima_(p) == 0);)
		{
		  debug_(p) = literal::violet;
		  ++p.col();
		}

		return bbox_ima_(p) == 0;
	      }

	      // Bottom
	      case anchor::StrictBottomCenter:
	      {
		point2d p(std::max(this->components_(current_object).bbox().pmax().row(),
				   this->components_(nbh).bbox().pmax().row()) + delta_ws_lookup_,
			  this->components_(current_object).bbox().pmin().col());
		p = scribo::internal::point_row_adjust(p, bbox_ima_);

		for (; p.col() <= this->components_(nbh).bbox().pmax().col()
		       && (bbox_ima_(p) == 0);)
		{
		  debug_(p) = literal::violet;
		  ++p.col();
		}

		return bbox_ima_(p) == 0;
	      }

	      default:
		trace::warning("anchor not handled!");
	    }

	    return false;
	  }


	  L bbox_ima_;
	  unsigned delta_ws_lookup_;

	  image2d<value::rgb8> debug_;
	};


	template <typename L, typename F>
	class single_left_dmax_ratio_aligned_delta_functor
	  : public link_single_dmax_ratio_aligned_delta_base<L, F, single_left_dmax_ratio_aligned_delta_functor<L,F> >
	{
	  typedef single_left_dmax_ratio_aligned_delta_functor<L,F> self_t;
	  typedef link_single_dmax_ratio_aligned_delta_base<L, F, self_t> super_;

	public:
	  typedef mln_site(L) P;

	  single_left_dmax_ratio_aligned_delta_functor(
	    const component_set<L>& components,
	    const DMax_Functor<F>& dmax_f,
	    unsigned delta,
	    anchor::Direction delta_direction,
	    const L& bbox_ima,
	    unsigned delta_ws_lookup)
	    : super_(components, dmax_f, delta, delta_direction),
	      bbox_ima_(bbox_ima), delta_ws_lookup_(delta_ws_lookup)
	  {
	    debug_ = data::convert(value::rgb8(), data::convert(bool(), bbox_ima));
	  }

	  void compute_next_site_(P& p)
	  {
	    --p.col();
	  }


	  inline
	  bool
	  valid_link_(unsigned current_object,
		      const P& start_point,
		      const P& p,
		      anchor::Type anchor)
	  {
	    if (!super_::valid_link_(current_object, start_point, p, anchor))
	      return false;

	    mln_value(L) nbh = this->labeled_image_(p);

	    switch (anchor)
	    {

	      // Top
	      case anchor::StrictTopCenter:
	      {
		point2d p(std::min(this->components_(nbh).bbox().pmin().row(),
				   this->components_(current_object).bbox().pmin().row()) - delta_ws_lookup_,
			  this->components_(current_object).bbox().pmax().col());
		p = scribo::internal::point_row_adjust(p, bbox_ima_);

		for (; p.col() > this->components_(nbh).bbox().pmin().col()
		       && (bbox_ima_(p) == 0);)
		{
		  debug_(p) = literal::violet;
		  --p.col();
		}

		return bbox_ima_(p) == 0;
	      }

	      // Bottom
	      case anchor::StrictBottomCenter:
	      {
		point2d p(std::max(this->components_(nbh).bbox().pmax().row(),
				   this->components_(current_object).bbox().pmax().row()) + delta_ws_lookup_,
			  this->components_(current_object).bbox().pmax().col());
		p = scribo::internal::point_row_adjust(p, bbox_ima_);

		for (; p.col() > this->components_(nbh).bbox().pmin().col()
		       && (bbox_ima_(p) == 0);)
		{
		  debug_(p) = literal::violet;
		  --p.col();
		}

		return bbox_ima_(p) == 0;
	      }

	      default:
		trace::warning("anchor not handled!");
	    }

	    return false;
	  }


	  L bbox_ima_;
	  unsigned delta_ws_lookup_;

	  image2d<value::rgb8> debug_;
	};


      } // end of namespace scribo::primitive::link::internal

    } // end of namespace scribo::primitive::link


    namespace extract
    {

      namespace internal
      {

	template <typename L>
	bool pass_comp_criterion(const line_info<L>& line)
	{
	  return line.bbox().height() < 301;// (line.pixel_area() < 10000);
	}


	inline
	bool
	is_valid_left_right_skewed_delimitor(const box2d& brot,
					     const image2d<bool> input,
					     unsigned local_delta)
	{
	  if (brot.pmax().col() - brot.pmin().col() > 6)
	  {
	    point2d
	      p1 = brot.pmin(),
	      p2 = brot.pmax();

	    // Handle left to right alignment
	    p1.col() -= local_delta;
	    p2.col() -= local_delta;
	    p_line2d l2d(p1, p2);
	    mln_piter_(p_line2d) p(l2d);
	    for_all(p)
	      if (input(p))
		return false;

	    return true;
	  }

	  return false;
	}


	inline
	bool
	is_valid_right_left_skewed_delimitor(const box2d& brot,
					     const image2d<bool> input,
					     unsigned local_delta)
	{
	  if (brot.pmax().col() - brot.pmin().col() > 6)
	  {
	    point2d
	      p1 = brot.pmin(),
	      p2 = brot.pmax();

	    // Handle right to left alignment
	    p1.col() = brot.pmax().col() - local_delta;
	    p2.col() = brot.pmin().col() - local_delta;
	    p_line2d l2d(p1, p2);
	    mln_piter_(p_line2d) p(l2d);
	    for_all(p)
	      if (input(p))
		return false;

	    return true;
	  }

	  return false;
	}


	inline
	box2d fast_rotate_positive(const box2d& box, const box2d& rbox)
	{
	  box2d b(point2d(box.pmin().col(),
			  rbox.ncols() - box.pmin().row() - 1),
		  point2d(box.pmax().col(),
			  rbox.ncols() - box.pmax().row() - 1));
	  return b;
	}


	inline
	box2d fast_rotate_negative(const box2d& box, const box2d& rbox)
	{
	  mln::def::coord max_row = rbox.ncols() - box.pmin().col() - 1;
	  mln::def::coord min_row = rbox.ncols() - box.pmax().col() - 1;

	  box2d b(point2d(min_row,
			  box.pmin().row()),
		  point2d(max_row,
			  box.pmax().row()));

	  return b;
	}


	inline
	unsigned
	compute_reliable_ldelta(const value::int_u12& char_width,
				const value::int_u12& char_space)
	{
	  return 1.2 * char_width + char_space;
	}


	inline
	unsigned
	compute_unreliable_ldelta(const value::int_u12& max_char_width)
	{
	  return max_char_width * 2; // FIXME: reduce to 1.5 ? Problem
				     // with 2 or 3 lines besides an
				     // image.
	}


      } // end of namespace scribo::primitive::extract


      /////////////
      // FACADES //
      /////////////


      template <typename L>
      mln::util::couple<component_set<L>, mln_ch_value(L,bool)>
      alignments(const document<L>& doc,
		 float dmax_ratio, unsigned delta_pixel)
      {
	trace::entering("scribo::primitive::extract::alignments");
	mln_precondition(doc.is_valid());
	mln_precondition(doc.has_text());
	const mln_ch_value(L,bool)& input = doc.binary_image();

	unsigned min_card = 3;
	unsigned delta = 5;

	// 0. Get low level structures in document data.
	typedef mln_value(L) V;
	const object_groups<L>& groups = doc.paragraphs().lines().groups();
	const line_set<L>& lines = doc.lines();

	(void) groups; // Avoid warnings when debug is disabled.

	// 1. Construct an image of group bounding boxes.
	//
	// This image is used later to group bounding boxes.  It is
	// rotated in order to make grouping faster (linear memory
	// reading).
	//
	box2d rbbox = box2d(input.domain().pmin(),
			    point2d(input.domain().pmax().col(),
				    input.domain().pmax().row()));

	L bbox_ima(rbbox);
	data::fill(bbox_ima, 0);
	for_all_lines(l, lines)
	  if (lines(l).is_valid()
	      && internal::pass_comp_criterion(lines(l)))
	    mln::draw::box(bbox_ima,
			   internal::fast_rotate_positive(lines(l).bbox(),
							  rbbox),
			   l);

	// Compute component information
	//
	// FIXME: not useful since we have all required information
	// in the line_set. However a component_set is needed in link
	// functors...
	component_set<L>
	  components = component_set<L>(bbox_ima, lines.nelements());


	// Dmax functor used for TOP and BOTTOM links.
	primitive::link::internal::dmax_hrules dmax_func(dmax_ratio, 100);


	// Output image
	image2d<bool> delimitors;
	initialize(delimitors, input);
	data::fill(delimitors, false);

# ifndef SCRIBO_NDEBUG
	image2d<value::rgb8> debug;
	initialize(debug, components.labeled_image());
	data::fill(debug, literal::black);
	scribo::draw::bounding_boxes(debug, components, literal::blue);
# endif // ! SCRIBO_NDEBUG




	// 2.1. BEGIN OF TOP LINKS PROCESSING
	{
	  object_links<L> top_links;
	  object_groups<L> top_groups;

	  typedef mln::accu::stat::median_h<value::int_u<12u> > median_t;
	  mln::util::array<median_t>
	    top_median_char_width,
	    top_median_char_space;

	  mln::util::array<unsigned> top_max_char_width;



	  // 2.1.1. Find TOP links.
	  {
	    object_links<L> right, left;

	    // link right
	    {
	      primitive::link::internal::single_right_dmax_ratio_aligned_delta_functor<
		L,primitive::link::internal::dmax_hrules>
		functor(components, dmax_func, delta_pixel, anchor::Vertical, bbox_ima, delta);

	      right = primitive::link::compute(functor, anchor::StrictTopCenter);
	    }

	    // Link left
	    {
	      primitive::link::internal::single_left_dmax_ratio_aligned_delta_functor<
		L,primitive::link::internal::dmax_hrules>
		functor(components, dmax_func, delta_pixel, anchor::Vertical, bbox_ima, delta);

	      left = primitive::link::compute(functor, anchor::StrictTopCenter);
	    }

	    // Merge links
	    top_links = primitive::link::merge_double_link_closest_aligned(left, right,
									   anchor::StrictTopCenter);

	    // Remove groups with not enough links.
	    top_groups = primitive::group::from_single_link(top_links);
	    top_groups = filter::object_groups_small(top_groups, min_card);


	    // Compute char_width and char_space statistics.
	    //
	    // Here, we also compute max_char_width, in case other
	    // statistics are not significant enough for next steps.
	    //
	    // To avoid biased statistics, char_space and char_width are
	    // used from lines with at least 2 components.
	    //
	    top_median_char_space.resize(top_groups.nelements());
	    top_median_char_width.resize(top_groups.nelements());
	    top_max_char_width.resize(top_groups.nelements());

	    for_all_groups(g, top_groups)
	      if (top_groups(g).is_valid())
		for_all_elements(e, top_groups(g).component_ids())
		{
		  unsigned l = top_groups(g).component_ids()(e);
		  if (lines(l).card() > 1)
		  {
		    top_median_char_space(g).take(lines(l).char_space());
		    top_median_char_width(g).take(lines(l).char_width());
		  }
		  else if (top_max_char_width(g) < lines(l).char_width())
		    top_max_char_width(g) = lines(l).char_width();
		}
	  }


	  // 2.1.2. Check TOP whitespaces
	  {
	    util::array<accu::shape::bbox<point2d> >
	      group_bbox(top_groups.nelements());

	    // Compute group bboxes
	    for_all_groups(g, top_groups)
		for_all_elements(e, top_groups(g).component_ids())
 		{
		  unsigned l = top_groups(g).component_ids()(e);
		  point2d p = components(top_links(l)).bbox().pmax();
		  p.row() = components(top_links(l)).bbox().pmin().row();

		  if (top_groups(g).is_valid())// && top_links.is_linked(l))
		  {
		    group_bbox(g).take(p);
		    group_bbox(g).take(components(l).bbox().pmin());

# ifndef SCRIBO_NDEBUG
		    // Draw first component bbox.
		    mln::draw::box(debug,
				   internal::fast_rotate_positive(
				     groups.components()(lines(l).component_ids()[0]).bbox(),
				     rbbox),
				   literal::cyan);

		    mln::draw::box(debug,
				   internal::fast_rotate_positive(
				     groups.components()(lines(top_links(l)).component_ids()[0]).bbox(),
				     rbbox),
				   literal::cyan);
# endif // ! SCRIBO_NDEBUG
		  }
# ifndef SCRIBO_NDEBUG
		  else if (top_groups(g).card() > 1)
		  {
		    mln::draw::line(debug,
				    components(l).bbox().pmin(),
				    p,
				    literal::orange);

		    // Draw first component bbox.
		    mln::draw::box(debug,
				   internal::fast_rotate_positive(
				     groups.components()(lines(l).component_ids()[0]).bbox(),
				     rbbox),
				   literal::cyan);

		    mln::draw::box(debug,
				   internal::fast_rotate_positive(
				     groups.components()(lines(top_links(l)).component_ids()[0]).bbox(),
				     rbbox),
				   literal::cyan);
		  }
# endif // ! SCRIBO_NDEBUG
		}

	    // Looking for whitespaces (if needed)
	    for_all_groups(g, top_groups)
	      if (top_groups(g).is_valid())
	      {
		unsigned ldelta = delta;

		// Special case : handle low connected groups.
		if (top_groups(g).card() >= 3 && top_groups(g).card() < 5)
		{
		  // Stats are not reliable, prefer using max char width.
		  if (top_median_char_width(g).card() == 0)
		    ldelta = internal::compute_unreliable_ldelta(top_max_char_width(g));
		  else
		    ldelta = internal::compute_reliable_ldelta(top_median_char_width(g),
							       top_median_char_space(g));

		  point2d p(group_bbox(g).to_result().pmin().row() - ldelta,
			    group_bbox(g).to_result().pmin().col());

		  p = scribo::internal::point_row_adjust(p, bbox_ima);

		  // Checking whitespace area
		  for (; p.col() <= group_bbox(g).to_result().pmax().col()
			 && (bbox_ima(p) == 0);)
		    ++p.col();

		  if (bbox_ima(p) != 0)
		  {
		    top_groups(g).invalidate();

# ifndef SCRIBO_NDEBUG
		    mln::draw::line(debug,
				    scribo::internal::point_row_adjust(
				      point2d(group_bbox(g).to_result().pmin().row() - ldelta,
					      group_bbox(g).to_result().pmin().col()), debug),
				    scribo::internal::point_row_adjust(
				      point2d(group_bbox(g).to_result().pmin().row() - ldelta,
					      group_bbox(g).to_result().pmax().col()), debug),
				    literal::red);
# endif // ! SCRIBO_NDEBUG
		    continue;
		  }
		}


		box2d
		  brot = internal::fast_rotate_negative(group_bbox(g).to_result(),
							rbbox);

		// Checking if the delimitor is skewed.
		if (internal::is_valid_left_right_skewed_delimitor(brot, input,
								   delta))
		{
		  mln::draw::line(delimitors,
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmin().row(),
					    brot.pmin().col() - delta), delimitors),
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmax().row(),
					    brot.pmax().col() - delta), delimitors),
				  true);
		}
		else if (internal::is_valid_right_left_skewed_delimitor(brot, input,
									delta))
		{
		  mln::draw::line(delimitors,
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmin().row(),
					    brot.pmax().col() - delta), delimitors),
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmax().row(),
					    brot.pmin().col() - delta), delimitors),
				  true);
		}
		else
		{
		  mln::draw::line(delimitors,
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmin().row(),
					    brot.pmin().col() - delta), delimitors),
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmax().row(),
					    brot.pmin().col() - delta), delimitors),
				  true);
		}

# ifndef SCRIBO_NDEBUG
		mln::draw::line(debug,
				scribo::internal::point_row_adjust(
				  point2d(group_bbox(g).to_result().pmin().row() - ldelta,
					  group_bbox(g).to_result().pmin().col()), debug),
				scribo::internal::point_row_adjust(
				  point2d(group_bbox(g).to_result().pmin().row() - ldelta,
					  group_bbox(g).to_result().pmax().col()), debug),
				literal::green);
# endif // ! SCRIBO_NDEBUG
	      }
	  }

	} // END OF TOP LINKS PROCESSING



	// 2.2. BEGIN OF BOTTOM LINKS PROCESSING
	{
	  object_links<L> bot_links;
	  object_groups<L> bot_groups;

	  typedef mln::accu::stat::median_h<value::int_u<12u> > median_t;
	  mln::util::array<median_t>
	    bot_median_char_width,
	    bot_median_char_space;

	  mln::util::array<unsigned> bot_max_char_width;

	  // 2.2.1. Find BOTTOM links.
	  {
	    object_links<L> right, left;

	    // link right
	    {
	      primitive::link::internal::single_right_dmax_ratio_aligned_delta_functor<
		L,primitive::link::internal::dmax_hrules>
		functor(components, dmax_func, delta_pixel, anchor::Vertical,
			bbox_ima, delta);

	      right = primitive::link::compute(functor, anchor::StrictBottomCenter);
	    }

	    // Link left
	    {
	      primitive::link::internal::single_left_dmax_ratio_aligned_delta_functor<
		L,primitive::link::internal::dmax_hrules>
		functor(components, dmax_func, delta_pixel, anchor::Vertical,
			bbox_ima, delta);

	      left = primitive::link::compute(functor, anchor::StrictBottomCenter);
	    }

	    // Merge links
	    bot_links = primitive::link::merge_double_link_closest_aligned(left, right,
									   anchor::StrictBottomCenter);

	    // Remove groups with not enough links.
	    bot_groups = primitive::group::from_single_link(bot_links);

	    bot_groups = filter::object_groups_small(bot_groups, min_card);

	    bot_median_char_space.resize(bot_groups.nelements());
	    bot_median_char_width.resize(bot_groups.nelements());
	    bot_max_char_width.resize(bot_groups.nelements());

	    for_all_groups(g, bot_groups)
	      if (bot_groups(g).is_valid())
		for_all_elements(e, bot_groups(g).component_ids())
		{
		  unsigned l = bot_groups(g).component_ids()(e);

		  if (lines(l).card() > 1)
		  {
		    bot_median_char_space(g).take(lines(l).char_space());
		    bot_median_char_width(g).take(lines(l).char_width());
		  }
		  // This data will be used if alignments are made with single
		  // components only.
		  else if (bot_max_char_width(g) < lines(l).char_width())
		    bot_max_char_width(g) = lines(l).char_width();
		}
	  }


	  // 2.2.2. Check BOTTOM whitespaces
	  {
	    util::array<accu::shape::bbox<point2d> >
	      group_bbox(bot_groups.nelements());

            // Compute group bboxes
	    for_all_groups(g, bot_groups)
	      for_all_elements(e, bot_groups(g).component_ids())
	    {
	      unsigned l = bot_groups(g).component_ids()(e);
	      point2d p = components(l).bbox().pmin();
	      p.row() = components(l).bbox().pmax().row();

	      if (bot_groups(g).is_valid())// && bot_links.is_linked(l))
	      {
		if (bot_groups.group_of(l).is_valid())
		{
		  group_bbox(g).take(p);
		  group_bbox(g).take(components(bot_links(l)).bbox().pmax());

# ifndef SCRIBO_NDEBUG
		  // Draw first component box
		  mln::draw::box(debug,
				 internal::fast_rotate_positive(
				   groups.components()(lines(l).component_ids()[lines(l).card() - 1]).bbox(),
				   rbbox),
				 literal::cyan);

		  mln::draw::box(debug,
				 internal::fast_rotate_positive(
				   groups.components()(lines(bot_links(l)).component_ids()[lines(bot_links(l)).card() - 1]).bbox(),
				   rbbox),
				 literal::cyan);
# endif // ! SCRIBO_NDEBUG
		}
	      }
# ifndef SCRIBO_NDEBUG
	      else if (bot_groups(g).card() > 1)
	      {
		mln::draw::line(debug,
				p,
				components(bot_links(l)).bbox().pmax(),
				literal::orange);

		// Draw first component box
		mln::draw::box(debug,
			       internal::fast_rotate_positive(
				 groups.components()(lines(l).component_ids()[lines(l).card() - 1]).bbox(),
				 rbbox),
			       literal::cyan);

		mln::draw::box(debug,
			       internal::fast_rotate_positive(
				 groups.components()(lines(bot_links(l)).component_ids()[lines(bot_links(l)).card() - 1]).bbox(),
				 rbbox),
			       literal::cyan);
	      }
# endif // ! SCRIBO_NDEBUG
	    }

	    // Looking for whitespaces
	    for_all_groups(g, bot_groups)
	      if (bot_groups(g).is_valid())
	      {
		unsigned ldelta = delta;
		// Special case : handle low connected groups.
		if (bot_groups(g).card() >= 3 && bot_groups(g).card() < 5)
		{
		  // Stats are not reliable, prefer using max char width.
		  if (bot_median_char_width(g).card() == 0)
		    ldelta = internal::compute_unreliable_ldelta(bot_max_char_width(g));
		  else
		    ldelta = internal::compute_reliable_ldelta(bot_median_char_width(g),
							       bot_median_char_space(g));

		  point2d p(bot_groups(g).bbox().pmax().row() + ldelta,
			    bot_groups(g).bbox().pmin().col());
		  p = scribo::internal::point_row_adjust(p, bbox_ima);

		  // Checking whitespace area
		  for (; p.col() <= group_bbox(g).to_result().pmax().col()
			 && (bbox_ima(p) == 0);)
		    ++p.col();

		  if (bbox_ima(p) != 0)
		  {
		    bot_groups(g).invalidate();

# ifndef SCRIBO_NDEBUG
		    mln::draw::line(debug,
				    scribo::internal::point_row_adjust(
				      point2d(group_bbox(g).to_result().pmax().row() + ldelta,
					      group_bbox(g).to_result().pmin().col()), debug),
				    scribo::internal::point_row_adjust(
				      point2d(group_bbox(g).to_result().pmax().row() + ldelta,
					      group_bbox(g).to_result().pmax().col()), debug),
				    literal::red);
# endif // ! SCRIBO_NDEBUG
		    continue;
		  }
		}

		box2d
		  brot = internal::fast_rotate_negative(group_bbox(g).to_result(),
							rbbox);

		// Checking if delimitor is skewed.
		if (internal::is_valid_left_right_skewed_delimitor(brot, input,
								   - delta))
		{
		  mln::draw::line(delimitors,
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmin().row(),
					    brot.pmin().col() + delta),
				    delimitors),
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmax().row(),
					    brot.pmax().col() + delta),
				    delimitors),
				  true);
		}
		else if (internal::is_valid_right_left_skewed_delimitor(brot,
									input,
									- delta))
		{
		  mln::draw::line(delimitors,
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmin().row(),
					    brot.pmax().col() + delta),
				    delimitors),
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmax().row(),
					    brot.pmin().col() + delta),
				    delimitors),
				  true);
		}
		else
		{
		  mln::draw::line(delimitors,
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmin().row(),
					    brot.pmax().col() + delta),
				    delimitors),
				  scribo::internal::point_col_adjust(
				    point2d(brot.pmax().row(),
					    brot.pmax().col() + delta),
				    delimitors),
				  true);
		}

# ifndef SCRIBO_NDEBUG
		mln::draw::line(debug,
				scribo::internal::point_row_adjust(
				  point2d(group_bbox(g).to_result().pmax().row() + ldelta,
					  group_bbox(g).to_result().pmin().col()), debug),
				scribo::internal::point_row_adjust(
				  point2d(group_bbox(g).to_result().pmax().row() + ldelta,
					  group_bbox(g).to_result().pmax().col()), debug),
				literal::green);
# endif // ! SCRIBO_NDEBUG
	      }
	  }

	} // END OF BOTTOM LINKS PROCESSING


	V ndelim;
	component_set<L>
	  delim_comps = primitive::extract::components(delimitors, c8(), ndelim);

	mln::util::couple<component_set<L>, mln_ch_value(L,bool)>
	  output(delim_comps, delimitors);

# ifndef SCRIBO_NDEBUG
	debug = preprocessing::rotate_90(debug, true);
	debug::logger().log_image(debug::All,
				  debug, "alignments_debug");
# endif // ! SCRIBO_NDEBUG

	trace::exiting("scribo::primitive::extract::alignments");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_ALIGNMENTS_HH
