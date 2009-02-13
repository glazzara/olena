#include "filetype.hh"

#include <mln/value/label_16.hh>
#include <mln/labeling/fill_holes.hh>
#include <mln/logical/not.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.xxx output.xxx" << std::endl
	    << "  Clean holes." << std::endl
	    << "  xxx is pbm (2D) or dump (3D)." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  trace::entering("main");

  std::string filename = argv[1];

  switch (get_filetype(argv[1]))
    {
    case filetype::pbm:
      {
	image2d<bool> ima, out;
	io::pbm::load(ima, argv[1]);
	value::label_16 n;
	out = labeling::fill_holes(ima, c4(), n);
	out = labeling::fill_holes(logical::not_(out), c4(), n);
	io::pbm::save(out, argv[2]);
      }
      break;

    case filetype::dump:
      {
	image3d<bool> ima, out;
	io::dump::load(ima, argv[1]);
	value::label_16 n;
	out = labeling::fill_holes(ima, c6(), n);
	out = labeling::fill_holes(logical::not_(out), c6(), n);
	io::dump::save(out, argv[2]);
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
