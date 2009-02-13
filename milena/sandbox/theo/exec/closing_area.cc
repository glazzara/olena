#include "filetype.hh"
#include <mln/morpho/closing_area.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.xxx lambda output.xxx" << std::endl
	    << "  Area closing." << std::endl
	    << "  xxx in { pgm, dump }" << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  trace::entering("main");

  std::string filename = argv[1];
  unsigned lambda = atoi(argv[2]);

  switch (get_filetype(argv[1]))
    {
    case filetype::pgm:
      {
	image2d<int_u8> ima, clo;
	io::pgm::load(ima, argv[1]);
	clo = morpho::closing_area(ima, c4(), lambda);
	io::pgm::save(clo, argv[3]);
      }
      break;

    case filetype::dump:
      {
	image3d<int_u8> ima, clo;
	io::dump::load(ima, argv[1]);
	clo = morpho::closing_area(ima, c6(), lambda);
	io::dump::save(clo, argv[3]);
      }
      break;

    case filetype::unknown:
      std::cerr << "unknown filename extension!" << std::endl;
      usage(argv);
      break;

    default:
      std::cerr << "file type not handled!" << std::endl;
      usage(argv);
    }

  trace::exiting("main");
}
