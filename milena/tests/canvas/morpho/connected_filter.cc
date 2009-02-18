

#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/all.hh>
#include <mln/util/timer.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/attribute/volume.hh>
#include <mln/morpho/attribute/card.hh>
#include <mln/canvas/morpho/connected_filter.hh>

int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  if (argc < 2)
    return 1;

  std::cout << "Leveling filter test" << std::endl;

  typedef mln::image2d<int_u8> I;
  I lena;

  float elapsed;
  mln::util::timer chrono;
  mln::morpho::attribute::volume<I> c;
  mln::morpho::attribute::card<I> c2;
  int lambda = atoi(argv[1]);

  mln::io::pgm::load(lena, "../../../../img/lena.pgm");
  I out;

  std::cout << "Test algebraic" << std::endl;
  chrono.start();
  out = mln::canvas::morpho::connected_filter(lena, c4(), c2, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(auto) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "alg_auto.pgm");

  chrono.start();
  out = mln::canvas::morpho::algebraic_filter(lena, c4(), c2, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(fast) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "alg_fast.pgm");

  chrono.start();
  out = mln::canvas::morpho::algebraic_filter(lena, c4(), c2, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(slow) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "alg_slow.pgm");

// Try force algebraic dispatch with wrong accu (volume).
// out = mln::canvas::morpho::algebraic_filter(lena, c4(), c, lambda, true);

  std::cout << "Test leveling" << std::endl;
  chrono.start();
  out = mln::canvas::morpho::connected_filter(lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(auto) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "lev_auto.pgm");

  chrono.start();
  out = mln::canvas::morpho::leveling_filter(lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(fast) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "lev_fast.pgm");

  chrono.start();
  out = mln::canvas::morpho::leveling_filter(lena, c4(), c, lambda, true);
  elapsed = chrono.stop();
  std::cout << "(slow) " << elapsed << "s" << std::endl;
  mln::io::pgm::save(out, "lev_slow.pgm");

// Try force leveling dispatch with wrong accu (card).
// mln::canvas::morpho::leveling_filter(lena, c4(), c2, lambda, true);


}
