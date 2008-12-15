
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/make/image.hh>
#include <mln/core/alias/neighb1d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/io/tikz/save.hh>

#include <cmath>

#include "tikz.hh"

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

  image2d<int_u8> cos_(6,6);
  for (int i = 0; i <= 6; i++)
    for (int j = 0; j <= 6; j++)
      cos_(point2d(i,j)) = (sin(i) * cos(j) + 1)/2*255;
  io::pgm::save(cos_, "cos.pgm");

  int_u8 tree[5][5] =
    {
      {100, 160, 160,  100,  100},
      {160, 220, 220,  100,  100},
      {160, 160, 220,  100,  100},
      {220, 160, 100,  100,  100},
      {100, 160, 100,  160,  160},
    };

  io::tikz::save(make::image(tree), "tree.tex");

  int_u8 tab[] = {2,3,1,0,2,3,4,5,1,1,0,5,6,8,7,1,1,2,3,4};
  image1d<int_u8> ima1= make::image(tab);

  io::pgm::save(n_cmpt::n_cmpt3(ima, c4(), lambda), "out.pgm");
}
