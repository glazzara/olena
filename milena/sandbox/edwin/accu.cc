
# include <mln/core/image/image2d.hh>
# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>
# include <mln/core/var.hh>
# include <mln/util/timer.hh>

# include "accu_trait.hh"
# include "accu_trait.hh"
# include "card.hh"
# include "card.hh"
# include "algebraic.hh"
# include "algebraic.hh"
# include "leveling.hh"
# include "leveling.hh"


int main()
{
  using namespace mln;
  typedef image2d<int> I;

  I ima(1000, 1000);
  mln::morpho::accu::card<point2d> acc;

  float elapsed;
  mln::util::timer chrono;

  debug::iota(ima);
  std::cout << "50 mean of a 1000x1000 image2d<int>" << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    algebraic(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(alge) " << elapsed << "s : " << acc.to_result() << std::endl;
  /*
  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    leveling(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(auto) " << elapsed << "s : " << acc.to_result() << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    mln::impl::leveling(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(generic) " << elapsed << "s : " << acc.to_result() << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    mln::impl::leveling_fast(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(fast) " << elapsed << "s : " << acc.to_result() << std::endl;
  */
}

