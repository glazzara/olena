#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/level/compare.hh>
#include <mln/value/int_u8.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " 1.pgm 2.pgm out.pgm" << std::endl
            << "(october 2008)" << std::endl;
  exit(1);
}


unsigned char diff_abs(unsigned char c1,
		       unsigned char c2)
{
  return c1 > c2 ? c1 - c2 : c2 - c1;
}


int main(int argc, char* argv[])
{
  if (argc != 4)
    usage(argv);

  using namespace mln;
  typedef image2d<value::int_u8> I;

  I ima1;
  io::pgm::load(ima1, argv[1]);
  I ima2;
  io::pgm::load(ima2, argv[2]);

  I out(ima1.domain());
  mln_piter_(I) p(ima1.domain());
  for_all(p)
    out(p) = diff_abs(ima1(p), ima2(p));

  io::pgm::save(out, argv[3]);
  return 0;
}
