
#include <oln/core/2d/image2d.hh>
#include <oln/core/2d/neighb2d.hh>

#include <oln/core/2d/window2d.hh>
#include <oln/core/2d/rectangle2d.hh>
#include <oln/core/2d/hline2d.hh>
#include <oln/core/2d/vline2d.hh>

#include <oln/morpho/elementary_dilation.hh>
#include <oln/morpho/elementary_erosion.hh>

#include <oln/morpho/dilation.hh>
#include <oln/morpho/erosion.hh>

#include <oln/morpho/opening.hh>
#include <oln/morpho/closing.hh>

#include <oln/morpho/elementary_closing.hh>
#include <oln/morpho/elementary_opening.hh>

#include <oln/debug/print.hh>


template <typename I>
void my_test(I ima, int i)
{
  oln_piter(I) p1(ima.points());
  for_all(p1)
    assert(ima(p1) == i);
}

int main()
{
  using namespace oln;

  int i = 0;

  typedef image2d<bool> I;

  I ima(5, 5);

  I::piter p1(ima.points());
  for_all(p1)
    ima(p1) = i++ % 2;

  my_test( morpho::elementary_erosion(ima + c4), 0);

  my_test( morpho::elementary_dilation(ima + c4), 1 );

  my_test( morpho::elementary_opening(ima + c4), 0);

  my_test( morpho::elementary_closing(ima + c4), 1);

  my_test( morpho::erosion(ima, win3x3), 0);

  my_test( morpho::dilation(ima, win3x3), 1);

  my_test( morpho::opening(ima, win3x3), 0);

  my_test( morpho::closing(ima, win3x3), 1);
}
