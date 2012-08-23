#include <mln/core/image/image2d.hh>
#include <scribo/canvas/integral_browsing.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/util/couple.hh>
#include <mln/util/timer.hh>
#include <scribo/binarization/internal/sauvola_formula.hh>
#include <scribo/util/init_integral_image.hh>
#include <scribo/util/integral_sum_sum2_functor.hh>

namespace mln
{

  template <typename I>
  struct sauvola_functor
  {

    const I& input;
    typedef mln_ch_value(I,bool) bin_t;
    bin_t bin;

    image2d<double> th_;

    const mln_value(I)* pi;
    bool* po;

    double K_;
    double R_;

    scribo::binarization::internal::sauvola_formula formula_;

    int step_;
    unsigned next_line2;
    unsigned next_line3;
    unsigned offset1;
    unsigned offset2;

    sauvola_functor(const I& input_, mln_ch_value(I,bool)& bin_, double K, double R)
      : input(input_),
	bin(bin_),
	pi(&input(input.domain().pmin())),
	po(&bin(bin.domain().pmin())),
	K_(K),
	R_(R)
    {
      step_ = 3;
      unsigned next_line1 = 2 * input.border();
      next_line2 = input.delta_index(dpoint2d(+1,0)) + next_line1;
      next_line3 = input.delta_index(dpoint2d(+2,0)) + next_line1;

      offset1 = input.delta_index(dpoint2d(+1,0));
      offset2 = input.delta_index(dpoint2d(+2,0));
    }

    // Run every 4 pixels.
    void exec(double mean, double stddev)
    {
      static point2d p(0,0);

      double th = formula_(p, mean, stddev, K_, R_);

      for (int i = 0; i < step_; ++i, ++po, ++pi)
      {
	*po = (*pi <= th);
	*(po + offset1) = (*(pi + offset1) <= th);
	*(po + offset2) = (*(pi + offset2) <= th);
      }

#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	// Store local mean
      unsigned index = pi - input.buffer();

      internal::debug_mean.element(index) = mean * internal::mean_debug_factor;
      internal::debug_stddev.element(index) = stddev * internal::stddev_debug_factor;
      internal::debug_threshold.element(index) = t;

      double alpha = K * (1 - stddev / R);
      internal::debug_alpham.element(index) = alpha * mean * internal::alpham_debug_factor;
      internal::debug_alphacond.element(index) = (stddev < (alpha * mean / 2.));
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

    }

    void end_of_row(int)
    {
      po += next_line3;
      pi += next_line3;
    }


    void finalize()
    {
    }

  };



  image2d<bool>
  sauvola_fast(const image2d<value::int_u8>& input, unsigned win)
  {
    util::timer t;
    t.start();

    scribo::util::integral_sum_sum2_functor<value::int_u8, double> fi;
    image2d<util::couple<double,double> >
      integral = scribo::util::init_integral_image(input, 3, fi);

    t.stop();
    std::cout << "image integrale - " << t << std::endl;

#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	  initialize(internal::debug_mean, input);
	  initialize(internal::debug_stddev, input);

	  initialize(internal::debug_threshold, input);
	  initialize(internal::debug_alpham, input);
	  initialize(internal::debug_alphacond, input);
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

    t.restart();
    image2d<bool> output;
    initialize(output, input);
    sauvola_functor<image2d<value::int_u8> > f(input, output,
					       SCRIBO_DEFAULT_SAUVOLA_K,
					       SCRIBO_DEFAULT_SAUVOLA_R);
    scribo::canvas::integral_browsing(integral, 1, win / 3, win / 3, 3, f);
    t.stop();
    std::cout << "Binarization - " << t << std::endl;

    return f.bin;
  }

}


int main(int argc, char *argv[])
{
  using namespace mln;

  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);

  unsigned win = atoi(argv[2]);

  util::timer t;
  t.start();
  image2d<bool> output = sauvola_fast(input, win);
  t.stop();
  std::cout << t << std::endl;

  io::pbm::save(output, argv[3]);
}

