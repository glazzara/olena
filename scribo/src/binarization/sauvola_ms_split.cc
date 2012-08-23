// Copyright (C) 2009, 2010, 2012 EPITA Research and Development
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

#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/magick/load.hh>
#include <mln/io/pbm/save.hh>

#include <scribo/binarization/sauvola_ms_split.hh>
#include <scribo/debug/option_parser.hh>
#include <scribo/debug/logger.hh>

static const scribo::debug::arg_data arg_desc[] =
{
  { "input.*", "An image." },
  { "output.pbm", "A binary image." },
  {0, 0}
};


// --enable/disable-<name>
static const scribo::debug::toggle_data toggle_desc[] =
{
  // name, description, default value
  {0, 0, false}
};


// --<name> <args>
static const scribo::debug::opt_data opt_desc[] =
{
  // name, description, arguments, check args function, number of args, default arg
  { "debug-prefix", "Enable debug image outputs. Prefix image name with that "
    "given prefix.", "<prefix>", 0, 1, 0 },
  { "k", "Sauvola's formulae parameter. Set it globally for all scales.",
    "<value>", 0, 1, "0.34" },

  { "k2", "Sauvola's formulae parameter", "<value>", 0, 1, "0.20" },
  { "k3", "Sauvola's formulae parameter", "<value>", 0, 1, "0.30" },
  { "k4", "Sauvola's formulae parameter", "<value>", 0, 1, "0.50" },

  { "min-ntrue", "The number of components in which a site must be set to 'True' in"
    " order to be set to 'True' in the output (Possible values: 1, 2, 3).",
    "<num>", scribo::debug::check_sauvola_split_ntrue, 1, "2" },
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

  trace::entering("main");

  // Window size
  unsigned w_1 = atoi(options.opt_value("win-size").c_str());  // Scale 1

  // First subsampling scale.
  unsigned s = atoi(options.opt_value("s").c_str());
  double k = atof(options.opt_value("k").c_str());
  unsigned min_ntrue = atoi(options.opt_value("min-ntrue").c_str());

  binarization::internal::k2 = atof(options.opt_value("k2").c_str());
  binarization::internal::k3 = atof(options.opt_value("k3").c_str());
  binarization::internal::k4 = atof(options.opt_value("k4").c_str());

  scribo::debug::logger() << "Using w_1=" << w_1 << " - s=" << s << " - k="
			  << k << " - min_ntrue=" << min_ntrue << std::endl;

  Magick::InitializeMagick(0);

  image2d<value::rgb8> input_1;
  io::magick::load(input_1, options.arg("input.*"));

  image2d<bool>
    output = scribo::binarization::sauvola_ms_split(input_1, w_1, s, min_ntrue, k);

  io::pbm::save(output, options.arg("output.pbm"));
}


