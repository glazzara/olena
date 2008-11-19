#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/blobs.hh>
#include <mln/level/fill.hh>
#include <mln/debug/println.hh>
#include "simple_point.hh"


int main()
{
  using namespace mln;
  using namespace mln::value;

  typedef image2d<bool> I;
  image2d<bool> ima(3,3);
  point2d p(1,1);

  std::cout << "----- Object in C8 ------" << std::endl;

  for (unsigned i = 0; i < 256; i++)
  {
    level::fill(ima, false);
    int_u8 tmp = i;

    mln_niter_(neighb2d)   n(c8() , p);
    for_all(n)
    {
      if (tmp % 2)
	ima(n) = true;
      tmp = tmp >> 1;
    }

    unsigned x;
    labeling::blobs(ima, c8(), x);

    mln_assertion(nb_connexity2d(ima, 8, p, true) == x);
  }

  std::cout << "----- Object in C4 ------" << std::endl;

  for (unsigned i = 0; i < 256; i++)
  {
    level::fill(ima, false);
    int_u8 tmp = i;

    mln_niter_(neighb2d)   n(c8() , p);
    for_all(n)
    {
      if (tmp % 2)
	ima(n) = true;
      tmp = tmp >> 1;
    }


    unsigned x;
    labeling::blobs(ima, c4(), x);

    mln_assertion(nb_connexity2d(ima, 4, p, true) == x);
  }
}
