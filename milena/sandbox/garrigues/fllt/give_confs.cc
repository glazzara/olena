#include <mln/core/image/image2d.hh>
#include <mln/core/point2d.hh>
#include <mln/core/site_set/p_set.hh>
#include <mln/core/clock_neighb2d.hh>
#include <mln/core/neighb2d.hh>

#include <mln/labeling/level.hh>
#include <mln/level/fill.hh>
#include <mln/debug/println.hh>
#include <iomanip>

int main()
{
  using namespace mln;
  using namespace mln::value;

  image2d<bool> ima(3,3);
  point2d p(1,1);
  dpoint2d dp(-1,0);
  clock_neighb2d nbh = cc8(dp);
  int n_holes = 0;

  bool tab[8][8];
  for (int i = 0; i < 256; i++)
  {
    level::fill(ima, false);
    int_u8 tmp = i;

    mln_fwd_niter_(clock_neighb2d)   n(nbh , p);
    for_all(n)
    {
      if (tmp % 2)
	ima(n) = true;
      tmp = tmp >> 1;
    }


    int x, y;
    labeling::level(ima, false, c8(), x);


    ima(p) = true;
    labeling::level(ima, false, c8(), y);

//     std::cout << "----- conf no " << i << "------" << std::endl;
//     debug::println(ima);
//     std::cout << "----- " << x << " holes before------" << std::endl;
//     std::cout << "----- " << y << " holes after------" << std::endl;
//     std::cout << "----- " << y - x << " holes added------" << std::endl << std::endl;

    std::cout <<  std::setw(2) << y - x << ", ";
    if (! ((i + 1) % 4)) std::cout << " ";
    if (! ((i + 1) % 16)) std::cout << std::endl;
    if (! ((i + 1) % 64)) std::cout <<  std::endl;
  }
}
