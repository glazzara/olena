#include "filetype.hh"

#include <mln/core/routine/duplicate.hh>
#include <mln/pw/all.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.xxx low output.xxx" << std::endl
	    << "  Low threshold." << std::endl
	    << "  xxx is pbm (2D) or dump (3D)." << std::endl;
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

  int low = std::atoi(argv[2]);
  if (low < 0 || low > 255)
    {
      std::cerr << "bad threshold!" << std::endl;
      usage(argv);
    }



  switch (get_filetype(argv[1]))
    {
    case filetype::pgm:
      {
	image2d<int_u8> ima;
	io::pgm::load(ima, argv[1]);
	io::pbm::save(duplicate((pw::value(ima) < pw::cst(low)) | ima.domain()), argv[3]);
      }
      break;

    case filetype::dump:
      {
	image3d<int_u8> ima;
	io::dump::load(ima, argv[1]);
	io::dump::save(duplicate((pw::value(ima) < pw::cst(low)) | ima.domain()), argv[3]);
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
