#include <core/image2d.hh>
#include <level/fill.hh>
#include <debug/println.hh>

#include <core/window2d.hh>
#include <core/rectangle2d.hh>


int main()
{
  using namespace mln;

  box2d b = mk_box2d(/* row = */ 1, 2,
		     /* col = */ 3, 5);
  std::cout << b << std::endl;

  image2d<int> ima(b);
  level::fill(ima, 51);
  debug::println(ima);

  bool w[] =
    { 0, 1, 0,
      0, 1, 0,
      1, 0, 0 };
  window2d win = mk_window2d(w);
  std::cout << win << std::endl;

  debug::println(ima, win);

  rectangle2d rec(1, 2);
  std::cout << rec << std::endl;
}
