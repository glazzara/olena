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

#ifndef SCRIBO_TOOLCHAIN_INTERNAL_TEXT_IN_PICTURE_FUNCTOR_HH
# define SCRIBO_TOOLCHAIN_INTERNAL_TEXT_IN_PICTURE_FUNCTOR_HH


/// \file
///
/// \brief Localize text in a picture.


# include <libgen.h>
# include <iostream>

#  ifndef SCRIBO_NDEBUG
#  include <mln/util/timer.hh>
#  endif // ! SCRIBO_NDEBUG

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/literal/colors.hh>
# include <mln/value/rgb8.hh>

# include <mln/fun/v2v/rgb_to_int_u.hh>

# include <mln/subsampling/antialiased.hh>

# include <scribo/core/def/lbl_type.hh>

# include <scribo/draw/bounding_boxes.hh>
# include <scribo/draw/groups_bboxes.hh>

# include <scribo/binarization/sauvola_ms.hh>
# include <scribo/binarization/sauvola.hh>

# include <scribo/primitive/extract/components.hh>

# include <scribo/primitive/link/merge_double_link.hh>
# include <scribo/primitive/link/with_single_left_link.hh>
# include <scribo/primitive/link/with_single_right_link.hh>

# include <scribo/primitive/group/apply.hh>
# include <scribo/primitive/group/from_double_link.hh>
# include <scribo/primitive/group/from_single_link.hh>

# include <scribo/primitive/regroup/from_single_left_link.hh>

# include <scribo/filter/object_groups_with_holes.hh>

# include <scribo/filter/object_links_bbox_h_ratio.hh>
# include <scribo/filter/object_links_bbox_overlap.hh>

# include <scribo/filter/object_groups_small.hh>
# include <scribo/filter/object_groups_mean_width.hh>

# include <scribo/debug/decision_image.hh>
# include <scribo/debug/linked_bboxes_image.hh>

# include <scribo/debug/logger.hh>
# include <scribo/debug/highlight_text_area.hh>
# include <scribo/debug/text_areas_image.hh>

# include <scribo/preprocessing/split_bg_fg.hh>

# include <scribo/make/debug_filename.hh>

# include <scribo/toolchain/internal/toolchain_functor.hh>

# include <src/afp/components.hh>
# include <src/afp/link.hh>
# include <src/afp/regroup.hh>


namespace scribo
{

  namespace toolchain
  {

    namespace internal
    {


      using namespace mln;

      /// \brief Localize text in a picture.
      template <typename I>
      struct text_in_picture_functor
	: public Toolchain_Functor
      {
	typedef scribo::def::lbl_type V;
	typedef mln_ch_value(I,V) L;

	text_in_picture_functor();

	virtual int nsteps() const;

	//===============
	// Core function
	//===============

	component_set<L> operator()(const Image<I>&);


	//=========
	// Options
	//=========
	bool enable_bg_removal;
	bool enable_multi_scale_bin;

	//============
	// Parameters
	//============

	unsigned lambda;

	// Image resizing factor
	unsigned max_dim_size;

	// Sauvola ms
	unsigned sauvola_s;
	unsigned sauvola_min_w;

	// Group Filtering
	float bbox_h_ratio;
	float bbox_overlap;
	unsigned small_groups;
	unsigned mean_width;
	unsigned regroup_dmax;
	unsigned group_min_holes;

	//=========
	// Results
	//=========
	component_set<L> output;
	object_groups<L> groups;

#  ifndef SCRIBO_NDEBUG
	//=============
	// DEBUG TOOLS
	//=============
	virtual void on_start();
	virtual void on_end();
	virtual void on_progress();

	mln::util::timer t;
	mln::util::timer gt;
#  endif // ! SCRIBO_NDEBUG

      private:
	unsigned get_factor(const I& ima, unsigned max_dim_size);

      };


# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      unsigned
      text_in_picture_functor<I>::get_factor(const I& ima, unsigned max_dim_size)
      {
	unsigned
	  nrows = ima.nrows(),
	  ncols = ima.ncols(),
	  max_dim = std::max(nrows, ncols),
	  factor = max_dim / max_dim_size;

	return factor ? factor : 1;
      }


