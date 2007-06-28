#include <cmath>

#include <core/image2d.hh>
#include <level/fill.hh>
#include <debug/println.hh>

#include <core/window2d.hh>
#include <core/rectangle2d.hh>

#include <morpho/erosion.hh>


int cos_sin(const mln::point2d& p)
{
  return int(255 * std::cos(float(p.row())) * std::sin(float(p.col())));
}


int main()
{
  using namespace mln;

  box2d b = mk_box2d(/* row = */ 1, 3,
		     /* col = */ 4, 6);
  std::cout << b << std::endl;

  bool w[] =
    { 0, 1, 0,
      0, 1, 0,
      1, 0, 0 };
  window2d win = mk_window2d(w);

  /*
    {
  image2d<int> ima(b);
  level::fill(ima, 51);
  debug::println(ima);

  std::cout << win << std::endl;

  morpho::erosion(ima, win);

  rectangle2d rec(1, 2);
  std::cout << rec << std::endl;
  }
  */


  {
    image2d<int> ima(b);
    level::fill(ima, cos_sin);
    debug::println(ima);

    std::cout << std::endl;

    image2d<int> ima2 = morpho::erosion(ima, win);
    debug::println(ima2);
  }
}
