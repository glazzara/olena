#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/util/timer.hh>

#include "integral.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> in;
  io::pgm::load(in, "in.pgm");

  util::timer t;
  t.start();
  image2d<int_u8> out = subsampling::integral(in, 3);
  float t_ = t.stop();
  std::cout << t_ << std::endl;

  io::pgm::save(out, "out.pgm");
}
