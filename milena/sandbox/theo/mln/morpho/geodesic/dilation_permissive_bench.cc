#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/println.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/util/timer.hh>

#include "dilation_permissive.hh"


int main()
{
  using namespace mln;

  image2d<bool>
    f = io::pbm::load("./+f.pbm"),
    g = io::pbm::load("./+g.pbm"),
    o;
  neighb2d nbh = c4();

  util::timer t;
  const unsigned n = 20;

  {
    t.start();
    for (unsigned i = 0; i < n; ++i)
      o = morpho::geodesic::impl::dilation_strict_1_set_formula(f, g, nbh);
    std::cout << "dilation_strict_1_set_formula " << t.stop() << std::endl;
    io::pbm::save(o, "o.pbm");
  }

  {
    t.start();
    for (unsigned i = 0; i < n; ++i)
      o = morpho::geodesic::impl::dilation_permissive_1_set_generic(f, g, nbh);
    std::cout << "dilation_permissive_1_set_generic " << t.stop() << std::endl;
  }

  {
    t.start();
    for (unsigned i = 0; i < n; ++i)
      o = morpho::geodesic::impl::dilation_permissive_1_set_fastest(f, g, nbh);
    std::cout << "dilation_permissive_1_set_fastest " << t.stop() << std::endl;
//     io::pbm::save(o, "o.pbm");
  }

  {
    t.start();
    for (unsigned i = 0; i < n; ++i)
      o = morpho::geodesic::impl::dilation_permissive_1_set_generic_alt(f, g, nbh);
    std::cout << "dilation_permissive_1_set_generic_alt " << t.stop() << std::endl;
  }

  {
    t.start();
    for (unsigned i = 0; i < n; ++i)
      o = morpho::geodesic::impl::dilation_permissive_1_set_fastest_alt(f, g, nbh);
    std::cout << "dilation_permissive_1_set_fastest_alt " << t.stop() << std::endl;
  }
}
