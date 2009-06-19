#include "filetype.hh"

#include <mln/make/image3d.hh>




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " output.dump input1.xxx .. inputn.xxx" << std::endl;
  std::cerr << "  It works with binary (pbm), gray-level (int_u8), and color (rgb8) images." << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc < 3)
    usage(argv);

  trace::entering("main");

  std::string filename = argv[2];

  switch (get_filetype(argv[2]))
    {
    case filetype::pbm:
      {
	typedef image2d<bool> I;
	util::array<I> arr;
	for (int i = 2; i < argc; ++i)
	  {
	    I ima;
	    io::pbm::load(ima, argv[i]);
	    arr.append(ima);
	  }
	image3d<bool> vol = make::image3d(arr);
	io::dump::save(vol, argv[1]);
      }
      break;

    case filetype::pgm:
      {
	using value::int_u8;
	typedef image2d<int_u8> I;
	util::array<I> arr;
	for (int i = 2; i < argc; ++i)
	  {
	    I ima;
	    io::pgm::load(ima, argv[i]);
	    arr.append(ima);
	  }
	std::cout << "nbr elements: " << arr.nelements() << std::endl;
	image3d<int_u8> vol = make::image3d(arr);
	io::dump::save(vol, argv[1]);
      }
      break;

    case filetype::ppm:
      {
	using value::rgb8;
	typedef image2d<rgb8> I;
	util::array<I> arr;
	for (int i = 2; i < argc; ++i)
	  {
	    I ima;
	    io::ppm::load(ima, argv[i]);
	    arr.append(ima);
	  }
	image3d<rgb8> vol = make::image3d(arr);
	io::dump::save(vol, argv[1]);
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

}
