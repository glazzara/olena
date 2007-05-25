#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/neighb2d.hh>

#include <oln/debug/println.hh>
#include <oln/io/load_pgm.hh>
#include <oln/io/save_pgm.hh>
#include <oln/morpho/Rd/union_find.hh>


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " f.pgm g.pgm c output.pgm" << std::endl
            << "reconstruction by dilation (union_find version; may 2007)" << std::endl
            << "f = marker (to be dilated)" << std::endl
            << "g = mask (constraint >= f)" << std::endl
            << "c: 4 or 8" << std::endl;
  exit(1);
}


int main(int argc, char* argv[])
{
  if (argc != 5)
    usage(argv);

  using namespace oln;
  typedef image2d<unsigned char> I;

  int c = atoi(argv[3]);
  if (c != 4 and c != 8)
    usage(argv);

  I f = io::load_pgm(argv[1]);
  I g = io::load_pgm(argv[2]);
  
  if (not (f <= g))
    {
      std::cerr << "pb" << std::endl;
      return 1;
    }

  io::save_pgm(morpho::Rd::union_find(f, g,
				      (c == 4 ? c4 : c8)),
	       argv[4]);
  return 0;
}
