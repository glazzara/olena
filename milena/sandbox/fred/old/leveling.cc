

#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/all.hh>
#include <mln/util/timer.hh>
#include <mln/core/alias/neighb2d.hh>
#include "mean.hh"
#include "leveling_filter.hh"

int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  std::cout << "Leveling filter test" << std::endl;

  typedef mln::image2d<int_u8> I;
  I lena;

  float elapsed;
  mln::util::timer chrono;
  mln::morpho::attribute::mean<I> c;
  int lambda = atoi(argv[1]);

  mln::io::pgm::load(lena, "../../img/lena.pgm");
  I out;

  chrono.start();
  out = mln::canvas::morpho::dataing_filter(lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(auto) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "auto.pgm");

  chrono.start();
  out = mln::canvas::morpho::internal::dataing_filter_dispatch(mln::metal::true_(), lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(fast) " << elapsed << "s" << std::endl;

  mln::io::pgm::save(out, "fast.pgm");

  chrono.start();
  out = mln::canvas::morpho::internal::dataing_filter_dispatch(mln::metal::false_(), lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(slow) " << elapsed << "s" << std::endl;

  mln::io::pgm::save(out, "slow.pgm");
}
