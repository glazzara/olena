// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/alias/neighb2d.hh>
#include <mln/data/stretch.hh>
#include <mln/debug/filename.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/all.hh>
#include <mln/literal/colors.hh>
#include <mln/math/sqr.hh>
#include <mln/subsampling/subsampling.hh>
#include <mln/transform/influence_zone_geodesic_saturated.hh>
#include <mln/util/timer.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/debug/filename.hh>

#include <scribo/core/macros.hh>
#include <scribo/core/object_image.hh>
#include <scribo/filter/objects_small.hh>
#include <scribo/filter/objects_large.hh>

#include <scribo/filter/objects_thin.hh>
#include <scribo/filter/objects_thick.hh>

#include <scribo/primitive/extract/objects.hh>
#include <scribo/binarization/sauvola_threshold.hh>
#include <scribo/debug/usage.hh>
#include <scribo/debug/save_object_diff.hh>

namespace mln
{


// FIXME: do not use scribo's filters since they use object images.
// Object images computes object bounding boxes which are not needed here.
//
//   template <typename L>
//   mln_concrete(L)
//   filter_small_objects(const Image<L>& objects,
// 		       const mln_value(L)& nobjects, unsigned min_size)
//   {
//     card = labeling::compute(card_t(), objects, objects.nlabels());

//     mln_concrete(L) output;
//     initialize(output, objects);

//     fun::i2v::array<bool> f(nobjects.next(), 0);
//     for (unsigned i = 0; i < card.size(); ++i)

//   }

  template <typename I, typename T>
  mln_ch_value(I, bool)
  apply_sauvola(const I& intensity, const T& threshold, unsigned s)
  {
    mln_precondition(intensity.domain() == threshold.domain());

    mln_ch_value(I, bool) output;
    initialize(output, intensity);

    mln_piter(I) p(intensity.domain());
    for_all(p)
      output(p) = (intensity(p) <= threshold(p / s));

    return output;
  }


  template <typename I>
  mln_concrete(I)
  enlarge(const Image<I>& input_, unsigned ratio)
  {
    const I& input = exact(input_);
    mln_precondition(input.is_valid());

    mln_domain(I) bbox(input.domain().pmin() * ratio,
		       input.domain().pmax() * ratio
		       + (ratio - 1) * mln::down_right);

    mln_concrete(I) output(bbox);

    mln_site(I) first_p = input.domain().pmin();
    for (def::coord j = geom::min_col(input);
	 j <= geom::max_col(input); ++j)
      for (def::coord i = geom::min_row(input);
	   i <= geom::max_row(input); ++i)
      {
	point2d p1(i, j);
	point2d p2(first_p[0] + i * ratio, first_p[1] + j * ratio);

	output(p2) = input(p1);
	output(p2 + mln::right) = input(p1);
	output(p2 + mln::down_right) = input(p1);
	output(p2 + mln::down) = input(p1);
      }

    return output;
  }

  template <typename I>
  object_image(mln_ch_value(I, value::label_16))
  get_objects(const Image<I>& input_,
	      unsigned min_size, unsigned max_size,
	      unsigned ratio, unsigned w, unsigned scale)
  {
    const I& input = exact(input_);

    mln_precondition(input.is_valid());

    typedef mln_ch_value(I, value::label_16) L;

    dpoint2d none(0, 0);
    I input_sub = mln::subsampling::subsampling(input, none, ratio);

    image2d<value::int_u8>
      t = scribo::binarization::sauvola_threshold(input_sub, w);

    image2d<bool> b = apply_sauvola(input_sub, t, 1);

    value::label_16 nb;
    object_image(L) lbl = scribo::primitive::extract::objects(b, c8(), nb);
    object_image(L) lbl_raw(lbl);

    if (min_size > 0)
      lbl = scribo::filter::objects_small(lbl, min_size);
    if (max_size > 0)
      lbl = scribo::filter::objects_large(lbl, max_size);
    scribo::debug::save_object_diff(lbl_raw, lbl,
				    mln::debug::filename("filter_diff.ppm", scale + 2));

    return lbl;
  }


  bool
  check_args(int argc, char * argv[])
  {
    if (argc < 7)
      return false;

    int nb_scale = atoi(argv[3]);
    int s = atoi(argv[4]);
    int q = atoi(argv[5]);

    if (q < 2)
    {
      std::cout << "q must be greater than 2." << std::endl;
      return false;
    }
    if (s < 1 || s < q)
    {
      std::cout << "s must be greater or equal to 1 and greater than q."
		<< std::endl;
      return false;
    }

    if (nb_scale < 1)
    {
      std::cout << "Not enough scales." << std::endl;
      return false;
    }

    if ((argc - 7) != (nb_scale - 1))
    {
      std::cout << "Not enough area threshold."
		<< "There must be nb_scale - 1 thresholds."
		<< std::endl;
      return false;
    }

    return true;
  }


} // end of namespace mln;




const char *args_desc[][2] =
{
  { "input.pgm", "A graylevel image." },
  { "w", "Window size." },
  { "nb_scale",    "Number of scales (Common value: 3)." },
  { "s", "First subsampling ratio (Common value: 2)." },
  { "q", "Next subsampling ratio (Common value: 2)." },
  { "<area threshold> (Common values: 200 800)",    "Area threshold" },
  {0, 0}
};




