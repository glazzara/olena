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

#ifndef SCRIBO_TOOLCHAIN_TEXT_IN_PICTURE_HH
# define SCRIBO_TOOLCHAIN_TEXT_IN_PICTURE_HH


/// \file
///
/// Localize text in a picture.


# include <libgen.h>
# include <iostream>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>

# include <mln/literal/colors.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/label_16.hh>

# include <mln/fun/v2v/rgb_to_int_u.hh>

# include <mln/subsampling/antialiased.hh>

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

# include <scribo/debug/usage.hh>

# include <scribo/preprocessing/split_bg_fg.hh>

# include <scribo/make/debug_filename.hh>

# include <src/afp/components.hh>
# include <src/afp/link.hh>
# include <src/afp/regroup.hh>


# include <mln/util/timer.hh>


namespace scribo
{

  namespace toolchain
  {

    using namespace mln;


    template <typename I>
    component_set<mln_ch_value(I, value::label_16)>
    text_in_picture(const Image<I>& input_rgb_orig,
		    bool bg_removal, bool multi_scale_bin,
		    unsigned max_dim_size = 0, unsigned lambda = 0,
		    const char *debug_outdir = 0);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I>
      unsigned get_factor(const I& ima, unsigned max_dim_size)
      {
	unsigned
	  nrows = ima.nrows(),
	  ncols = ima.ncols(),
	  max_dim = std::max(nrows, ncols),
	  factor = max_dim / max_dim_size;

	return factor ? factor : 1;
      }


      struct config
      {
	config()
	{
	  max_dim_size = 1024;

	  sauvola_s = 2u; // 3?
	  sauvola_min_w = 51u;

	  // Group Filtering
	  bbox_h_ratio = 1.60f;
	  bbox_overlap = 0.80f;
	  small_groups = 3;
	  mean_width = 8;
	  regroup_dmax = 30;
	  group_min_holes = 3;
	}

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
      };


    } // end of namespace scribo::toolchain::internal


