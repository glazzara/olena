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

#include <libgen.h>
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/colorize.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/magick/all.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/fun/v2v/rgb_to_luma.hh>

#include <mln/subsampling/antialiased.hh>

#include <scribo/draw/bounding_boxes.hh>
#include <scribo/draw/groups_bboxes.hh>

#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/binarization/sauvola.hh>

#include <scribo/primitive/extract/components.hh>

#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/with_single_left_link.hh>
#include <scribo/primitive/link/with_single_right_link.hh>

#include <scribo/primitive/group/apply.hh>
#include <scribo/primitive/group/from_double_link.hh>
#include <scribo/primitive/group/from_single_link.hh>

#include <scribo/primitive/regroup/from_single_left_link.hh>
// #include <scribo/primitive/regroup/from_single_left_link_wrt_h_ratio.hh>

#include <scribo/filter/object_groups_with_holes.hh>

#include <scribo/filter/object_links_bbox_h_ratio.hh>
#include <scribo/filter/object_links_bbox_overlap.hh>

#include <scribo/filter/object_groups_small.hh>
#include <scribo/filter/object_groups_v_thickness.hh>

#include <scribo/debug/highlight_text_area.hh>
#include <scribo/debug/text_areas_image.hh>

#include <scribo/debug/decision_image.hh>
#include <scribo/debug/linked_bboxes_image.hh>

#include <scribo/debug/usage.hh>

#include <scribo/preprocessing/split_bg_fg.hh>

#include <scribo/make/debug_filename.hh>

#include <mln/util/timer.hh>
#include <mln/core/var.hh>


#include <src/afp/components.hh>
#include <src/afp/link.hh>
#include <src/afp/regroup.hh>

const char *args_desc[][2] =
{
  { "input.*", "A color image." },
  { "ouput.ppm", "A color image where the text is highlighted." },
  { "debug_output_dir", "Directory were debug images will be saved" },
  { "lambda", "Lambda value used for foreground extraction" },
  {0, 0}
};


namespace mln
{


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
      v_thickness = 8;
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
    unsigned v_thickness;
    unsigned regroup_dmax;
    unsigned group_min_holes;
  };


} // end of namespace mln


// Global config variable.
mln::config conf;



namespace mln
{

  template <typename I>
  unsigned get_factor(const I& ima)
  {
    unsigned
      nrows = ima.nrows(),
      ncols = ima.ncols(),
      max_dim = std::max(nrows, ncols),
      factor = max_dim / conf.max_dim_size;

    return factor ? factor : 1;
  }

} // end of namespace mln


int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace scribo::primitive;
  using namespace mln;

  if (argc < 3 || argc > 11)
    return scribo::debug::usage(argv,
				"Find text in a photo.\n\n"
				"Common usage: ./text_in_picture input.*"
				" output.ppm 1 1 1 1 1",
				"input.ppm output.ppm <bg/fg enabled>"
				" <sauvola_ms enabled> "
				"<holes in group filter enabled> "
				"<small group filter enabled> "
				"<thin group filter enabled> "
				"[debug_output_dir] [max_dim_size] [lambda]",
				args_desc);

  std::string out_base_dir;
  bool debug = false;
  if (argc > 8)
  {
    scribo::make::internal::debug_filename_prefix = argv[8];
    debug = true;
    out_base_dir = argv[8];
  }

  trace::entering("main");

  Magick::InitializeMagick(*argv);

  image2d<value::rgb8> input_rgb;
  io::magick::load(input_rgb, argv[1]);

  if (argc >= 10)
    conf.max_dim_size = atoi(argv[9]);

  unsigned factor = get_factor(input_rgb);

  std::cout << "Reduction Factor : " << factor << std::endl;

  std::cout << "Original domain: " << input_rgb.domain() << std::endl;

  input_rgb = mln::subsampling::antialiased(input_rgb, factor);

  std::cout << "Resized domain: " << input_rgb.domain() << std::endl;

  unsigned lambda;
  if (argc == 11)
    lambda = atoi(argv[10]);
  else
    lambda = 1.2 * (input_rgb.nrows() + input_rgb.ncols());

  std::cout << "Using lambda = " << lambda << std::endl;

  image2d<value::int_u8> intensity_ima;
  mln::util::timer timer_, global_t_;
  float t_;

  global_t_.start();

  if (argc > 3 && atoi(argv[3]) != 0)
  {
    // Extract foreground
    timer_.start();
    std::cout << "** Using split_bg_fg" << std::endl;
    image2d<value::rgb8>
      fg = preprocessing::split_bg_fg(input_rgb, lambda, 32).second();
    intensity_ima = data::transform(fg,
				    mln::fun::v2v::rgb_to_luma<value::int_u8>());
    t_ = timer_;
    std::cout << "Foreground extracted. " << t_ << std::endl;

#ifndef NOUT
    if (debug)
    {
      io::ppm::save(fg, scribo::make::debug_filename("foreground.ppm"));
    }

#endif // !NOUT
  }
  else
  {
    timer_.start();
    std::cout << "** Using data::transform(intensity)" << std::endl;
    intensity_ima = data::transform(input_rgb,
				    mln::fun::v2v::rgb_to_luma<value::int_u8>());
    t_ = timer_;
    std::cout << "Intensity image " << t_ << std::endl;
  }


  // Binarize foreground to use it in the processing chain.
  timer_.restart();
  image2d<bool> input;
  unsigned w = std::min(intensity_ima.nrows() / 3, intensity_ima.ncols() / 3);
  if (! w % 2)
    ++w;
  w = std::min(w, conf.sauvola_min_w);
  if (argc > 4 && atoi(argv[4]) != 0)
  {
    std::cout << "** Using sauvola_ms with w_1 = " << w << std::endl;
    input = scribo::binarization::sauvola_ms(intensity_ima, w, conf.sauvola_s);
  }
  else
  {
    std::cout << "** Using sauvola with w_1 = " << w << std::endl;
    input = scribo::binarization::sauvola(intensity_ima, w);
  }
