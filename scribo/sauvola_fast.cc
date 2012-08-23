#include <mln/core/image/image2d.hh>
#include <scribo/canvas/integral_browsing.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/util/couple.hh>
#include <mln/util/timer.hh>
#include <scribo/binarization/internal/sauvola_formula.hh>
#include <scribo/binarization/internal/sauvola_functor.hh>
#include <scribo/util/init_integral_image.hh>
#include <scribo/util/integral_sum_sum2_functor.hh>
#include <scribo/util/compute_sub_domains.hh>

#include <mln/io/dump/save.hh>

#include <mln/border/mirror.hh>
#include <mln/border/adjust.hh>

namespace mln
{

  image2d<bool>
  sauvola_fast(const image2d<value::int_u8>& input, unsigned win)
  {
    util::timer t;
    t.start();

    mln::util::array<mln::util::couple<box2d, unsigned> >
      sub_domains = scribo::util::compute_sub_domains(input, 1, 3);

    border::adjust(input, sub_domains(1).second());
    border::mirror(input);

    scribo::util::integral_sum_sum2_functor<value::int_u8, double> fi;
    image2d<util::couple<double,double> >
      integral = scribo::util::init_integral_image(input, 3, fi, sub_domains[2].first(),
						   sub_domains[2].second());

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
    scribo::binarization::internal::sauvola_functor<image2d<value::int_u8> > f(input,
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

