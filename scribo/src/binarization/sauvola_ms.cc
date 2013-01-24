// Copyright (C) 2009, 2010, 2011, 2012 EPITA Research and Development
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

/// \file

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/magick/all.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_luma.hh>
#include <mln/util/timer.hh>
#include <mln/logical/not.hh>

#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/debug/option_parser.hh>
#include <scribo/debug/logger.hh>

static const scribo::debug::arg_data arg_desc[] =
{
  { "input.*", "An image." },
  { "output.*", "A binary image." },
  {0, 0}
};


// --enable/disable-<name>
static const scribo::debug::toggle_data toggle_desc[] =
{
  // name, description, default value
  { "negate", "Negate output image.", false},
  {0, 0, false}
};


// --<name> <args>
static const scribo::debug::opt_data opt_desc[] =
{
  // name, description, arguments, check args function, number of args, default arg
  { "debug-prefix", "Enable debug image outputs. Prefix image name with that "
    "given prefix.", "<prefix>", 0, 1, 0 },
  { "all-k", "Sauvola's formulae parameter. Set it globally for all scales.",
    "<value>", 0, 1, "0.34" },

  { "k2", "Force Sauvola's formulae parameter value for scale 2", "<value>", 0, 1, 0 },
  { "k3", "Force Sauvola's formulae parameter value for scale 3", "<value>", 0, 1, 0 },
  { "k4", "Force Sauvola's formulae parameter value for scale 4", "<value>", 0, 1, 0 },


  { "s", "First subsampling ratio. Possible values: 2 or 3.", "ratio",
    scribo::debug::check_sauvola_first_subsampling, 1, "3" },
  { "verbose", "Enable verbose mode (mute, time, low, medium, full)",
    "<mode>", scribo::debug::check_verbose_mode, 1, "mute" },
  { "win-size", "Window size at scale 1", "<size>", 0, 1, "101" },
  {0, 0, 0, 0, 0, 0}
};






int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;

  scribo::debug::option_parser options(arg_desc, toggle_desc, opt_desc);

  if (!options.parse(argc, argv))
    return 1;

  // Enable debug output.
  if (options.is_set("debug-prefix"))
  {
    scribo::debug::logger().set_filename_prefix(options.opt_value("debug-prefix").c_str());
    scribo::debug::logger().set_level(scribo::debug::All);
  }

  Magick::InitializeMagick(*argv);

  trace::entering("main");


  // Window size
  unsigned w_1 = atoi(options.opt_value("win-size").c_str());
  // First subsampling scale.
  unsigned s = atoi(options.opt_value("s").c_str());

  // Setting k parameter.
  double k = atof(options.opt_value("all-k").c_str());
  binarization::internal::k2 = k;
  binarization::internal::k3 = k;
  binarization::internal::k4 = k;

  // Override k parameter for specific scales.
  if (options.is_set("k2"))
    binarization::internal::k2 = atof(options.opt_value("k2").c_str());
  if (options.is_set("k3"))
    binarization::internal::k3 = atof(options.opt_value("k3").c_str());
  if (options.is_set("k4"))
    binarization::internal::k4 = atof(options.opt_value("k4").c_str());

  scribo::debug::logger() << "Using w_1=" << w_1 << " - s=" << s
			  << " - k2=" << binarization::internal::k2
			  << " - k3=" << binarization::internal::k3
			  << " - k4=" << binarization::internal::k4
			  << std::endl;


  // Load
  image2d<value::rgb8> input_1;
  io::magick::load(input_1, options.arg("input.*"));

  // Convert to Gray level image.
  image2d<value::int_u8>
    input_1_gl = data::transform(input_1,
				 mln::fun::v2v::rgb_to_luma<value::int_u8>());


  scribo::debug::logger().start_local_time_logging();

  // Binarize
  image2d<bool>
    output = scribo::binarization::sauvola_ms(input_1_gl, w_1, s);

  scribo::debug::logger().stop_local_time_logging("Binarized in");

  if (options.is_enabled("negate"))
    io::magick::save(logical::not_(output), options.arg("output.*"));
  else
    io::magick::save(output, options.arg("output.*"));
}