      template <typename I>
      text_in_picture_functor<I>::text_in_picture_functor()
	: enable_bg_removal(false),
	  enable_multi_scale_bin(true),
	  lambda(0),
	  max_dim_size(1024),
	  sauvola_s(2u),
	  sauvola_min_w(51u),
	  bbox_h_ratio(1.60f),
	  bbox_overlap(0.80f),
	  small_groups(3),
	  mean_width(8),
	  regroup_dmax(30),
	  group_min_holes(3)
      {
      }


      template <typename I>
      component_set<typename text_in_picture_functor<I>::L>
      text_in_picture_functor<I>::operator()(const Image<I>& input_rgb_orig_)
      {
	const I& input_rgb_orig = exact(input_rgb_orig_);
	mln_precondition(input_rgb_orig.is_valid());

	using namespace scribo;
	using namespace scribo::primitive;
	using namespace mln;

	on_start();

	// Setting up options.
	if (lambda == 0)
	  lambda = 1.2 * (input_rgb_orig.nrows() + input_rgb_orig.ncols());

	if (verbose)
	  std::cout << "Using lambda = " << lambda << std::endl;


	on_new_progress_label("Resizing image if needed...");

	if (max_dim_size != 0)
	  max_dim_size = max_dim_size;

	unsigned factor = get_factor(input_rgb_orig, max_dim_size);

	if (verbose)
	{
	  std::cout << "Reduction Factor : " << factor << std::endl;
	  std::cout << "Original domain: " << input_rgb_orig.domain() << std::endl;
	}

	mln_concrete(I)
	  input_rgb = mln::subsampling::antialiased(input_rgb_orig, factor);

	if (verbose)
	  std::cout << "Resized domain: " << input_rgb.domain() << std::endl;

	on_progress();


	mln_ch_value(I,value::int_u8) intensity_ima;
	if (enable_bg_removal)
	{
	  on_new_progress_label("Extracting foreground and converting to intensity image...");

	  // Extract foreground
	  mln_ch_value(I,value::rgb8)
	    fg = preprocessing::split_bg_fg(input_rgb, lambda, 32).second();
	  intensity_ima = data::transform(fg, mln::fun::v2v::rgb_to_int_u<8>());

#  ifndef SCRIBO_NDEBUG
	  scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults, fg,
					    "foreground");
#  endif // ! SCRIBO_NDEBUG
	}
	else
	{
	  on_new_progress_label("Converting to intensity image...");
	  intensity_ima = data::transform(input_rgb,
					  mln::fun::v2v::rgb_to_int_u<8>());
	}

	on_progress();

	// Binarize foreground to use it in the processing chain.
	mln_ch_value(I,bool) input;
	unsigned w = std::min(intensity_ima.nrows() / 3, intensity_ima.ncols() / 3);
	if (! w % 2)
	  ++w;
	w = std::min(w, sauvola_min_w);
	if (verbose)
	  std::cout << "** Using Sauvola with w_1 = " << w << std::endl;

	if (enable_multi_scale_bin)
	{
	  on_new_progress_label("Binarizing image with multi-scale algorithm...");
	  input = scribo::binarization::sauvola_ms(intensity_ima, w, sauvola_s);
	}
	else
	{
	  on_new_progress_label("Binarizing image...");
	  input = scribo::binarization::sauvola(intensity_ima, w);
	}

#  ifndef SCRIBO_NDEBUG
	scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults, input,
					  "binarization.pbm");
#  endif // ! SCRIBO_NDEBUG

	on_progress();



	typedef mln_ch_value(I,def::lbl_type) L;

	/// Finding components.
	on_new_progress_label("Finding components");

	typedef component_set<L> Obj;
	Obj filtered_components;

