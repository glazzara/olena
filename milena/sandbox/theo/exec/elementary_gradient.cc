#include "filetype.hh"
#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/elementary/gradient_internal.hh>
#include <mln/morpho/elementary/gradient_external.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.xxx kind output.xxx" << std::endl
	    << "  Elementary gradient." << std::endl
	    << "  kind is 0 (external), 1 (internal), or 2 (thick)." << std::endl
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
  int kind = std::atoi(argv[2]);
  if (kind < 0 || kind > 2)
    {
      std::cerr << "bad kind!" << std::endl;
      usage(argv);
    }

  switch (get_filetype(argv[1]))
    {
    case filetype::pgm:
      {
	image2d<int_u8> ima, grad;
	io::pgm::load(ima, argv[1]);
	switch (kind)
	  {
	  case 0:
	    grad = morpho::elementary::gradient_external(ima, c4());
	    break;
	  case 1:
	    grad = morpho::elementary::gradient_internal(ima, c4());
	    break;
	  case 2:
	    grad = morpho::elementary::gradient(ima, c4());
	    break;
	  }
	io::pgm::save(grad, argv[3]);
      }
      break;

    case filetype::dump:
      {
	image3d<int_u8> ima, grad;
	io::dump::load(ima, argv[1]);
	switch (kind)
	  {
	  case 0:
	    grad = morpho::elementary::gradient_external(ima, c6());
	    break;
	  case 1:
	    grad = morpho::elementary::gradient_internal(ima, c6());
	    break;
	  case 2:
	    grad = morpho::elementary::gradient(ima, c6());
	    break;
	  }
	io::dump::save(grad, argv[3]);
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
