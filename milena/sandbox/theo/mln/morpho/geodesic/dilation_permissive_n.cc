#include <cstdlib>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/logical/and.hh>
#include <mln/util/timer.hh>

#include "dilation_permissive.hh"


int main(int argc, char* argv[])
{
  using namespace mln;


  image2d<bool>
    f = io::pbm::load("./+f.pbm"),
    g = io::pbm::load("./+g.pbm"),
    o;
  neighb2d nbh = c8();

  int n = std::atoi(argv[1]);

//   image2d<bool> f_g = logical::and_(f, g);
//   io::pbm::save(f_g, "f_g.pbm");
//   o = morpho::geodesic::dilation_permissive(f_g, g, nbh, n);

  util::timer t;
  t.start();
  o = morpho::geodesic::dilation_permissive(f, g, nbh, n);
  std::cout << t.stop() << std::endl;

  io::pbm::save(o, "o.pbm");
}