#ifndef NOUT
    if (debug)
    {
      io::pbm::save(input,
		    scribo::make::debug_filename("binarization.pbm"));
    }

#endif // !NOUT

//   if (debug)
//     io::pbm::save(input, "input.pbm");
  t_ = timer_;
  std::cout << "Foreground binarized. " << t_ << std::endl;



  typedef image2d<value::label_16> L;

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
    links = scribo::primitive::link::left_right(filtered_components);

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
  object_links<L> merged_links =
    scribo::primitive::link::merge_double_link(left_link, right_link);
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
    image2d<value::rgb8>
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
    image2d<value::rgb8> overlap_decision_image
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
    image2d<value::rgb8>
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
  if (argc > 6 && atoi(argv[6]) != 0)
  {
    std::cout << "** Using group too small" << std::endl;
    filtered_small_groups = filter::object_groups_small(groups,
							conf.small_groups);
  }
  else
    filtered_small_groups = groups;
  t_ = g_timer;
  std::cout << "Small groups removed " << t_ << std::endl;



#ifndef NOUT
  component_set<L> debug_image;
  image2d<value::rgb8> decision_image;
  if (debug)
  {
    decision_image = data::convert(value::rgb8(), input);

    scribo::draw::groups_bboxes(decision_image, groups, literal::red);
    scribo::draw::groups_bboxes(decision_image, filtered_small_groups,
				literal::green);


    io::ppm::save(decision_image,
		  scribo::make::debug_filename("small_groups_filter.ppm"));
  }
#endif



  // Remove components part of groups having a mean thickness lower than 8.
  g_timer.restart();
  object_groups<L> filtered_thin_groups;
  if (argc > 7 && atoi(argv[7]) != 0)
  {
    std::cout << "** Using group too thin" << std::endl;
    filtered_thin_groups
      = filter::object_groups_v_thickness(filtered_small_groups,
					  conf.v_thickness);
  }
  else
    filtered_thin_groups = filtered_small_groups;
  t_ = g_timer;
  std::cout << "Groups too thin " << t_ << std::endl;


#ifndef NOUT
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
#endif



  /// Apply grouping in the object image.
  g_timer.restart();

  // Grouping groups together if possible.
//   filtered_thin_groups = regroup::from_single_left_link_wrt_h_ratio(filtered_thin_groups,
// 								    conf.regroup_dmax,
// 								    1.60f);
  filtered_thin_groups = regroup::from_single_left_link(filtered_thin_groups,
								   conf.regroup_dmax);


//   component_set<L>
//     grouped_components = group::apply(groups);


  t_ = g_timer;
  std::cout << "Link and group again " << t_ << std::endl;

  timer_.stop();

//   if (debug)
//     io::ppm::save(mln::labeling::colorize(value::rgb8(),
// 					  grouped_components,
// 					  grouped_components.nelements()),
// 		  scribo::make::debug_filename("out_raw.ppm"));


  timer_.resume();

//   io::pgm::save(data::wrap(value::int_u8(), grouped_components.labeled_image_()), "lbl_to_be_filtered.pgm");

  g_timer.restart();
  /// Filter grouped objects not having enough background components.
  if (argc > 5 && atoi(argv[5]) != 0)
  {
    std::cout << "** Using objects_with_two_holes" << std::endl;

    groups  = scribo::filter::object_groups_with_holes(filtered_thin_groups,
						       conf.group_min_holes);

    t_ = g_timer;
    std::cout << "Objects_with_holes " << t_ << std::endl;
  }
  else
    groups = filtered_thin_groups;


  t_ = timer_;
  std::cout << "Objects groups filtered. " << t_ << std::endl;


  t_ = global_t_;
  std::cout << "*** Result computed " << t_ << std::endl;

#ifndef NOUT
  if (debug)
  {
    decision_image = data::convert(value::rgb8(), input);

    scribo::draw::groups_bboxes(decision_image, filtered_thin_groups,
				literal::red);
    scribo::draw::groups_bboxes(decision_image, groups, literal::green);

    io::ppm::save(decision_image, scribo::make::debug_filename("group_with_holes_filter.ppm"));
  }
#endif


  timer_.restart();
  std::cout << "Saving result..." << std::endl;

  component_set<L> comps = primitive::group::apply(groups);
  io::ppm::save(mln::labeling::colorize(value::rgb8(),
					comps.labeled_image(),
					comps.nelements()),
		argv[2]);

  io::ppm::save(scribo::debug::highlight_text_area(input_rgb, comps),
		out_base_dir + "_input_with_bboxes.ppm");
  io::ppm::save(scribo::debug::text_areas_image(input_rgb, comps),
		out_base_dir + "_out_text.ppm");

  t_ = timer_;
  std::cout << "Output saved " << t_ << std::endl;

  std::cout << "# objects = " << comps.nelements() << std::endl;

  trace::exiting("main");
  return comps.nelements() != 0;
}
