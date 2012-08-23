// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

#include <cmath>
#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/magick/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/transform.hh>
#include <mln/fun/v2v/rgb_to_luma.hh>

#include <scribo/binarization/sauvola_ms.hh>
#include <scribo/debug/option_parser.hh>
#include <scribo/debug/logger.hh>

#include <scribo/util/integral_sum_sum2_functor.hh>

#include <scribo/text/extract_lines.hh>
#include <scribo/debug/bboxes_enlarged_image.hh>
#include <algorithm>


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
  { "k", "Sauvola's formulae parameter", "<value>", 0, 1, "0.34" },
  { "s", "First subsampling ratio. Possible values: 2 or 3.", "ratio",
    scribo::debug::check_sauvola_first_subsampling, 1, "3" },
  { "verbose", "Enable verbose mode", 0, 0, 0, 0 },
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


  bool verbose = options.is_set("verbose");
  // Window size
  unsigned w_1 = atoi(options.opt_value("win-size").c_str());
  // First subsampling scale.
  unsigned s = atoi(options.opt_value("s").c_str());
  double k = atof(options.opt_value("k").c_str());

  if (verbose)
    std::cout << "Using w_1=" << w_1 << " - s=" << s
	      << " - k=" << k << std::endl;

  // Load
  image2d<value::rgb8> input_1;
  io::magick::load(input_1, options.arg("input.*"));

  // Convert to Gray level image.
  image2d<value::int_u8>
    input_1_gl = data::transform(input_1,
				 mln::fun::v2v::rgb_to_luma<value::int_u8>());

  // 1st simple binarization
  image2d<bool>
    output = scribo::binarization::sauvola_ms(input_1_gl, w_1, s, k);

  io::pbm::save(output, "debug_bin.pbm");

  // Compute integral image
  scribo::util::integral_sum_sum2_functor<value::int_u8,double> f_sum_sum2;
  image2d<mln::util::couple<double,double> >
    integral_sum_sum_2 = scribo::util::init_integral_image(input_1_gl, 1, f_sum_sum2);

  // Find text lines
  line_set<image2d<scribo::def::lbl_type> >
    lines = scribo::text::extract_lines(output, c8());

  typedef scribo::def::lbl_type V;
  typedef image2d<V> L;
  mln::util::array<unsigned> thickness(lines.nelements() + 1, 0);
  const component_set<L>& comp_set = lines.components();
  const L& lbl = comp_set.labeled_image();

  // Compute run-lengths histogram in order to compute character
  // thickness for each line.
  for_all_lines(i, lines)
  {
    if (!lines(i).is_textline())
      continue;

    std::map<unsigned, unsigned> histo;
    int count = 0;
    for (int l = lines(i).bbox().pmin().row(); l <= lines(i).bbox().pmax().row(); ++l)
    {
      const V* end_ptr = &lbl.at_(l, lines(i).bbox().pmax().col() + 1);
      for (const V* run_ptr = &lbl.at_(l, lines(i).bbox().pmin().col()); run_ptr != end_ptr; ++run_ptr)
	if (*run_ptr)
	  ++count;
	else
	  if (count)
	  {
	    if (histo.find(count) != histo.end())
	      histo[count]++;
	    else
	      histo.insert(std::make_pair(count, 1));
	    count = 0;
	  }
    }

    unsigned max = 0;
    unsigned thick = 0;
    for (std::map<unsigned, unsigned>::const_iterator it = histo.begin(); it != histo.end(); ++it)
      if (it->second > max)
      {
	max = it->second;
	thick = it->first;
      }

    thickness(lines(i).id()) = thick;
    std::cout << "line " << i << " - thickness = " << thick << " - height = " << lines(i).x_height() << std::endl;
  }

  // std::cout << "integral_sum_sum_2.domain() = " << integral_sum_sum_2.domain() << std::endl;

  // FIXME: To be removed.
  extension::adjust_duplicate(integral_sum_sum_2, 50);

  for_all_lines(i, lines)
  {
    if (!lines(i).is_textline())
      continue;

    math::round<double> round;
    double
      win_min = round(std::max(thickness(lines(i).id()),2u) / 2.0),
      win_max = round(std::max(lines(i).x_height(),2u) / 2.0),
      card_min,
      card_max;

    if (win_min == 0)
    {
      std::cout << "win_min == 0 - thickness = " << thickness(lines(i).id()) << std::endl;
      abort();
    }

    if (win_max == 0)
    {
      std::cout << "win_max == 0 - x_height = " << lines(i).x_height() << std::endl;
      abort();
    }


    binarization::internal::sauvola_formula compute_thres;


    // std::cout << "<<<<<<<<<<<<<< New line" << std::endl;

    mln_piter(L) p(lines(i).bbox());
    for_all(p)
    {
      accu::shape::bbox<point2d> accu;
      point2d tl, br;

      // Min case
      tl.row() = (p.row() - win_min - 1);
      tl.col() = (p.col() - win_min - 1);

      br.row() = (p.row() + win_min);
      br.col() = (p.col() + win_min);

      accu.take(tl);
      accu.take(br);
      box2d b = accu.to_result();
      b.crop_wrt(integral_sum_sum_2.domain());

      point2d tr = b.pmax();
      tr.row() = b.pmin().row();
      point2d bl = b.pmin();
      bl.row() = b.pmax().row();

      {
	accu::shape::bbox<point2d> accu;
	point2d tl, br;

	tl.row() = (p.row() - win_min);
	tl.col() = (p.col() - win_min);

	br.row() = (p.row() + win_min);
	br.col() = (p.col() + win_min);

	accu.take(tl);
	accu.take(br);
	box2d b = accu.to_result();
	b.crop_wrt(input_1_gl.domain());
	card_min = b.nsites();
      }

      // std::cout << "p = " << p << " - box = " << b << " - nsites = " << b.nsites() << " - card_min = " << card_min << std::endl;

      double sum = integral_sum_sum_2(b.pmax()).first() - integral_sum_sum_2(tr).first() - integral_sum_sum_2(bl).first() + integral_sum_sum_2(b.pmin()).first();
      double sum_2 = integral_sum_sum_2(b.pmax()).second() - integral_sum_sum_2(tr).second() - integral_sum_sum_2(bl).second() + integral_sum_sum_2(b.pmin()).second();
      double mean = sum / card_min;

      double num = (sum_2 - sum * sum / card_min);
      double stddev = 0;
      if (num > 0)
	  stddev = std::sqrt(num / (card_min - 1));
      else
	stddev = 0;


      double T_min = compute_thres(mean, stddev);

      // Max case
      accu.init();
      tl.row() = (p.row() - win_max - 1);
      tl.col() = (p.col() - win_max - 1);

      br.row() = (p.row() + win_max);
      br.col() = (p.col() + win_max);

      accu.take(tl);
      accu.take(br);
      b = accu.to_result();
      b.crop_wrt(integral_sum_sum_2.domain());

      tr = b.pmax();
      tr.row() = b.pmin().row();
      bl = b.pmin();
      bl.row() = b.pmax().row();

      {
	accu::shape::bbox<point2d> accu;
	point2d tl, br;

	tl.row() = (p.row() - win_max);
	tl.col() = (p.col() - win_max);

	br.row() = (p.row() + win_max);
	br.col() = (p.col() + win_max);

	accu.take(tl);
	accu.take(br);
	box2d b = accu.to_result();
	b.crop_wrt(input_1_gl.domain());
	card_max = b.nsites();

	// std::cout << "card_max = " << card_max << " - b = " << b << std::endl;
      }

      sum = integral_sum_sum_2(b.pmax()).first() - integral_sum_sum_2(tr).first() - integral_sum_sum_2(bl).first() + integral_sum_sum_2(b.pmin()).first();
      sum_2 = integral_sum_sum_2(b.pmax()).second() - integral_sum_sum_2(tr).second() - integral_sum_sum_2(bl).second() + integral_sum_sum_2(b.pmin()).second();
      mean = sum / card_max;

      num = (sum_2 - sum * sum / card_max);
      stddev = 0;
      if (num > 0)
	  stddev = std::sqrt(num / (card_max - 1));
      else
	stddev = 0;

      double T_max = compute_thres(mean, stddev);


      // Final threshold

      double teta = 0.3;
      double T = teta * T_max + (1 - teta) * T_min;

//      std::cout << "bbox = " << lines(i).bbox() << " - box = " << b << " - T_max = " << T_max << " - T_min = " << T_min << " -  T = " << T << std::endl;
      mln_assertion(T_min <= 255);
      mln_assertion(T_max <= 255);
      mln_assertion(T <= 255);

      output(p) = input_1_gl(p) <= T;
    }

  }


  image2d<value::rgb8> bbox = scribo::debug::bboxes_enlarged_image(output, lines);
  io::ppm::save(bbox, "debug_bbox.ppm");


  io::pbm::save(output, options.arg("output.pbm"));
}
