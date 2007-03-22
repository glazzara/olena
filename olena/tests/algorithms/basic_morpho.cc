
#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/window2d.hh>
#include <oln/core/2d/neighb2d.hh>

#include <oln/morpho/elementary_dilation.hh>
#include <oln/morpho/elementary_erosion.hh>

#include <oln/morpho/dilation.hh>
#include <oln/morpho/erosion.hh>

#include <oln/morpho/opening.hh>
#include <oln/morpho/closing.hh>

#include <oln/morpho/elementary_closing.hh>
#include <oln/morpho/elementary_opening.hh>

#include <oln/debug/print.hh>

int main()
{
  using namespace oln;

  int i = 0;

  typedef image2d<bool> I;

  I ima(5, 5);

  I::piter p1(ima.points());
  for_all(p1)
    ima(p1) = i++ % 2;

  std::cout << "ima" << std::endl;
  debug::print(ima);


  I tmp = (morpho::elementary_erosion(ima + c4)).image();
  p1 = tmp.points();
  for_all(p1)
    assert(tmp(p1) == 0);

  //  std::cout << "elementary_dilation" << std::endl;
  tmp = (morpho::elementary_dilation(ima + c4)).image();
  p1 = tmp.points();
  for_all(p1)
    assert(tmp(p1) == 1);

  // std::cout << "erosion_w" << std::endl;
  tmp = morpho::erosion(ima, win3x3);
  p1 = tmp.points();
  for_all(p1)
    assert(tmp(p1) == 0);

  // std::cout << "dilation_w" << std::endl;
  tmp = morpho::dilation(ima, win3x3);
  p1 = tmp.points();
  for_all(p1)
    assert(tmp(p1) == 1);

  // std::cout << "elementary_opening" << std::endl;
  tmp = (morpho::elementary_opening(ima + c4)).image();
  p1 = tmp.points();
  for_all(p1)
    assert(tmp(p1) == 0);

  // std::cout << "elementary_closing" << std::endl;
  tmp = (morpho::elementary_closing(ima + c4)).image();
  p1 = (tmp.points());
  for_all(p1)
    assert(tmp(p1) == 1);

  // std::cout << "opening" << std::endl;
  tmp = morpho::opening(ima, win3x3);
  p1 = (tmp.points());
  for_all(p1)
    assert(tmp(p1) == 1);

  // std::cout << "closing" << std::endl;
  debug::print( morpho::closing(ima, win3x3) );
  p1 = (tmp.points());
  for_all(p1)
    assert(tmp(p1) == 1);
}