    template <typename I>
    component_set<mln_ch_value(I, value::label_16)>
    text_in_picture(const Image<I>& input_rgb_orig_,
		    bool bg_removal, bool multi_scale_bin,
		    unsigned max_dim_size = 0, unsigned lambda = 0,
		    const char *debug_outdir = 0)
    {
      trace::entering("scribo::toolchain::text_in_picture");

      const I& input_rgb_orig = exact(input_rgb_orig_);
      mln_precondition(input_rgb_orig.is_valid());

      using namespace scribo;
      using namespace scribo::primitive;
      using namespace mln;


      // Global config variable.
      internal::config conf;


      bool debug = false;
      if (debug_outdir)
      {
	scribo::make::internal::debug_filename_prefix = debug_outdir;
	debug = true;
      }

      if (max_dim_size != 0)
	conf.max_dim_size = max_dim_size;

      unsigned factor = internal::get_factor(input_rgb_orig, conf.max_dim_size);

      std::cout << "Reduction Factor : " << factor << std::endl;

      std::cout << "Original domain: " << input_rgb_orig.domain() << std::endl;

      mln_concrete(I)
	input_rgb = mln::subsampling::antialiased(input_rgb_orig, factor);

      std::cout << "Resized domain: " << input_rgb.domain() << std::endl;

      if (lambda == 0)
	lambda = 1.2 * (input_rgb.nrows() + input_rgb.ncols());

      std::cout << "Using lambda = " << lambda << std::endl;

      mln_ch_value(I,value::int_u8) intensity_ima;
      mln::util::timer timer_, global_t_;
      float t_;

      global_t_.start();

      if (bg_removal)
      {
	// Extract foreground
	timer_.start();
	std::cout << "** Using split_bg_fg" << std::endl;
	mln_ch_value(I,value::rgb8)
	  fg = preprocessing::split_bg_fg(input_rgb, lambda, 32).second();
	intensity_ima = data::transform(fg, mln::fun::v2v::rgb_to_int_u<8>());
	t_ = timer_;
	std::cout << "Foreground extracted. " << t_ << std::endl;

	if (debug)
	{
	  io::ppm::save(fg, scribo::make::debug_filename("foreground.ppm"));
	}
      }
      else
      {
	timer_.start();
	std::cout << "** Using data::transform(intensity)" << std::endl;
	intensity_ima = data::transform(input_rgb,
					mln::fun::v2v::rgb_to_int_u<8>());
	t_ = timer_;
	std::cout << "Intensity image " << t_ << std::endl;
      }


      // Binarize foreground to use it in the processing chain.
      timer_.restart();
      mln_ch_value(I,bool) input;
      unsigned w = std::min(intensity_ima.nrows() / 3, intensity_ima.ncols() / 3);
      if (! w % 2)
	++w;
      w = std::min(w, conf.sauvola_min_w);
      if (multi_scale_bin)
      {
	std::cout << "** Using sauvola_ms with w_1 = " << w << std::endl;
	input = scribo::binarization::sauvola_ms(intensity_ima, w, conf.sauvola_s);
      }
      else
      {
	std::cout << "** Using sauvola with w_1 = " << w << std::endl;
	input = scribo::binarization::sauvola(intensity_ima, w);
      }

      if (debug)
      {
	io::pbm::save(input,
		      scribo::make::debug_filename("binarization.pbm"));
      }

      t_ = timer_;
      std::cout << "Foreground binarized. " << t_ << std::endl;



      typedef mln_ch_value(I,value::label_16) L;

      /// Finding components.
      timer_.restart();

      typedef component_set<L> Obj;
      Obj filtered_components;

      {
	mln::util::array<std::pair<box2d, std::pair<point2d, unsigned> > > attribs;

	value::label_16 ncomponents;
	L components = extract_components(input, ncomponents, attribs);

	filtered_components = Obj(components, ncomponents, attribs);
      }

      t_ = timer_;
      std::cout << "Object extracted " << t_ << " - " << filtered_components.nelements() << " components" << std::endl;

//   if (debug)
//     io::pgm::save(data::stretch(value::int_u8(), filtered_components), "ref_components.pgm");


      /// linking potential components
      timer_.restart();
      mln::util::couple<object_links<L>, object_links<L> >
	links = link::left_right(filtered_components);

      object_links<L>& left_link = links.first();
      object_links<L>& right_link = links.second();

//   object_links<L> left_link
//     = primitive::link::with_single_left_link(filtered_components, 30);
//   t_ = timer_;
//   std::cout << "Left Link done " << t_ << std::endl;

//   timer_.restart();
//   object_links<L> right_link
//     = primitive::link::with_single_right_link(filtered_components, 30);
      t_ = timer_;
      std::cout << "Link done " << t_ << std::endl;



#ifndef NOUT
      if (debug)
      {
	std::cerr << "BEFORE - ncomponents = " << filtered_components.nelements() << std::endl;
	io::ppm::save(scribo::debug::linked_bboxes_image(input,
							 left_link, right_link,
							 literal::red, literal::cyan,
							 literal::yellow,
							 literal::green,
							 anchor::MassCenter),
		      scribo::make::debug_filename("links.ppm"));
      }
#endif



      // Validating left and right links.
      timer_.restart();
      object_links<L>
	merged_links = link::merge_double_link(left_link, right_link);
      t_ = timer_;
      std::cout << "Right/Left Validation. " << t_ << std::endl;


      // Remove links if bboxes have too different sizes.
      timer_.restart();
      object_links<L>
	hratio_filtered_links = filter::object_links_bbox_h_ratio(merged_links,
								  conf.bbox_h_ratio);




#ifndef NOUT
      if (debug)
      {
	mln_ch_value(I,value::rgb8)
	  hratio_decision_image = scribo::debug::decision_image(input,
								merged_links,
								hratio_filtered_links,
								anchor::MassCenter);
	io::ppm::save(hratio_decision_image,
		      scribo::make::debug_filename("hratio_links_decision_image.ppm"));
      }
#endif




      //Remove links if bboxes overlap too much.
      object_links<L> overlap_filtered_links
	= filter::object_links_bbox_overlap(hratio_filtered_links,
					    conf.bbox_overlap);




#ifndef NOUT
      if (debug)
      {
	mln_ch_value(I,value::rgb8) overlap_decision_image
	  = scribo::debug::decision_image(input,
					  hratio_filtered_links,
					  overlap_filtered_links,
					  anchor::MassCenter);
	io::ppm::save(overlap_decision_image,
		      scribo::make::debug_filename("overlap_links_decision_image.ppm"));
      }
#endif


      t_ = timer_;
      std::cout << "Components links filtered. " << t_ << std::endl;

      timer_.restart();
      object_groups<L>
	groups = group::from_single_link(overlap_filtered_links);



//  Apply grouping in a temporary image (for debug purpose).
#ifndef NOUT
      component_set<L>
	raw_group_image = group::apply(groups);
#endif // !NOUT

      t_ = timer_;
      std::cout << "Components grouped. " << t_ << std::endl;

#ifndef NOUT

      if (debug)
      {
	mln_ch_value(I,value::rgb8)
	  decision_image = data::convert(value::rgb8(), input);

	scribo::draw::bounding_boxes(decision_image,
				     filtered_components, literal::green);
	scribo::draw::bounding_boxes(decision_image,
				     raw_group_image, literal::blue);

	io::ppm::save(decision_image,
		      scribo::make::debug_filename("group_and_object_image.ppm"));

	decision_image = data::convert(value::rgb8(), input);
	scribo::draw::bounding_boxes(decision_image,
				     raw_group_image, literal::blue);
	io::ppm::save(decision_image,
		      scribo::make::debug_filename("group_image.ppm"));
      }
#endif // !NOUT


      std::cout << "Filtering groups..." << std::endl;

      mln::util::timer g_timer;

      timer_.restart();
      // Remove components part of groups with strictly less than 3 components.

      g_timer.start();
      object_groups<L> filtered_small_groups;
      std::cout << "** Using group too small" << std::endl;
      filtered_small_groups = filter::object_groups_small(groups,
							  conf.small_groups);
      t_ = g_timer;
      std::cout << "Small groups removed " << t_ << std::endl;



      component_set<L> debug_image;
      mln_ch_value(I,value::rgb8) decision_image;
      if (debug)
      {
	decision_image = data::convert(value::rgb8(), input);

	scribo::draw::groups_bboxes(decision_image, groups, literal::red);
	scribo::draw::groups_bboxes(decision_image, filtered_small_groups,
				    literal::green);


	io::ppm::save(decision_image,
		      scribo::make::debug_filename("small_groups_filter.ppm"));
      }



      // Remove components part of groups having a mean thickness lower than 8.
      g_timer.restart();
      object_groups<L> filtered_thin_groups;
      std::cout << "** Using group too thin" << std::endl;
      filtered_thin_groups
	= filter::object_groups_mean_width(filtered_small_groups,
					   conf.mean_width);
      t_ = g_timer;
      std::cout << "Groups too thin " << t_ << std::endl;


      if (debug)
      {
	decision_image = data::convert(value::rgb8(), input);

	scribo::draw::groups_bboxes(decision_image, filtered_small_groups,
				    literal::red);
	scribo::draw::groups_bboxes(decision_image, filtered_thin_groups,
				    literal::green);

	io::ppm::save(decision_image,
		      scribo::make::debug_filename("thin_groups_filter.ppm"));
      }



      /// Apply grouping in the object image.
      g_timer.restart();


      filtered_thin_groups = regroup::from_single_left_link(filtered_thin_groups,
							    conf.regroup_dmax);


      t_ = g_timer;
      std::cout << "Link and group again " << t_ << std::endl;

      timer_.stop();

      timer_.resume();

      g_timer.restart();


      /// Filter grouped objects not having enough background components.
      std::cout << "** Using objects_with_two_holes" << std::endl;

      groups  = scribo::filter::object_groups_with_holes(filtered_thin_groups,
							 conf.group_min_holes);

      t_ = g_timer;
      std::cout << "Objects_with_holes " << t_ << std::endl;

      t_ = timer_;
      std::cout << "Objects groups filtered. " << t_ << std::endl;


      t_ = global_t_;
      std::cout << "*** Result computed " << t_ << std::endl;

      if (debug)
      {
	decision_image = data::convert(value::rgb8(), input);

	scribo::draw::groups_bboxes(decision_image, filtered_thin_groups,
				    literal::red);
	scribo::draw::groups_bboxes(decision_image, groups, literal::green);

	io::ppm::save(decision_image, scribo::make::debug_filename("group_with_holes_filter.ppm"));
      }


      timer_.restart();
      std::cout << "Saving result..." << std::endl;

      component_set<L> comps = primitive::group::apply(groups);

      t_ = timer_;
      std::cout << "Output saved " << t_ << std::endl;

      std::cout << "# objects = " << comps.nelements() << std::endl;

      trace::exiting("scribo::toolchain::text_in_picture");
      return comps;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::toolchain

} // end of namespace scribo


#endif // SCRIBO_TOOLCHAIN_TEXT_IN_PICTURE_HH

