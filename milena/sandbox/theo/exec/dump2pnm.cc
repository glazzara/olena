#include "filetype.hh"

#include <mln/literal/black.hh>
#include <mln/debug/slices_2d.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.dump output.pnm" << std::endl;
  std::cerr << "  It works with binary (pbm), gray-level (int_u8), and color (rgb8) images." << std::endl;
  std::cerr << "  Sample use: binary input.dump -> output.pbm" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 3)
    usage(argv);

  trace::entering("main");

  std::string filename = argv[2];

  switch (get_filetype(argv[2]))
    {
    case filetype::pbm:
      {
	image3d<bool> vol;
	io::dump::load(vol, argv[1]);
	bool bg = false;
	image2d<bool> ima = debug::slices_2d(vol, 1.f, bg);
	io::pbm::save(ima, argv[2]);
      }
      break;

    case filetype::pgm:
      {
	using value::int_u8;
	image3d<int_u8> vol;
	io::dump::load(vol, argv[1]);
	int_u8 bg = 0;
	image2d<int_u8> ima = debug::slices_2d(vol, 1.f, bg);
	io::pgm::save(ima, argv[2]);
      }
      break;

    case filetype::ppm:
      {
	using value::rgb8;
	image3d<rgb8> vol;
	io::dump::load(vol, argv[1]);
	rgb8 bg = literal::black;
	image2d<rgb8> ima = debug::slices_2d(vol, 1.f, bg);
	io::ppm::save(ima, argv[2]);
      }
      break;

    case filetype::dump:
      {
	std::cerr << "Output shall not be a dump file!" << std::endl;
	usage(argv);
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




//   using namespace mln;
//   using value::int_u8;

//   if (argc != 3)
//     usage(argv);

//   std::string filename = argv[2];

}
