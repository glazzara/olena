#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/util/timer.hh>

#include "debase.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> in;
  io::pgm::load(in, "in.pgm");

  util::timer t;
  t.start();
  image2d<int_u8> out = subsampling::debase(in, 3, dpoint2d(1,1));
  float t_ = t.stop();
  std::cout << t_ << std::endl;

  io::pgm::save(out, "out.pgm");
}
