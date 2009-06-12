
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/accu/shape/volume.hh>

#include "n_cmpt.hh"

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

  int lambda = atoi(argv[2]);

  image2d<int_u8> out(ima.domain());

  n_cmpt::algebraic_union_find(ima, c4(), lambda, out);

  io::pgm::save(out, "out.pgm");
}

