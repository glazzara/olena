#include "filetype.hh"

#include <mln/morpho/closing/structural.hh>
#include <mln/win/disk2d.hh>
#include <mln/win/sphere3d.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.xxx r output.xxx" << std::endl
	    << "  Closing with an isotropic structuring element." << std::endl
	    << "  xxx is pbm (2D) or dump (3D)." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 4)
    usage(argv);

  trace::entering("main");

  int r = std::atoi(argv[2]);
  if (r < 0)
    {
      std::cerr << "bad radius!" << std::endl;
      usage(argv);
    }


  std::string filename = argv[1];

  switch (get_filetype(argv[1]))
    {
    case filetype::pbm:
      {
	image2d<bool> ima, out;
	io::pbm::load(ima, argv[1]);
	out = morpho::closing::structural(ima, win::disk2d(2 * r + 1));
	io::pbm::save(out, argv[3]);
      }
      break;

    case filetype::dump:
      {
	image3d<bool> ima, out;
	io::dump::load(ima, argv[1]);
	out = morpho::closing::structural(ima, win::sphere3d(2 * r + 1));
	io::dump::save(out, argv[3]);
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
