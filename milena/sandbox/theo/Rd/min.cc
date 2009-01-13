#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " 1.pgm 2.pgm out.pgm" << std::endl
            << "(sep 2008)" << std::endl;
  exit(1);
}


template <typename I>
I min(const I& ima1, const I& ima2)
{
  mln_precondition(ima1.is_valid() and ima2.is_valid());
  mln_precondition(ima1.domain() == ima2.domain());
  I out(ima1.domain());
  mln_piter(I) p(ima1.domain());
  for_all(p)
    out(p) = ima1(p) < ima2(p) ? ima1(p) : ima2(p);
  return out;
}


int main(int argc, char* argv[])
{
  if (argc != 4)
    usage(argv);

  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;

  I ima1 = io::pgm::load<int_u8>(argv[1]);
  I ima2 = io::pgm::load<int_u8>(argv[2]);

  io::pgm::save(min(ima1, ima2), argv[3]);
}