	{
	  mln::util::array<std::pair<box2d, std::pair<point2d, unsigned> > > attribs;

	  def::lbl_type ncomponents;
	  L components = extract_components(input, ncomponents, attribs);

	  filtered_components = Obj(components, ncomponents, attribs);
	}

	if (verbose)
	  std::cout << "Object extracted " << filtered_components.nelements() << " components" << std::endl;

	on_progress();

	/// linking potential components
	on_new_progress_label("Linking potential components");

	mln::util::couple<object_links<L>, object_links<L> >
	  links = link::left_right(filtered_components);

	object_links<L>& left_link = links.first();
	object_links<L>& right_link = links.second();

	on_progress();

#  ifndef SCRIBO_NDEBUG
	if (scribo::debug::logger().is_enabled())
	{
	  std::cerr << "BEFORE - ncomponents = " << filtered_components.nelements() << std::endl;
	  scribo::debug::logger().log_image(
	    scribo::debug::AuxiliaryResults,
	    scribo::debug::linked_bboxes_image(input,
					       left_link, right_link,
					       literal::red, literal::cyan,
					       literal::yellow,
					       literal::green,
					       anchor::MassCenter),
	    "links");
	}
#  endif // ! SCRIBO_NDEBUG

	// Validating left and right links.
	on_new_progress_label("Validating left and right links");

	object_links<L>
	  merged_links = link::merge_double_link(left_link, right_link);

	on_progress();

	// Remove links if bboxes have too different sizes.
	on_new_progress_label("Filtering component links.");

	object_links<L>
	  hratio_filtered_links = filter::object_links_bbox_h_ratio(merged_links,
								    bbox_h_ratio);


#  ifndef SCRIBO_NDEBUG
	if (scribo::debug::logger().is_enabled())
	{
	  mln_ch_value(I,value::rgb8)
	    hratio_decision_image = scribo::debug::decision_image(input,
								  merged_links,
								  hratio_filtered_links,
								  anchor::MassCenter);

	  scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults,
					    hratio_decision_image,
					    "hratio_links_decision_image");
	}
#  endif // ! SCRIBO_NDEBUG

	//Remove links if bboxes overlap too much.
	object_links<L> overlap_filtered_links
	  = filter::object_links_bbox_overlap(hratio_filtered_links,
					      bbox_overlap);


#  ifndef SCRIBO_NDEBUG
	if (scribo::debug::logger().is_enabled())
	{
	  mln_ch_value(I,value::rgb8) overlap_decision_image
	    = scribo::debug::decision_image(input,
					    hratio_filtered_links,
					    overlap_filtered_links,
					    anchor::MassCenter);

	  scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults,
					    overlap_decision_image,
					    "overlap_links_decision_image");
	}
#  endif // ! SCRIBO_NDEBUG

	on_progress();


	on_new_progress_label("Grouping components...");

	groups = group::from_single_link(overlap_filtered_links);

	on_progress();


#  ifndef SCRIBO_NDEBUG
	if (scribo::debug::logger().is_enabled())
	{
	  //  Apply grouping in a temporary image (for debug purpose).
	  component_set<L>
	    raw_group_image = group::apply(groups);

	  mln_ch_value(I,value::rgb8)
	    decision_image = data::convert(value::rgb8(), input);

	  scribo::draw::bounding_boxes(decision_image,
				       filtered_components, literal::green);
	  scribo::draw::bounding_boxes(decision_image,
				       raw_group_image, literal::blue);

	  scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults,
					    decision_image,
					    "group_and_object_image");

	  decision_image = data::convert(value::rgb8(), input);
	  scribo::draw::bounding_boxes(decision_image,
				       raw_group_image, literal::blue);

	  scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults,
					    decision_image,
					    "group_image");
	}
#  endif // ! SCRIBO_NDEBUG

	on_new_progress_label("Filtering groups with too low cardinality...");

	// Remove components part of groups with strictly less than 3 components.
	object_groups<L> filtered_small_groups;
	filtered_small_groups = filter::object_groups_small(groups,
							    small_groups);

