#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/pw/all.hh>
#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/neighb.hh>
#include <mln/win/rectangle2d.hh>
#include <mln/extension/adjust.hh>
#include <mln/border/mirror.hh>
#include <mln/extension/duplicate.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/data/convert.hh>
#include <mln/data/stretch.hh>
#include <mln/debug/println.hh>
#include <mln/data/transform.hh>
#include <mln/io/dump/save.hh>
#include <mln/accu/math/sumpow.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/data/compute_in_window.hh>
#include <mln/core/var.hh>
#include <mln/data/saturate.hh>
#include <mln/fun/v2b/threshold_le.hh>
#include <mln/arith/revert.hh>
#include <mln/util/timer.hh>

mln::image2d<double> skewness;

#include <scribo/binarization/sauvola_ms.hh>
#include "integral_browsing_rv.hh"

mln::image2d<bool> skewness_pbm;
std::string prefix;


namespace scribo
{
  using namespace mln;

  template <typename I, typename J>
  void
  integral_rv(const Image<I>& input_,
	      Image<J>& integral_sum_sum_2_sum_3_)
  {
    trace::entering("subsampling::impl::integral_2");

    const unsigned scale = 1;

    const I& input = exact(input_);
    J& integral_sum_sum_2_sum_3 = exact(integral_sum_sum_2_sum_3_);

    typedef mln_value(I) V;
    typedef mln_sum(V) S;
    typedef mln_site(I) P;
    typedef mln_value(J) V2;

    mlc_bool(P::dim == 2)::check();
    mln_precondition(input.is_valid());
    mln_precondition(input.domain().pmin() == literal::origin);

    initialize(integral_sum_sum_2_sum_3, input);
    V2* p_integ = integral_sum_sum_2_sum_3.buffer();

    const int up = integral_sum_sum_2_sum_3.delta_index(dpoint2d(-1, 0));

    const unsigned nrows = input.nrows();
    const unsigned ncols = input.ncols();

    unsigned border_thickness = input.border();
    unsigned b_offset = integral_sum_sum_2_sum_3.delta_index(dpoint2d(border_thickness,
								      border_thickness));
    p_integ += b_offset;

    unsigned row = 0;
    {
      S h_sum = 0, h_sum_2 = 0, h_sum_3 = 0;
      const V* ptr1 = & input.at_(row, 0);
      for (unsigned col = 0; col < ncols; ++col)
      {
	V v11 = *ptr1++;
	h_sum   += v11;
	h_sum_2 += v11 * v11;
	h_sum_3 += v11 * v11 * v11;

	// exception
	p_integ->first() = h_sum;
	p_integ->second().first() = h_sum_2;
	p_integ->second().second() = h_sum_3;

	++p_integ;
      }
    }

    unsigned b_next = 2 * border_thickness;

    p_integ += b_next;

    for (row += scale; row < nrows; ++row)
    {
      S h_sum = 0, h_sum_2 = 0, h_sum_3 = 0;
      const V* ptr1 = & input.at_(row, 0);
      for (unsigned col = 0; col < ncols; ++col)
      {
	V v11 = *ptr1++;
	h_sum   += v11;
	h_sum_2 += v11 * v11;
	h_sum_3 += v11 * v11 * v11;

	p_integ->first() = h_sum + (p_integ + up)->first();
	p_integ->second().first() = h_sum_2 + (p_integ + up)->second().first();
	p_integ->second().second() = h_sum_3 + (p_integ + up)->second().second();

	++p_integ;
      }

      p_integ += b_next;
    }

    trace::exiting("subsampling::impl::integral_2");
  }

} // end of namespace scribo


namespace mln
{

  template <typename I>
  struct invert_on_skewness
  {
    I input;
    image2d<double> skewness_;
    mln_fwd_pixter(I) pxl;

    invert_on_skewness(const I& input_)
      : input(duplicate(input_)),
	pxl(input)
    {
      pxl.start();
      initialize(skewness_, input);
    }

    void exec(double skewness)
    {
      skewness_.element(pxl.offset()) = skewness;

      if (skewness > 1000.)
	pxl.val() = 255 - pxl.val();

      pxl.next(); // next pixel
    }

    void finalize()
    {
    }
  };

}

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <input.pgm> <prefix> <win_size>" << std::endl;
    return 1;
  }

  util::timer tt;
  tt.start();
  util::timer t;

  t.start();
  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);

  prefix = argv[2];

  int win_size = atoi(argv[3]);
  t.stop();
  std::cout << "Initialization - " << t << std::endl;

  // invert data if skewness > 0
  t.restart();

  image2d<util::couple<double, util::couple<double, double> > > integral_sum_sum_2_sum_3;
  scribo::integral_rv(input, integral_sum_sum_2_sum_3);

  invert_on_skewness<image2d<value::int_u8> > f(input);
  scribo::canvas::integral_browsing_rv(integral_sum_sum_2_sum_3, win_size, win_size, f);

  t.stop();
  std::cout << "invert on skewness - " << t << std::endl;

  io::dump::save(f.skewness_, prefix + "skewness.dump");

  t.restart();
  image2d<bool> bin = scribo::binarization::sauvola_ms(f.input, 101, 2);
  std::cout << "sauvola_ms - " << t << std::endl;


  std::cout << "Total time : " << tt << std::endl;
  // prefix += "i_";

  // image2d<bool> bin_i = scribo::binarization::sauvola(arith::revert(input), 51);

  // prefix = argv[2];

  io::pbm::save(bin, prefix + "bin.pbm");
  // io::pbm::save(bin_i, prefix + "bin_i.pbm");

  // image2d<bool> out;
  // initialize(out, bin);
  // mln_piter_(image2d<bool>) p(out.domain());
  // for_all(p)
  //   if (skewness_pbm(p))
  //     out(p) = bin(p);
  //   else
  //     out(p) = bin_i(p);

  // io::pbm::save(out, prefix + "output.pbm");

  // io::dump::save(scribo::binarization::internal::debug_stddev, prefix + "stddev.dump");
  // io::dump::save(scribo::binarization::internal::debug_mean, prefix + "mean.dump");
  // io::dump::save(scribo::binarization::internal::debug_threshold, prefix + "threshold.dump");

}
