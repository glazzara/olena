// Copyright (C) 2009, 2010, 2011, 2012, 2013 EPITA Research and
// Development Laboratory (LRDE)
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
#include <mln/value/int_u8.hh>
#include <mln/io/magick/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_luma.hh>
#include <mln/data/paste.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/data/convert.hh>
#include <mln/labeling/foreground.hh>
#include <mln/literal/colors.hh>

#include <scribo/binarization/sauvola_ms.hh>
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
  { "all-k", "Sauvola's formulae parameter", "<value>", 0, 1, "0.34" },

  { "k2", "Sauvola's formulae parameter", "<value>", 0, 1, 0 },
  { "k3", "Sauvola's formulae parameter", "<value>", 0, 1, 0 },
  { "k4", "Sauvola's formulae parameter", "<value>", 0, 1, 0 },

  { "s", "First subsampling ratio. Possible values: 2 or 3.", "ratio",
    scribo::debug::check_sauvola_first_subsampling, 1, "3" },
  { "verbose", "Enable verbose mode (mute, time, low, medium, full)",
    "<mode>", scribo::debug::check_verbose_mode, 1, "mute" },
  { "win-size", "Window size at scale 1", "<size>", 0, 1, "101" },
  {0, 0, 0, 0, 0, 0}
};


namespace mln
{

  struct scale_to_color : public Function_v2v<scale_to_color>
  {
    typedef value::rgb8 result;

    value::rgb8 operator()(const value::int_u8& v) const
    {
      switch (v)
      {
	default:
	case 0:
	  return literal::black;

	case 2:
	  return literal::green;

	case 3:
	  return literal::blue;

	case 4:
	  return literal::red;
      }
    }

  };

}


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

  mln_trace("main");


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

  scribo::binarization::internal::scale_image_output = "scale_image.pgm";
  scribo::binarization::internal::threshold_image_output = "threshold_image.pbm";
  scribo::binarization::internal::full_threshold_image_output = "full_threshold_image.pbm";
  scribo::binarization::internal::scale_iz_image_output = "scale_iz.pgm";
  scribo::binarization::internal::scale_proba_output = "scale_proba.dump";
  const char *scale_bin_output = "scale_bin.ppm";

  // Load
  image2d<value::rgb8> input_1;
  io::magick::load(input_1, options.arg("input.*"));

  // Convert to Gray level image.
  image2d<value::int_u8>
    input_1_gl = data::transform(input_1,
				 mln::fun::v2v::rgb_to_luma<value::int_u8>());

  scribo::debug::logger().start_time_logging();

  // Binarize.
  image2d<bool>
    output = scribo::binarization::sauvola_ms(input_1_gl, w_1, s);

  scribo::debug::logger().stop_time_logging("Binarized in");

#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
  {
    image2d<value::int_u8> scale_bin;
    initialize(scale_bin, output);

    for (unsigned i = 0; i < geom::nrows(output); ++i)
      for (unsigned j = 0; j < geom::ncols(output); ++j)
	if (output.at_(i,j))
	  scale_bin.at_(i,j) = binarization::internal::debug_e_2.at_(i/s, j/s);
	else
	  scale_bin.at_(i,j) = 0;

    scale_to_color f;
    io::ppm::save(data::transform(scale_bin, f),
		  mln::debug::filename(scale_bin_output));

    // Computing size of retrieved objects for each scale.
    unsigned max = 15000;
    std::ofstream ostr(mln::debug::filename("area_per_scale.tsv").c_str());
    for (unsigned j = 0; j < max; ++j)
    {
      ostr << j;
      for (unsigned i = 0; i < 3; ++i)
      {
	if (binarization::internal::area_histo[i].find(j) != binarization::internal::area_histo[i].end())
	  ostr << " " << binarization::internal::area_histo[i][j];
	else
	  ostr << " " << 0;
      }
      ostr << std::endl;
    }
    ostr.close();


  }
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG


  io::pbm::save(output, options.arg("output.pbm"));
}


