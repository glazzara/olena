// Copyright (C) 2010, 2011, 2013 EPITA Research and Development
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

#include <mln/io/magick/load.hh>

#include <scribo/toolchain/text_in_picture.hh>
#include <scribo/debug/option_parser.hh>
#include <scribo/debug/logger.hh>

#include <mln/world/rgb/invert.hh>
#include <scribo/primitive/merge/components.hh>

static const scribo::debug::arg_data arg_desc[] =
{
  { "input.*", "An image." },
  { "output.ppm", "A color image where the text is highlighted." },
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
  { "max-dim-size", "Set the maximum size of the largest image dimension.", "<size>", 0, 1, "1024" },
  { "lambda", "Set the maximum area of the background objects. It is only useful if fg-extraction is enabled.", "<size>",
    0, 1, "0" },
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


  trace::entering("main");

  image2d<value::rgb8> input_rgb;
  io::magick::load(input_rgb, argv[1]);

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
  component_set<L>
    comps = toolchain::text_in_picture(input_rgb, fg_extraction, multi_scale_bin,
				       max_dim_size, lambda, verbose);


  typedef image2d<scribo::def::lbl_type> L;
  component_set<L>
    comps_neg = toolchain::text_in_picture(world::rgb::invert(input_rgb),
					   fg_extraction, multi_scale_bin,
					   max_dim_size, lambda, verbose);


  component_set<L> merged_comps = primitive::merge::components(comps, comps_neg);


  io::ppm::save(mln::labeling::colorize(value::rgb8(),
					merged_comps.labeled_image(),
					merged_comps.nelements()),
		options.arg("output.ppm"));

#  ifndef SCRIBO_NDEBUG
	if (scribo::debug::logger().is_enabled())
	{
	  scribo::debug::logger().log_image(scribo::debug::Results,
					    scribo::debug::highlight_text_area(input_rgb,
									       merged_comps),
					    "input_with_bboxes.ppm");
	  scribo::debug::logger().log_image(scribo::debug::Results,
					    scribo::debug::text_areas_image(input_rgb,
									    merged_comps),
					    "out_text.ppm");
	}
#  endif // ! SCRIBO_NDEBUG

  std::cout << "# objects = " << merged_comps.nelements() << std::endl;

  return merged_comps.nelements() != 0;
}