int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace scribo;
  using value::rgb8;
  using value::int_u8;
  using value::int_u16;
  using value::label_16;

  typedef image2d<label_16> L;


  if (!check_args(argc, argv))
    return scribo::debug::usage(argv,
				"Binarization of a color image based on Sauvola's algorithm.",
				"input.pgm w nb_scale s q <area thresholds>",
				args_desc, "A binary image.");

  trace::entering("main");

  mln::debug::internal::filename_prefix = argv[1];

  // Window size.
  unsigned w = atoi(argv[2]);

  // First subsampling scale.
  unsigned s = atoi(argv[4]);

  // Number of subscales.
  unsigned nb_subscale = atoi(argv[3]);

  // Subscale step.
  unsigned q = atoi(argv[5]);

  std::cout << "Running Sauvola_ms with w = " << w
	    << ", s = " << s
	    << ", nb_subscale = " << nb_subscale
	    << ", q = " << q
	    << std::endl;

  typedef image2d<value::int_u8> I;
  dpoint2d none(0, 0);

  mln::util::timer timer_;

  timer_.start();
  I input_full;
  io::pgm::load(input_full, argv[1]);
  std::cout << "Image loaded - " << timer_ << std::endl;

  timer_.restart();

  util::array<object_image(L)> lbls;



  // First subsampling 1/s

//  std::cout << "1/" << s << std::endl;
  timer_.start();
  I input = mln::subsampling::subsampling(input_full, none, s);

  integral_image<int_u8> simple, squared;
  image2d<int_u8>
    t_1 = scribo::binarization::sauvola_threshold(input, w, simple, squared);

  label_16 nb1;
  image2d<bool> b_1 = apply_sauvola(input, t_1, 1);

  {
    lbls.append(primitive::extract::objects(b_1, c8(), nb1));
    object_image(L) lbl_raw(lbls[0]);
    lbls[0] = filter::objects_large(lbls[0], atoi(argv[6]));
    scribo::debug::save_object_diff(lbl_raw, lbls[0],
				    mln::debug::filename("filter_diff.ppm", 2));
  }

  std::cout << "Scale 2 - 1/" << s << " Done - " << timer_ << std::endl;




  // Additional subscales.
  for (unsigned i = 1; i < nb_subscale; ++i)
  {
    unsigned ratio = std::pow(float(q), int(i));
//    std::cout << "Scale " << 2 + i << " - 1/" << s * ratio << std::endl;
    timer_.restart();
    unsigned
      min_size = atoi(argv[5 + i]) / ratio,
      max_size;
    if ((3 + i) == static_cast<unsigned>(argc))
      max_size = 0; // Last subscale, so not max size limit.
    else
      max_size = atoi(argv[6 + i]) / ratio;

    lbls.append(get_objects(input, min_size, max_size, ratio, w, i));
    std::cout << "Scale " << 2 + i
	      << " - 1/" << s * ratio
	      << " Done - " << timer_ << std::endl;
  }

  std::cout << "--------" << std::endl;

  // Constructing "scale image".
  image2d<int_u8> e;
  initialize(e, input);

  data::fill(e, 0);

  typedef object_image(L) obj_t;

  for (int i = nb_subscale - 1; i >= 0; --i)
  {
    unsigned ratio = std::pow(float(q), int(i));

    std::cout << "Scale " << 2 + i << " - 1/" << s * ratio << " merged" << std::endl;

    mln_piter_(obj_t) p(lbls[i].domain());
    for_all(p)
      if (lbls[i](p) != 0 && e(p * ratio) == 0)
      {
	box2d b(p * ratio, p * ratio + mln::down_right * (ratio - 1));
	data::fill((e | b).rw(), i + 1);
      }
  }

  std::cout << "--------" << std::endl;

  /// Saving "scale image".
  {
    mln::fun::i2v::array<value::int_u8> f(nb_subscale + 1, 0);
    for (unsigned i = 1; i <= nb_subscale; ++i)
      f(i) = 255 / nb_subscale * i;

    io::pgm::save(e, "e_raw.pgm");
    io::pgm::save(data::transform(e, f), "e.pgm");
  }


  /// Saving influence zone scale image.
  image2d<int_u8> e_ext =
    transform::influence_zone_geodesic_saturated(e, c8(), mln_max(unsigned));
  io::pgm::save(e_ext, "e_ext.pgm");


  /// Creating window size image
  std::cout << "Creating window size image..." << std::endl;
  mln::fun::i2v::array<value::int_u16> f(nb_subscale, 0);
  for (unsigned i = 0; i < nb_subscale; ++i)
  {
    unsigned ratio = std::pow(float(q), int(i));
    f(i) = ratio * w;
  }

  image2d<int_u16> wsize = data::transform(e_ext, f);
  io::pgm::save(data::stretch(int_u8(), wsize), "wsize.pgm");



  /// Constructing threshold image
  image2d<int_u8> t;
  initialize(t, input);

  std::cout << "Computing threshold image..." << std::endl;
  timer_.restart();
  mln_piter_(L) p(input.domain());
  for_all(p)
    t(p) = binarization::internal::compute_sauvola_threshold(p,
							     simple,
							     squared,
							     wsize(p));
  std::cout << "Compute t Done - " << timer_ << std::endl;

  io::pgm::save(t, "t.pgm");


  timer_.restart();

  /// Applying threshold image and save.
  io::pbm::save(apply_sauvola(input_full, t, s), argv[argc - 1]);
  std::cout << "sauvola applied and saved Done - " << timer_ << std::endl;


  trace::exiting("main");
}
