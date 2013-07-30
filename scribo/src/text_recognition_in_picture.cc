// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
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

#include <mln/data/stretch.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/magick/all.hh>

#include <mln/math/min.hh>

#include <mln/logical/not.hh>

#include <mln/literal/colors.hh>
#include <mln/value/rgb8.hh>

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
#include <scribo/filter/object_groups_mean_width.hh>

#include <scribo/debug/highlight_text_area.hh>

#include <scribo/debug/decision_image.hh>

#include <scribo/debug/option_parser.hh>

#include <scribo/preprocessing/split_bg_fg.hh>
#include <scribo/preprocessing/rotate_90.hh>

#include <scribo/debug/logger.hh>
#include <scribo/toolchain/text_in_picture.hh>

#include <mln/util/timer.hh>
#include <mln/core/var.hh>


#include <scribo/afp/components.hh>
#include <scribo/afp/link.hh>

#include <scribo/core/line_set.hh>
#include <scribo/text/recognition.hh>
#include <scribo/text/look_like_text_lines.hh>


#include <scribo/io/text_boxes/save.hh>


static const scribo::debug::arg_data arg_desc[] =
{
  { "input.*", "An image." },
  { "output.ppm", "A color image where the text is highlighted." },
  { "output.txt", "Recognized text with its position." },
  {0, 0}
};

// --enable/disable-<name>
static const scribo::debug::toggle_data toggle_desc[] =
{
  // name, description, default value
  { "fg-extraction", "Detect and slit foreground/background components. (default: disabled)", false },
  { "ms-bin", "Use a multi-scale binarization. (default: enabled)", true },
  {0, 0, false}
};


// --<name> <args>
static const scribo::debug::opt_data opt_desc[] =
{
  // name, description, arguments, check args function, number of args, default arg
  { "debug-prefix", "Enable debug image outputs. Prefix image name with that "
    "given prefix.", "<prefix>", 0, 1, 0 },
  { "lambda", "Set the maximum area of the background objects. It is only useful if fg-extraction is enabled.", "<size>",
    0, 1, "0" },
  { "max-dim-size", "Set the maximum size of the largest image dimension.", "<size>", 0, 1, "1024" },
  { "ocr-lang", "Set the language to be recognized by the OCR (Tesseract). "
    "According to your system, you can choose between eng (default), "
    "fra, deu, ita, nld, por, spa, vie",
    "<lang>", scribo::debug::check_ocr_lang, 1, "eng" },
  { "verbose", "Enable verbose mode", 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0}
};




int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace scribo::primitive;
  using namespace mln;

  scribo::debug::option_parser options(arg_desc, toggle_desc, opt_desc);

  if (!options.parse(argc, argv))
    return 1;

  if (options.is_set("debug-prefix"))
  {
    scribo::debug::logger().set_filename_prefix(options.opt_value("debug-prefix").c_str());
    scribo::debug::logger().set_level(scribo::debug::All);
  }

  mln_trace("main");

  typedef image2d<value::rgb8> I;
  I input_rgb;
  mln::io::magick::load(input_rgb, argv[1]);



  bool verbose = options.is_set("verbose");
  unsigned max_dim_size = atoi(options.opt_value("max-dim-size").c_str());
  bool fg_extraction = options.is_enabled("fg-extraction");
  bool multi_scale_bin = options.is_enabled("ms-bin");
  unsigned lambda = atoi(options.opt_value("lambda").c_str());

  if (verbose)
    std::cout << "Using max_dim_size = " << max_dim_size
	      << " -  fg_extraction = "  << fg_extraction
	      << " - multi_scale_bin = " << multi_scale_bin
	      << " - lambda = " << lambda << std::endl;

  typedef image2d<scribo::def::lbl_type> L;

  toolchain::internal::text_in_picture_functor<I> f;
  f.enable_bg_removal = fg_extraction;
  f.enable_multi_scale_bin = multi_scale_bin;
  f.max_dim_size = max_dim_size;
  f.lambda = lambda;
  f.verbose = verbose;

  component_set<L> output = f(input_rgb);

  // // Grouping groups together if possible.
  // groups = regroup::from_single_left_link_wrt_h_ratio(filtered_thin_groups,
  // 						      conf.regroup_dmax,
  // 						      conf.bbox_h_ratio);

  mln::io::ppm::save(scribo::debug::highlight_text_area(input_rgb, output),
		     options.arg("output.ppm"));

  scribo::line_set<L> lines = scribo::make::line_set(f.groups);
  text::look_like_text_lines_inplace(lines);
  text::recognition(lines, options.opt_value("ocr-lang").c_str());
  scribo::io::text_boxes::save(lines, options.arg("output.txt"));

  if (verbose)
    std::cout << output.nelements() << " text lines found." << std::endl;

  return 0;
}