#  ifndef SCRIBO_NDEBUG
	mln_ch_value(I,value::rgb8) decision_image;
	if (scribo::debug::logger().is_enabled())
	{
	  decision_image = data::convert(value::rgb8(), input);

	  scribo::draw::groups_bboxes(decision_image, groups, literal::red);
	  scribo::draw::groups_bboxes(decision_image, filtered_small_groups,
				      literal::green);

	  scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults,
					    decision_image,
					    "small_groups_filter");
	}
#  endif // ! SCRIBO_NDEBUG

	on_progress();

	on_new_progress_label("Filtering groups having components with too low thickness...");

	// Remove components part of groups having a mean thickness lower than 8.
	object_groups<L> filtered_thin_groups;
	filtered_thin_groups
	  = filter::object_groups_mean_width(filtered_small_groups,
					     mean_width);

#  ifndef SCRIBO_NDEBUG
	if (scribo::debug::logger().is_enabled())
	{
	  decision_image = data::convert(value::rgb8(), input);

	  scribo::draw::groups_bboxes(decision_image, filtered_small_groups,
				      literal::red);
	  scribo::draw::groups_bboxes(decision_image, filtered_thin_groups,
				      literal::green);

	  scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults,
					    decision_image,
					    "thin_groups_filter");
	}
#  endif // ! SCRIBO_NDEBUG


	on_progress();

	/// Apply grouping in the object image.

	on_new_progress_label("Regrouping groups...");

	filtered_thin_groups = regroup::from_single_left_link(filtered_thin_groups,
							      regroup_dmax);

	on_progress();


	/// Filter grouped objects not having enough background components.
	on_new_progress_label("Filtering groups with too many holes...");

	groups  = scribo::filter::object_groups_with_holes(filtered_thin_groups,
							   group_min_holes);

	on_progress();

#  ifndef SCRIBO_NDEBUG
	if (scribo::debug::logger().is_enabled())
	{
	  decision_image = data::convert(value::rgb8(), input);

	  scribo::draw::groups_bboxes(decision_image, filtered_thin_groups,
				      literal::red);
	  scribo::draw::groups_bboxes(decision_image, groups, literal::green);

	  scribo::debug::logger().log_image(scribo::debug::AuxiliaryResults,
					    decision_image,
					    "group_with_holes_filter");
	}
#  endif // ! SCRIBO_NDEBUG


	on_new_progress_label("Finalizing results...");

	output = primitive::group::apply(groups);

	on_progress();

	if (verbose)
	  std::cout << "# objects = " << output.nelements() << std::endl;


#  ifndef SCRIBO_NDEBUG
	if (scribo::debug::logger().is_enabled())
	{
	  scribo::debug::logger().log_image(scribo::debug::Results,
					    scribo::debug::highlight_text_area(input_rgb,
									       output),
					    "input_with_bboxes");
	  scribo::debug::logger().log_image(scribo::debug::Results,
					    scribo::debug::text_areas_image(input_rgb,
									    output),
					    "out_text");
	}
#  endif // ! SCRIBO_NDEBUG

	on_end();

	return output;
      }


      template <typename I>
      int
      text_in_picture_functor<I>::nsteps() const
      {
	return 10; // FIXME
      }


#  ifndef SCRIBO_NDEBUG

      template <typename I>
      void
      text_in_picture_functor<I>::on_start()
      {
	gt.start();
	t.start();
      }

      template <typename I>
      void
      text_in_picture_functor<I>::on_end()
      {
	gt.stop();
	if (verbose)
	  std::cout << "Total time: " << gt << std::endl;
      }

      template <typename I>
      void
      text_in_picture_functor<I>::on_progress()
      {
	t.stop();
	if (verbose)
	  std::cout << t << std::endl;
	t.restart();
      }


#  endif // ! SCRIBO_NDEBUG


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::toolchain::internal

  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // SCRIBO_TOOLCHAIN_INTERNAL_TEXT_IN_PICTURE_FUNCTOR_HH

