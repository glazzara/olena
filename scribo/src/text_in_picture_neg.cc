// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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
#include <mln/core/image/imorph/tr_image.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/colorize.hh>

#include <mln/data/stretch.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/magick/all.hh>

#include <mln/arith/revert.hh>

#include <mln/math/min.hh>

#include <mln/logical/not.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/label_16.hh>

#include <mln/data/wrap.hh>

#include <mln/draw/box.hh>

#include <mln/geom/translate.hh>

#include <mln/subsampling/antialiased.hh>

#include <mln/world/rgb/invert.hh>

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

#include <scribo/filter/common/objects_photo.hh>

#include <scribo/filter/object_groups_small.hh>
#include <scribo/filter/object_groups_v_thickness.hh>

#include <scribo/debug/highlight_text_area.hh>
#include <scribo/debug/text_areas_image.hh>

#include <scribo/debug/decision_image.hh>

#include <scribo/debug/usage.hh>

#include <scribo/preprocessing/split_bg_fg.hh>

#include <scribo/make/debug_filename.hh>

#include <scribo/toolchain/text_in_picture.hh>

#include <scribo/primitive/merge/components.hh>

#include <mln/util/timer.hh>
#include <mln/core/var.hh>

const char *args_desc[][2] =
{
  { "input.*", "A color image." },
  { "ouput.ppm", "A color image where the text is highlighted." },
  { "debug_output_dir", "Directory were debug images will be saved" },
  { "lambda", "Lambda value used for foreground extraction" },
  {0, 0}
};



int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace scribo::primitive;
  using namespace mln;

  if (argc < 3 || argc > 8)
    return scribo::debug::usage(argv,
				"Find text in a photo.\n\n"
				"Common usage: ./text_in_photo_fast input.*"
				" output.ppm 1 1",
				"input.ppm output.ppm <bg/fg enabled>"
				" <sauvola_ms enabled> "
				"[debug_output_dir] [max_dim_size] [lambda]",
				args_desc);

  char *out_base_dir = 0;
  if (argc > 5)
    out_base_dir = argv[5];

  trace::entering("main");

  Magick::InitializeMagick(*argv);

  image2d<value::rgb8> input_rgb;
  io::magick::load(input_rgb, argv[1]);

  unsigned max_dim_size = 0;
  if (argc >= 7)
    max_dim_size = atoi(argv[6]);


  unsigned lambda = 0;
  if (argc == 8)
    lambda = atoi(argv[7]);

  bool bg_removal = false;
  if (argc > 3 && atoi(argv[3]) != 0)
    bg_removal = true;

  bool multi_scale_bin = false;
  if (argc > 4 && atoi(argv[4]) != 0)
    multi_scale_bin = true;


  typedef image2d<value::label_16> L;
  component_set<L>
    comps = toolchain::text_in_picture(input_rgb, bg_removal, multi_scale_bin,
				       max_dim_size, lambda, out_base_dir);


  typedef image2d<value::label_16> L;
  component_set<L>
    comps_neg = toolchain::text_in_picture(world::rgb::invert(input_rgb),
					   bg_removal,
					   multi_scale_bin,
					   max_dim_size, lambda, out_base_dir);


  component_set<L> merged_comps = primitive::merge::components(comps, comps_neg);



  io::ppm::save(mln::labeling::colorize(value::rgb8(),
					merged_comps.labeled_image(),
					merged_comps.nelements()),
		argv[2]);

  if (out_base_dir)
  {
    io::ppm::save(scribo::debug::highlight_text_area(input_rgb, merged_comps),
		  std::string(out_base_dir) + "_input_with_bboxes.ppm");
    io::ppm::save(scribo::debug::text_areas_image(input_rgb, merged_comps),
		  std::string(out_base_dir) + "_out_text.ppm");
  }

  std::cout << "# objects = " << merged_comps.nelements() << std::endl;

  return merged_comps.nelements() != 0;
}
