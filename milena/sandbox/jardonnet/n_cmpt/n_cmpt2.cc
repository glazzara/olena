
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>


#include "n_cmpt2.hh"

using namespace mln;
using namespace mln::value;

bool usage(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cout << argv[0] << " ima.pgm n_cmpt" << std::endl;
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

  unsigned n_cmpt = atoi(argv[2]);

  n_cmpt(ima, c4(), n_cmpt, out);
}
