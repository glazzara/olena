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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <libgen.h>
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/tr_image.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/colorize.hh>

#include <mln/data/stretch.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/magick/all.hh>

#include <mln/math/min.hh>

#include <mln/logical/not.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/fun/v2v/rgb_to_luma.hh>

#include <mln/data/wrap.hh>

#include <mln/draw/box.hh>

#include <mln/geom/translate.hh>

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
#include <scribo/primitive/regroup/from_single_left_link_wrt_h_ratio.hh>

#include <scribo/filter/objects_size_ratio.hh>

#include <scribo/filter/object_groups_with_holes.hh>

#include <scribo/filter/object_links_bbox_h_ratio.hh>
#include <scribo/filter/object_links_bbox_overlap.hh>

#include <scribo/filter/common/objects_photo.hh>

#include <scribo/filter/object_groups_small.hh>
#include <scribo/filter/object_groups_v_thickness.hh>

#include <scribo/debug/highlight_text_area.hh>

#include <scribo/debug/decision_image.hh>

#include <scribo/debug/usage.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/preprocessing/rotate_90.hh>

#include <scribo/make/debug_filename.hh>

#include <mln/util/timer.hh>
#include <mln/core/var.hh>


#include <src/afp/components.hh>
#include <src/afp/link.hh>

#include <scribo/core/line_set.hh>
#include <scribo/text/recognition.hh>
#include <scribo/text/look_like_text_lines.hh>


#include <scribo/io/text_boxes/save.hh>


const char *args_desc[][2] =
{
  { "input.*", "An image." },
  { "ouput.ppm", "A color image where the text is highlighted." },
  { "out.txt", "Text recognized text with its position." },
  { "max_dim_size", "The highest dimension size of the image used for "
    "computation. It is used to find a resize factor and impacts on the"
    " performance. (1024 by default)" },
  { "lambda", "Lambda value used for foreground extraction." },
  { "lbl.ppm", "A color image with the labeled text components. (Considered"
    " as optional debug)" },
  {0, 0}
};


namespace mln
{

  struct config
  {
    config()
    {
      min_size_ratio = 0.2f;
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

    // Component filtering
    double min_size_ratio;

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

  if (argc < 4 || argc > 7)
    return scribo::debug::usage(argv,
				"Find text in a photo.\n\n"
				"Common usage: ./text_recognition_in_picture"
				" input.* output.ppm out.txt [max_dim_size]"
				" [lambda] [lbl.ppm]",
				"input.* output.ppm out.txt [max_dim_size]"
				" [lambda] [lbl.ppm]",
				args_desc);


  trace::entering("main");

  Magick::InitializeMagick(*argv);

  image2d<value::rgb8> input_rgb;
  mln::io::magick::load(input_rgb, argv[1]);

  if (argc > 4)
    conf.max_dim_size = atoi(argv[4]);

  unsigned factor = get_factor(input_rgb);

  std::cout << "Original domain: " << input_rgb.domain() << std::endl;

  input_rgb = mln::subsampling::antialiased(input_rgb, factor);

  std::cout << "Resized domain: " << input_rgb.domain() << std::endl;

  unsigned lambda;

  if (argc >= 6)
    lambda = atoi(argv[5]);
  else
    lambda = 1.2 * (input_rgb.nrows() + input_rgb.ncols());

  std::cout << "Using lambda = " << lambda << std::endl;

  image2d<value::int_u8> intensity_ima;

  // Extract foreground
  image2d<value::rgb8>
    fg = preprocessing::split_bg_fg(input_rgb, lambda, 32).second();
  intensity_ima = data::transform(fg,
				  mln::fun::v2v::rgb_to_luma<value::int_u8>());

//   // Perform an initial rotation if needed.
// //   input_rgb = geom::rotate(input_rgb, -45, literal::black);
//   intensity_ima = geom::rotate(intensity_ima, -45);

//   // Make sure the domain origin is set to (0,0).
//   box2d rb = intensity_ima.domain();
//   box2d b(geom::nrows(intensity_ima), geom::ncols(intensity_ima));
// //   {
// //     image2d<value::rgb8> tmp(b);
// //     data::paste_without_localization(input_rgb, tmp);
// //     input_rgb = tmp;
// //   }
//   {
//     image2d<value::int_u8> tmp(b);
//     data::paste_without_localization(intensity_ima, tmp);
//     intensity_ima = tmp;
//   }



  // Binarize foreground to use it in the processing chain.
  image2d<bool> input;
  unsigned w = std::min(intensity_ima.nrows() / 3, intensity_ima.ncols() / 3);
  if (! w % 2)
    ++w;
  w = std::min(w, conf.sauvola_min_w);

  input = scribo::binarization::sauvola_ms(intensity_ima, w, conf.sauvola_s);

  logical::not_inplace(input);


  typedef image2d<value::label_16> L;

  /// Finding components.
  component_set<L> filtered_components;
  {
    mln::util::array<std::pair<box2d, std::pair<point2d, unsigned> > > attribs;

    value::label_16 ncomponents;
    L components = extract_components(input, ncomponents, attribs);

    filtered_components = component_set<L>(components, ncomponents, attribs);
  }

  filtered_components = filter::objects_size_ratio(filtered_components,
						   conf.min_size_ratio);

  /// linking potential components
  mln::util::couple<object_links<L>, object_links<L> >
    links = primitive::link::left_right(filtered_components);

  object_links<L>& left_link = links.first();
  object_links<L>& right_link = links.second();


  // Validating left and right links.
  object_links<L>
    merged_links = primitive::link::merge_double_link(left_link, right_link);



  // Remove links if bboxes have too different sizes.
  object_links<L>
    hratio_filtered_links = filter::object_links_bbox_h_ratio(merged_links,
							      conf.bbox_h_ratio);



  //Remove links if bboxes overlap too much.
  object_links<L> overlap_filtered_links
    = filter::object_links_bbox_overlap(hratio_filtered_links,
					conf.bbox_overlap);


  object_groups<L>
    groups = primitive::group::from_single_link(overlap_filtered_links);


  // Remove components part of groups with strictly less than 3 components.

  object_groups<L> filtered_small_groups;

  filtered_small_groups = filter::object_groups_small(groups,
						      conf.small_groups);


  // Remove components part of groups having a mean thickness lower than 8.
  object_groups<L> filtered_thin_groups;
  filtered_thin_groups
    = filter::object_groups_v_thickness(filtered_small_groups,
					conf.v_thickness);


  // Grouping groups together if possible.
  groups = regroup::from_single_left_link_wrt_h_ratio(filtered_thin_groups,
						      conf.regroup_dmax,
						      conf.bbox_h_ratio);

  /// Filter grouped objects not having enough background components.
  groups  = scribo::filter::object_groups_with_holes(groups,
						     conf.group_min_holes);

  component_set<L> comps = primitive::group::apply(groups);

  if (argc > 6)
    mln::io::ppm::save(mln::labeling::colorize(value::rgb8(),
					       comps.labeled_image(),
					       comps.nelements()),
		       argv[6]);
//   mln::io::ppm::save(scribo::debug::highlight_text_area_rotated(input_rgb,
//  comps, -45, rb),
// 		argv[2]);
  mln::io::ppm::save(scribo::debug::highlight_text_area(input_rgb, comps),
		     argv[2]);


  scribo::line_set<L> lines = scribo::make::line_set(groups);
  text::look_like_text_lines_inplace(lines);
  text::recognition(lines, "fra");

  scribo::io::text_boxes::save(lines, argv[3]);


  trace::exiting("main");
  return comps.nelements() != 0;
}
