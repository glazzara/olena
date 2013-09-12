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

#include <libgen.h>
#include <iostream>

#include <mln/io/magick/all.hh>

#include <scribo/toolchain/text_in_picture.hh>
#include <scribo/debug/option_parser.hh>
#include <scribo/debug/logger.hh>


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
  { "alternate-results", "Save debug images with all the text bboxes. (default: disabled)", false },
  { "debug", "Enable debug outputs (default: disabled).", false },
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
  using namespace mln;

  scribo::debug::option_parser options(arg_desc, toggle_desc, opt_desc);

  if (!options.parse(argc, argv))
    return 1;

  if (options.is_set("debug-prefix"))
    scribo::debug::logger().set_filename_prefix(options.opt_value("debug-prefix").c_str());

  if (options.is_enabled("debug"))
    scribo::debug::logger().set_level(scribo::debug::All);

  mln_trace("main");

  image2d<value::rgb8> input_rgb;
  io::magick::load(input_rgb, options.arg("input.*"));

  bool verbose = options.is_set("verbose");
  unsigned max_dim_size = atoi(options.opt_value("max-dim-size").c_str());
  bool fg_extraction = options.is_enabled("fg-extraction");
  bool multi_scale_bin = options.is_enabled("ms-bin");
  unsigned lambda = atoi(options.opt_value("lambda").c_str());

  if (options.is_enabled("alternate-results"))
    scribo::debug::logger().set_level(scribo::debug::Results);

  if (verbose)
    std::cout << "Using max_dim_size = " << max_dim_size
	      << " -  fg_extraction = "  << fg_extraction
	      << " - multi_scale_bin = " << multi_scale_bin
	      << " - lambda = " << lambda << std::endl;

  component_set<image2d<scribo::def::lbl_type> >
    comps = toolchain::text_in_picture(input_rgb,
				       fg_extraction, multi_scale_bin,
				       max_dim_size, lambda, verbose);


  io::magick::save(mln::labeling::colorize(value::rgb8(),
					   comps.labeled_image(),
					   comps.nelements()),
		   options.arg("output.ppm"));
}
