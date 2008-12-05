
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/make/image.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include "n_cmpt3.hh"

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

  int_u8 tab[] = {2,3,1,0,2,3,2,2,1};
  image1d<int_u8> ima1= make::image(tab);
  
  io::pgm::save(n_cmpt::n_cmpt3(ima1, c4(), lambda),
                "out.pgm");
}
