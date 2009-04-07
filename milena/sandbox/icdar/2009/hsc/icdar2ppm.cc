#include "icdar2ppm.hh"

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cout << "Usage: " << argv[0] << " <input.pbm> <input.dat> <output.ppm>" << std::endl
	      << "  HSC @ ICDAR'2009" << std::endl
	      << "input.pbm  The original image. It is used to get the image dimension." << std::endl
	      << "input.dat  The \"Ground truth\" data" << std::endl
	      << "output.ppm The PPM version of input.dat." << std::endl;
    return 1;
  }

  mln::icdar2ppm(argv[1], argv[2], argv[3]);
}
