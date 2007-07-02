#include <cmath>

#include <mln/core/image2d.hh>
#include <mln/level/fill.hh>
#include <mln/debug/println.hh>

#include <mln/core/window2d.hh>
#include <mln/core/rectangle2d.hh>

#include <mln/core/neighb2d.hh>

#include <mln/morpho/erosion.hh>
#include <mln/morpho/Rd.hh>


typedef unsigned char int_u8;


int_u8 cos_sin(const mln::point2d& p)
{
  return (int_u8)(255 * std::cos(float(p.row())) * std::sin(float(p.col())));
}


int main()
{
  using namespace mln;

  const unsigned size = 1000;
  image2d<int_u8> f(size, size);
  morpho::Rd(f, f, c8());
}



//   box2d b = mk_box2d(/* row = */ 1, 3,
// 		     /* col = */ 4, 6);
//   std::cout << b << std::endl;

//   bool w[] =
//     { 0, 1, 0,
//       0, 1, 0,
//       1, 0, 0 };
//   window2d win = mk_window2d(w);
//   std::cout << win << std::endl;
//   std::cout << c8() << std::endl;

//     {
//   image2d<int> ima(b);
//   level::fill(ima, 51);
//   debug::println(ima);

//   std::cout << win << std::endl;

//   morpho::erosion(ima, win);

//   rectangle2d rec(1, 2);
//   std::cout << rec << std::endl;
//   }


//   {
//     image2d<int> ima(b);
//     level::fill(ima, cos_sin);
//     debug::println(ima);

//     std::cout << std::endl;

//     image2d<int> ima2 = morpho::erosion(ima, win);
//     debug::println(ima2);
//   }
