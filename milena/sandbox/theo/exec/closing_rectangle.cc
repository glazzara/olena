#include "filetype.hh"

#include <mln/morpho/closing/structural.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.xxx height width output.xxx" << std::endl
	    << "  Height and width are odd positive integers." << std::endl
	    << "  xxx is pbm or pgm." << std::endl
	    << "  Rectangle closing on a 2D image." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  trace::entering("main");

  if (argc != 5)
    usage(argv);

  int height = atoi(argv[2]);
  if (height < 0 || height % 2 == 0)
    usage(argv);

  int width  = atoi(argv[3]);
  if (width < 0 || width % 2 == 0)
    usage(argv);


  std::string filename = argv[1];

  switch (get_filetype(argv[1]))
    {
    case filetype::pbm:
      {
	image2d<bool> ima, clo;
	io::pbm::load(ima, argv[1]);
	clo = morpho::closing::structural(ima, win::rectangle2d(height, width));
	io::pbm::save(clo, argv[4]);
      }
      break;

    case filetype::pgm:
      {
	image2d<int_u8> ima, clo;
	io::pgm::load(ima, argv[1]);
	clo = morpho::closing::structural(ima, win::rectangle2d(height, width));
	io::pgm::save(clo, argv[4]);
      }
      break;
    };


  trace::exiting("main");
}