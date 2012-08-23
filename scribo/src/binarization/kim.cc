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




double
compute_thres(const mln::image2d<mln::util::couple<double,double> >& integral_sum_sum_2,
	      int row, int col, unsigned win_size,
	      const scribo::binarization::internal::sauvola_formula& formula)
{
  point2d
    tl(row - win_size - 1,
       col - win_size - 1),
    br(row + win_size,
       col + win_size);

  box2d b(tl, br);
  b.crop_wrt(integral_sum_sum_2.domain());

  point2d tr = b.pmax();
  tr.row() = b.pmin().row();
  point2d bl = b.pmin();
  bl.row() = b.pmax().row();

  unsigned card_min = b.nsites() - b.height() - b.width() + 1;

  const mln::util::couple<double,double>&
    D = integral_sum_sum_2(b.pmax()),
    B = integral_sum_sum_2(tr),
    C = integral_sum_sum_2(bl),
    A = integral_sum_sum_2(b.pmin());

  double sum = D.first() - B.first() - C.first() + A.first();
  double sum_2 = D.second() - B.second() - C.second() + A.second();
  double mean = sum / card_min;

  double num = (sum_2 - sum * sum / card_min);
  double stddev;
  if (num > 0)
    stddev = std::sqrt(num / (card_min - 1));
  else
    stddev = 0;

  return formula(mean, stddev);
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

  mln::util::timer t;
  t.start();

  // Convert to Gray level image.
  image2d<value::int_u8>
    input_1_gl = data::transform(input_1,
				 mln::fun::v2v::rgb_to_luma<value::int_u8>());

  // 1st simple binarization
  image2d<bool>
    output = scribo::binarization::sauvola_ms(input_1_gl, w_1, s, k);

  mln::util::timer lt;
  lt.start();

  // Compute integral image
  scribo::util::integral_sum_sum2_functor<value::int_u8,double> f_sum_sum2;
  image2d<mln::util::couple<double,double> >
    integral_sum_sum_2 = scribo::util::init_integral_image(input_1_gl, 1, f_sum_sum2);

  lt.stop();
  std::cout << "integral image - " << lt << std::endl;
  lt.start();

  // Find text lines
  line_set<image2d<scribo::def::lbl_type> >
    lines = scribo::text::extract_lines(output, c8());

  typedef scribo::def::lbl_type V;
  typedef image2d<V> L;
  mln::util::array<unsigned> thickness(lines.nelements() + 1, 0);
  const component_set<L>& comp_set = lines.components();
  const L& lbl = comp_set.labeled_image();

  lt.stop();
  std::cout << "Text line finding - " << lt << std::endl;
  lt.start();

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
  }

  lt.stop();
  std::cout << "run-lengths histogram - " << lt << std::endl;

  lt.start();
  // Compute thresholds for each pixel of each line and binarize again!
  for_all_lines(i, lines)
  {
    if (!lines(i).is_textline())
      continue;

    double
      win_min = thickness(lines(i).id()),
      win_max = lines(i).bbox().height();;

    mln_assertion(win_min != 0);
    mln_assertion(win_max != 0);

    scribo::binarization::internal::sauvola_formula formula;

    for (int row = lines(i).bbox().pmin().row();
	 row <= lines(i).bbox().pmax().row();
	 ++row)
    {
      bool* out_ptr = &output.at_(row, lines(i).bbox().pmin().col());
      value::int_u8* in_ptr = &input_1_gl.at_(row, lines(i).bbox().pmin().col());
      for (int col = lines(i).bbox().pmin().col();
	   col <= lines(i).bbox().pmax().col();
	   ++col)
      {
	// Min case
	double T_min = compute_thres(integral_sum_sum_2, row, col, win_min, formula);

	// Max case
	double T_max = compute_thres(integral_sum_sum_2, row, col, win_max, formula);

	// Final threshold
	double teta = 0.3; // Good results from 0.1 to 0.3 according
			   // to the paper.
	double T = teta * T_max + (1 - teta) * T_min;

	mln_assertion(T_min <= 255);
	mln_assertion(T_max <= 255);
	mln_assertion(T <= 255);

	*out_ptr++ = *in_ptr++ <= T;
      }
    }
  }

  lt.stop();
  std::cout << "Last binarization - " << lt << std::endl;

  t.stop();
  std::cout << "Total time = " << t << std::endl;

  io::pbm::save(output, options.arg("output.pbm"));
}
