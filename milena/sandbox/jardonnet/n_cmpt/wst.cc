
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/make/image.hh>
#include <mln/core/alias/neighb1d.hh>

#include <mln/morpho/meyer_wst.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include "nwst.hh"

using namespace mln;
using namespace mln::value;

bool usage(int argc, char ** argv)
{
  if (argc != 3)
  {
    std::cout << argv[0] << " ima.pgm lambda" << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char ** argv)
{
  if (not usage(argc,argv))
    return 1;

  image2d<int_u8> ima;
  io::pgm::load(ima, argv[1]);
  unsigned lambda = atoi(argv[2]);

  ima(point2d(0,3)) = 106;

  int_u8 label;
  io::pgm::save(morpho::meyer_wst(ima, c4(), label),
                "meyer_wst.ppm");
  std::cout << label << std::endl;
}
