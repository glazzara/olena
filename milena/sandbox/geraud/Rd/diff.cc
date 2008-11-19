#include <oln/core/2d/image2d.hh>
#include <oln/io/load_pgm.hh>
#include <oln/level/compare.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " 1.pgm 2.pgm" << std::endl
            << "(may 2007)" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 3)
    usage(argv);

  using namespace oln;
  typedef image2d<unsigned char> I;

  I ima1 = io::load_pgm(argv[1]);
  I ima2 = io::load_pgm(argv[2]);

  if (ima1 != ima2)
    std::cout << "images differ" << std::endl;
  return 0;
}
